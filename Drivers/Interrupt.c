#include "main.h"
#include "math.h"
#include "DelayStruct.h"

// 控制用的定时器
void Timer0AIntHandler(void)
{
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    Control();
}

// 按键检测的中断
void Timer1AIntHandler(void)
{
    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

    int temp1 = key1Pressed(50);
    int temp2 = key2Pressed(50);
    if (temp1 == 1)
    {
        Key1SinglePressedFlag = true;
    }
    else if (temp1 == 2)
    {
        Key1DoublePressedFlag = true;
    }
    if (temp2 == 1)
    {
        Key2SinglePressedFlag = true;
    }
    else if (temp2 == 2)
    {
        Key2DoublePressedFlag = true;
    }
}

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

// 蓝牙串口
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
uint8_t Cx = 0, Cy = 0, Ci = 0;
void UART2IntHandler(void)
{
    uint32_t u32IntStatus = UARTIntStatus(UART2_BASE, true);
    UARTIntClear(UART2_BASE, u32IntStatus);

    uint8_t com_data;
    uint8_t i;
    static uint8_t RxCounter1 = 0;
    static uint16_t RxBuffer1[6] = {0};
    static uint8_t RxState = 0;
    static uint8_t RxFlag1 = 0;

    if (u32IntStatus & UART_INT_RX)
    {
        com_data = UARTCharGet(UART2_BASE);
        if (RxState == 0 && com_data == 0x2C) // 0x2c帧头
        {
            // sendMsgBySerial("openmv");
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

            if (RxCounter1 >= 6 || com_data == 0x5B) // RxBuffer1接受满了,接收数据结束
            {
                RxState = 3;
                RxFlag1 = 1;

                Cx = RxBuffer1[RxCounter1 - 4];
                Cy = RxBuffer1[RxCounter1 - 3];
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
                    if (Cy)
                        Track_Bias = Cx;
                    else
                        Track_Bias = -Cx;
                    if (abs(Track_Bias) > 200)
                    {
                        Track_Bias = 0;
                    }
                    if (Ci)
                    {
                        CrossPassDelayFlag.flag = true;
                    }
                    OpenmvTrackReadOnceFlag = true;
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
                for (i = 0; i < 6; i++)
                {
                    RxBuffer1[i] = 0x00; // 将存放数据数组清零
                }
            }
        }

        else // 接收异常
        {
            RxState = 0;
            RxCounter1 = 0;
            for (i = 0; i < 6; i++)
            {
                RxBuffer1[i] = 0x00; // 将存放数据数组清零
            }
        }
    }
}

// jy62
uint8_t RollH, RollL, PitchH, PitchL, YawH, YawL, VH, VL, CheckBit, Sum;
void UART5IntHandler(void)
{
    uint8_t com_data;
    uint8_t i;
    static uint8_t RxCounter1 = 0;
    static uint16_t RxBuffer1[11] = {0};
    static uint8_t RxState = 0;
    static uint8_t RxFlag1 = 0;
    uint32_t u32IntStatus = UARTIntStatus(UART5_BASE, true);
    UARTIntClear(UART5_BASE, u32IntStatus);

    if (u32IntStatus & UART_INT_RX)
    {

        com_data = UARTCharGet(UART5_BASE);
        if (RxState == 0 && com_data == 0x55)
        {
            RxState = 1;
            RxBuffer1[RxCounter1++] = com_data;
        }
        else if (RxState == 1 && com_data == 0x53)
        {
            RxState = 2;
            RxBuffer1[RxCounter1++] = com_data;
        }
        else if (RxState == 2)
        {
            RxBuffer1[RxCounter1++] = com_data;
            if (RxCounter1 >= 11)
            {
                RxState = 3;
                RxFlag1 = 1;
                // 取值
                RollL = RxBuffer1[RxCounter1 - 9];
                RollH = RxBuffer1[RxCounter1 - 8];
                PitchL = RxBuffer1[RxCounter1 - 7];
                PitchH = RxBuffer1[RxCounter1 - 6];
                YawL = RxBuffer1[RxCounter1 - 5];
                YawH = RxBuffer1[RxCounter1 - 4];
                VL = RxBuffer1[RxCounter1 - 3];
                VH = RxBuffer1[RxCounter1 - 2];
                CheckBit = RxBuffer1[RxCounter1 - 1];
                Sum = 0x55 + 0x53 + RollH + RollL + PitchH + PitchL + YawL + YawH + VL + VH;
            }
        }
        else if (RxState == 3)
        {
            if (CheckBit == Sum && !AngleReadOnceFlag)
            {

                if (RxFlag1)
                {
                    // 业务逻辑，取值的处理
                    short data1, data2, data3;
                    data1 = (short)(((short)RollH << 8) | RollL);
                    data2 = (short)(((short)PitchH << 8) | PitchL);
                    data3 = (short)(((short)YawH << 8) | YawL);

                    Roll = 1.0 * data1 / 32768 * 180;
                    Pitch = 1.0 * data2 / 32768 * 180;
                    Yaw = 1.0 * data3 / 32768 * 180;

                    AngleReadOnceFlag = true;
                }
                RxFlag1 = 0;
                RxCounter1 = 0;
                RxState = 0;
                //                IntEnable(UART5_BASE);
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
