#include "Control.h"

int Track_Bias;
int Velocity_PWM = 500;
int Track_Turn_PWM;
float Track_Turn_Kp = 300;

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
    // Track_Bias为正，，左轮加速
    Track_Turn_PWM = trackTurn(Track_Bias);

    int Motor_Left = Velocity_PWM + Track_Turn_PWM;
    int Motor_Right = Velocity_PWM - Track_Turn_PWM;
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
    general_bias = 0.84 * bias + 0.16 * last_bias;

    turn = general_bias * Track_Turn_Kp / 90;

    return turn;
}
