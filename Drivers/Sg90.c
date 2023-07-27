#include "Sg90.h"

int UnitValue;

void initSg90(void)
{
    SysCtlPWMClockSet(SYSCTL_PWMDIV_32);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_0);
    GPIOPinConfigure(GPIO_PD0_M1PWM0);
    PWMGenConfigure(PWM1_BASE, PWM_GEN_0,
                    PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, 25000);
    PWMOutputState(PWM1_BASE, PWM_OUT_0_BIT, true);
    PWMGenEnable(PWM1_BASE, PWM_GEN_0);

    UnitValue = 25;
}

void rotateSg90(int angle)
{
    int value = 5 * angle / 9 + 25;
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, value * UnitValue);
    PWMGenEnable(PWM1_BASE, PWM_GEN_0);
}
