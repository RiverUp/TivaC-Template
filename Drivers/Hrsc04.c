#include "Hcsr04.h"

int distanceCount;
float distance;

// pa7捕获 pa6触发

void initHcsr04()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_6);
    GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_7);
    GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_7 | GPIO_PIN_6,
                     GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPD);

    IntEnable(INT_GPIOA);
    GPIOIntTypeSet(GPIO_PORTA_BASE, GPIO_PIN_7, GPIO_BOTH_EDGES);
    GPIOIntEnable(GPIO_PORTA_BASE, GPIO_PIN_7);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
    TimerConfigure(TIMER2_BASE, TIMER_CFG_ONE_SHOT);
    // 4分频-》定时器2 10mhz
    TimerPrescaleSet(TIMER2_BASE, TIMER_A, 2);
    TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
    IntEnable(INT_TIMER2A);
}

void triggerHcsr04()
{
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, GPIO_PIN_6);
    delayus(15);
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 0);
}
