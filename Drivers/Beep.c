#include "Beep.h"

void initBeep(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_3);
}

void ring(int time)
{
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, GPIO_PIN_3);
    delayms(time);
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0);
}