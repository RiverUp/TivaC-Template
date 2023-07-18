#include "main.h"

#include "Serial.h"
#include "Flag.h"

void UART0IntHandler(void)
{
    uint32_t u32IntStatus = UARTIntStatus(UART0_BASE, true);
    UARTIntClear(UART0_BASE, u32IntStatus);
    if (u32IntStatus & UART_INT_RX)
    {
        char dat = UARTCharGet(UART0_BASE);
        if (!(dat == 'x' || serialBufferPtr > SERIAL_BUFFER_SIZE))
        {
            serialDataBuffer[serialBufferPtr++] = (char)dat;
        }
        else
        {
            SerialCompleteFlag = true;
        }
    }
}

