#include "Control.h"

int Track_Bias;
int Base_Velocity = 6000;
int Track_Turn_Kp = 40;

void initControl()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    // 5ms
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet() / 200);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    IntEnable(INT_TIMER1A);
    TimerEnable(TIMER0_BASE, TIMER_A);
}

void Control()
{
    
}

void limitPWM(int input, int max, int min)
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

    turn = general_bias * Track_Turn_Kp;

    return turn;
}