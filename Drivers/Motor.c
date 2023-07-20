#include "Motor.h"

// pc4 pb6输出 pa2使能 pd2(左) pd3(右）方向,低电平正转，高电平反转
void initMotor(void)
{
    // pwm
    // 因为设置了时钟总线是40MHz，所以在这里分一下频设置为4分频，那么PWM时钟就是10MHz
    SysCtlPWMClockSet(SYSCTL_PWMDIV_4);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
    GPIOPinTypePWM(GPIO_PORTC_BASE, GPIO_PIN_4);
    GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6);
    GPIOPinConfigure(GPIO_PC4_M0PWM6);
    GPIOPinConfigure(GPIO_PB6_M0PWM0);
    // 设置PWM0模块的第三个发生器为向下计数与不同步计数
    PWMGenConfigure(PWM0_BASE, PWM_GEN_3,
                    PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
    PWMGenConfigure(PWM0_BASE, PWM_GEN_0,
                    PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);

    // 方向
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    GPIODirModeSet(GPIO_PORTD_BASE, GPIO_PIN_2 | GPIO_PIN_3, GPIO_DIR_MODE_OUT);
    GPIOPadConfigSet(GPIO_PORTD_BASE,
                     GPIO_PIN_2 | GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD); // 推挽输出

    // 使能
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIODirModeSet(GPIO_PORTA_BASE, GPIO_PIN_2, GPIO_DIR_MODE_OUT);
    GPIOPadConfigSet(GPIO_PORTA_BASE,
                     GPIO_PIN_2, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD); // 推挽输出
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, 0);                       // PA2输出低电平
}

/*
 *关闭电机
 */
void turnOffMotor(void)
{
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, 0); // PA2输出低电平
}

/*
 * 打开电机
 */
void turnOnMotor(void)
{
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, GPIO_PIN_2); // PA2输出高电平
}
//之前的最大值相当于现在设置10000
void setPWM(int left, int right)
{
    // 正负决定方向
    if (left > 0)
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, 0);
    else
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, GPIO_PIN_2);
    if (right > 0)
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, 0);
    else
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_PIN_3);

    int pwm_left = abs(left);
    int pwm_right =abs(right);
    // 设置PWM0模块的第三个发生器每个计数周期为2000个数，而PWM时钟为10MHz，
    // 那么PWM输出频率就是10^7/2000为5KHz，但是这个数为16位寄存器，不能超过65535
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, SysCtlPWMClockGet() / pwm_right);
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, SysCtlPWMClockGet() / pwm_left);

    // 设置PWM0模块的第六个通道的占空比为10%，这个函数的第三个参数为周期内的高电平数，
    // 所以可以通过PWMGenPeriodGet得出一个周期内的计数总数再乘0.1然后减1就行
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_3) * 0.1 - 1);
    // 同理，只不过是设置第七个通道

    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0) * 0.1 - 1);
    // 使能第六个通道
    PWMOutputState(PWM0_BASE, PWM_OUT_6_BIT, true);

    PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT, true);
    // 使能第三个发生器
    PWMGenEnable(PWM0_BASE, PWM_GEN_3);
    PWMGenEnable(PWM0_BASE, PWM_GEN_0);
}
