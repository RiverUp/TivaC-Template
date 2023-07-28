#include "Control.h"

int Track_Bias, Target_Angle;
float Basic_Velocity = 6000;
float Target_Velocity = -10;
float Target_Distance = 25;
int Velocity_PWM;
int Track_Turn_PWM, Rotate_Turn_PWM;
float Velocity_Kp = 8000, Velocity_Ki = 40;
float Track_Turn_Kp = 3000, Rotate_Turn_Kp = 5000;
float Distance_Kp = 3000, Distance_Kd = 3000;
int CrossNum;

void initControl()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    // 5ms
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet() / 200);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    IntEnable(INT_TIMER0A);
    TimerEnable(TIMER0_BASE, TIMER_A);
}

void Control()
{
    // if (CrossPassDelayFlag.flag)
    //     passCross();

    // Track_Bias为正，，左轮加速
    limitDistance(distance);
    Track_Turn_PWM = trackTurn(Track_Bias);
    Velocity_PWM = Basic_Velocity;
    Rotate_Turn_PWM = rotateTurn();

    int Motor_Left = Velocity_PWM + Track_Turn_PWM + Rotate_Turn_PWM;
    int Motor_Right = Velocity_PWM - Track_Turn_PWM - Rotate_Turn_PWM;
    Motor_Left = limitPWM(Motor_Left, 11000, -11000);
    Motor_Right = limitPWM(Motor_Right, 11000, -11000);

    setPWM(Motor_Left, Motor_Right);
}

int velocity(int encoderLeft, int encoderRight)
{
    static float general_bias, last_bias, velocity, integral;

    float bias = Target_Velocity - (encoderLeft + encoderRight) / 2;
    general_bias = 0.16 * bias + 0.84 * last_bias;
    last_bias = general_bias;
    integral += general_bias;
    if (integral > 1000)
        integral = 1000;
    else if (integral < -1000)
        integral = -1000;

    velocity = general_bias * Velocity_Kp / 100 + integral * Velocity_Ki / 100;

    return velocity;
}

int limitPWM(int input, int max, int min)
{
    int output;
    if (input < min)
        output = min;
    else if (input > max)
        output = max;
    else
        output = input;

    return output;
}

void limitDistance(int distance)
{
    static float general_bias, last_bias, pre_bias, derivative;
    general_bias = distance - Target_Distance;
    derivative = general_bias - 2 * last_bias + pre_bias;
    float increment = derivative * Distance_Kd/2000 + general_bias * Distance_Kp/2000;
    pre_bias = last_bias;
    last_bias = general_bias;

    if(distance>40||distance==0)
        increment=0;

    Basic_Velocity += increment;

    Basic_Velocity=limitPWM(Basic_Velocity,7000,2000);
}

int trackTurn(float bias)
{
    static float turn, general_bias, last_bias;
    general_bias = 0.16 * bias + 0.84 * last_bias;

    turn = general_bias * Track_Turn_Kp / 10;

    return turn;
}

int rotateTurn()
{
    static float turn, general_bias, last_bias;
    float temp = abs(Yaw - Target_Angle);
    if (temp > 180)
        general_bias = (360 - temp) * 0.16 + 0.84 * last_bias;
    else
        general_bias = temp * 0.16 + 0.84 * last_bias;
    if (RotateRightFlag || RotateLeftFlag)
    {
        if (general_bias < 4)
        {
            RotateRightFlag = RotateLeftFlag = false;
            Basic_Velocity = 500;
            return 0;
        }
    }
    last_bias = general_bias;
    // 旋转的时候不循迹
    if (RotateLeftFlag)
    {
        turn = -general_bias * Rotate_Turn_Kp / 20;
        Track_Turn_PWM = 0;
    }
    else if (RotateRightFlag)
    {
        turn = general_bias * Rotate_Turn_Kp / 20;
        Track_Turn_PWM = 0;
    }
    else
    {
        turn = 0;
    }
    return turn;
}

void passCross()
{
    countDelay(&CrossPassDelayFlag);
    if (CrossPassDelayFlag.trigger)
    {
        Basic_Velocity = 0;
        setRotateTarget(90, RIGHT, Yaw);
        CrossPassDelayFlag.trigger = false;
    }
}

void setRotateTarget(int angle, bool direction, int current_yaw)
{
    if (direction == RIGHT)
    {
        if (current_yaw + angle >= 180)
            Target_Angle = current_yaw + angle - 360;
        else
            Target_Angle = current_yaw + angle;
        RotateRightFlag = true;
    }
    if (direction == LEFT)
    {
        if (current_yaw - angle < -180)
            Target_Angle = current_yaw - angle + 360;
        else
            Target_Angle = current_yaw - angle;
        RotateLeftFlag = true;
    }
}
