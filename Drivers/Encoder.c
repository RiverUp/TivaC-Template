#include "Encoder.h"

int EncoderRight, EncoderLeft;

// PA4,PA5,PE2,PE3
void initEncoder()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_2 | GPIO_PIN_3);
    IntEnable(INT_GPIOE);
    GPIOIntTypeSet(GPIO_PORTE_BASE, GPIO_PIN_2 | GPIO_PIN_3, GPIO_BOTH_EDGES);
    GPIOIntEnable(GPIO_PORTE_BASE, GPIO_PIN_2 | GPIO_PIN_3);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    IntEnable(INT_GPIOA);
    GPIOIntTypeSet(GPIO_PORTA_BASE, GPIO_PIN_4 | GPIO_PIN_5, GPIO_BOTH_EDGES);
    GPIOIntEnable(GPIO_PORTA_BASE, GPIO_PIN_4 | GPIO_PIN_5);
}

int readEncoder(uint8_t side)
{
    int coder_value = 0;
    if (!side)
    {
        coder_value = EncoderRight;
        EncoderRight = 0;
    }
    else
    {
        coder_value = EncoderLeft;
        EncoderLeft = 0;
    }
    return coder_value;
}