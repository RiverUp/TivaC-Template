#include "main.h"
// 电脑串口USB
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

//蓝牙串口
void UART1IntHandler(void)
{
    uint32_t u32IntStatus = UARTIntStatus(UART1_BASE, true);
    UARTIntClear(UART1_BASE, u32IntStatus);
    if (u32IntStatus & UART_INT_RX)
    {
        char dat = UARTCharGet(UART1_BASE);
        if (!(dat == 'x' || blueToothBufferPtr > BLUETOOTH_BUFFER_SIZE))
        {
            blueToothDataBuffer[blueToothBufferPtr++] = (char)dat;
        }
        else
        {
            BluetoothCompleteFlag = true;
        }
    }
}

// 循迹Openmv串口
uint8_t Cx=0,Cy=0,Cw=0,Ch=0,Ci=0;
void UART2IntHandler(void)
{
    uint32_t u32IntStatus = UARTIntStatus(UART1_BASE, true);
    UARTIntClear(UART1_BASE, u32IntStatus);

    uint8_t com_data;
    uint8_t i;
    static uint8_t RxCounter1 = 0;
    static uint16_t RxBuffer1[12] = {0};
    static uint8_t RxState = 0;
    static uint8_t RxFlag1 = 0;

    if (u32IntStatus & UART_INT_RX)
    {
        com_data = UARTCharGet(UART2_BASE);
        if (RxState == 0 && com_data == 0x2C) // 0x2c帧头
        {

            RxState = 1;
            RxBuffer1[RxCounter1++] = com_data;
        }

        else if (RxState == 1 && com_data == 0x12) // 0x12帧头
        {
            RxState = 2;
            RxBuffer1[RxCounter1++] = com_data;
        }

        else if (RxState == 2)
        {
            RxBuffer1[RxCounter1++] = com_data;

            if (RxCounter1 >= 11 || com_data == 0x5B) // RxBuffer1接受满了,接收数据结束
            {
                RxState = 3;
                RxFlag1 = 1;

                Cx = RxBuffer1[RxCounter1 - 6];
                Cy = RxBuffer1[RxCounter1 - 5];
                Cw = RxBuffer1[RxCounter1 - 4];
                Ch = RxBuffer1[RxCounter1 - 3];
                Ci = RxBuffer1[RxCounter1 - 2];
            }
        }
        else if (RxState == 3) // 检测是否接受到结束标志
        {
            if (RxBuffer1[RxCounter1 - 1] == 0x5B)
            {
                // USART_ITConfig(USART2, USART_IT_RXNE, DISABLE); // 关闭DTSABLE中断
                if (RxFlag1)
                {

                    // if (Cw)
                    //     Track_Bias = Cx;
                    // else
                    //     Track_Bias = -Cx;
                    // if (myabs(Track_Bias) > 200)
                    // {
                    //     Track_Bias = 0;
                    // }
                    // Ci检测是否为路口,计数经过了多少个路口
                }
                RxFlag1 = 0;
                RxCounter1 = 0;
                RxState = 0;
                // USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
            }
            else // 接收错误
            {
                RxState = 0;
                RxCounter1 = 0;
                for (i = 0; i < 10; i++)
                {
                    RxBuffer1[i] = 0x00; // 将存放数据数组清零
                }
            }
        }

        else // 接收异常
        {
            RxState = 0;
            RxCounter1 = 0;
            for (i = 0; i < 10; i++)
            {
                RxBuffer1[i] = 0x00; // 将存放数据数组清零
            }
        }
    }
}
