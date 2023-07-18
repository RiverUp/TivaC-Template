#include "Lights.h"

enum Color lightColor;

void initLights()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
}

void turnOnLights(enum Color color)
{
    switch (color)
    {
    case Red:
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
        break;
    case Blue:
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
        break;
    case Green:
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
        break;
    default:
        break;
    }
}

void turnOffLights(enum Color color)
{
    switch (color)
    {
    case Red:
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
        break;
    case Blue:
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
        break;
    case Green:
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
        break;
    default:
        break;
    }
}