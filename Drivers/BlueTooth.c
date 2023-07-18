#include "BlueTooth.h"

char blueToothDataBuffer[SERIAL_BUFFER_SIZE];
int blueToothBufferPtr;

void initBlueTooth()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinConfigure(GPIO_PB0_U1RX);
    GPIOPinConfigure(GPIO_PB1_U1TX);
    GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 9600, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    UARTFIFODisable(UART1_BASE);
    IntEnable(INT_UART1);
    UARTIntEnable(UART1_BASE, UART_INT_RX);
}

void sendMsgByBT(char *msg)
{
    while(*msg)
    {
        UARTCharPut(UART1_BASE, *msg++);
    }
}
