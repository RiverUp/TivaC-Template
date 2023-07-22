#include "Control.h"

int Track_Bias, Target_Angle;
float Basic_Velocity = 500;
int Velocity_PWM;
int Track_Turn_PWM, Rotate_Turn_PWM;
float Track_Turn_Kp = 300, Rotate_Turn_Kp = 300;
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
    if (CrossPassDelayFlag.flag)
        passCross();

    // Track_Bias为正，，左轮加速
    Track_Turn_PWM = trackTurn(Track_Bias);
    Velocity_PWM = Basic_Velocity;
    Rotate_Turn_PWM = rotateTurn();

    int Motor_Left = Velocity_PWM + Track_Turn_PWM + Rotate_Turn_PWM;
    int Motor_Right = Velocity_PWM - Track_Turn_PWM - Rotate_Turn_PWM;
    Motor_Left = limitPWM(Motor_Left, 1000, -1000);
    Motor_Right = limitPWM(Motor_Right, 1000, -1000);

    setPWM(Motor_Left, Motor_Right);
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

int trackTurn(float bias)
{
    static float turn, general_bias, last_bias;
    general_bias = 0.16 * bias + 0.84 * last_bias;

    turn = general_bias * Track_Turn_Kp / 10;

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

int rotateTurn()
{
    static float turn, general_bias, last_bias;
    float temp = abs(Yaw - Target_Angle);
    if (temp > 180)
        general_bias = (360 - temp) * 0.16 + 0.84 * last_bias;
    else
        general_bias = temp * 0.16 + 0.84 * last_bias;
    if (general_bias < 4)
    {
        RotateRightFlag = RotateLeftFlag = false;
        Basic_Velocity = 500;
        return 0;
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
