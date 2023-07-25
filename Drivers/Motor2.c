#include "Motor2.h"
// pc4 pc5输出 pd2 pd3B的方向 pb6 pb7A的方向
void initMotor2()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
    SysCtlPWMClockSet(SYSCTL_PWMDIV_4);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    // SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinTypePWM(GPIO_PORTC_BASE, GPIO_PIN_4);
    GPIOPinTypePWM(GPIO_PORTC_BASE, GPIO_PIN_5);
    GPIOPinConfigure(GPIO_PC4_M0PWM6);
    GPIOPinConfigure(GPIO_PC5_M0PWM7);
    PWMGenConfigure(PWM0_BASE, PWM_GEN_3,
                    PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
    // 1mhz
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, 1000);
    PWMOutputState(PWM0_BASE, PWM_OUT_6_BIT, true);
    PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, true);
    PWMGenEnable(PWM0_BASE, PWM_GEN_3);

    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE,GPIO_PIN_2|GPIO_PIN_3);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE,GPIO_PIN_6|GPIO_PIN_7);
                                                                                                               //    GPIODirModeSet(GPIO_PORTD_BASE,
                                                                                                               //                   GPIO_PIN_6 | GPIO_PIN_7, GPIO_DIR_MODE_OUT);

    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_2);
    // GPIODirModeSet(GPIO_PORTA_BASE, GPIO_PIN_2, GPIO_DIR_MODE_OUT);
    // GPIOPadConfigSet(GPIO_PORTA_BASE,
    //                  GPIO_PIN_2, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, 0);
}

void turnOffMotor2(void)
{
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, 0); // PA2输出低电平
}

void turnOnMotor2(void)
{
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, GPIO_PIN_2); // PA2输出高电平
}

void setPWM2(int left, int right)
{
    if (left > 0)
    {
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, GPIO_PIN_2);
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, 0);
    }
    else
    {
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, 0);
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_PIN_3);
    }

    if (right > 0)
    {
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6, GPIO_PIN_6);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7, 0);
    }
    else
    {
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6, 0);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7, GPIO_PIN_7);
    }
    int pwm_left = abs(left);
    int pwm_right = abs(right);

    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6, pwm_left);
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, pwm_right);
}
