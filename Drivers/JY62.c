#include "JY62.h"

// Z轴角度归零
int command1[3] = {0xff, 0xaa, 0x52};
// 加速度计校准
int command2[3] = {0xff, 0xaa, 0x67};
float Roll,Pitch,Yaw;


void initJY62()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinConfigure(GPIO_PE4_U5RX);
    GPIOPinConfigure(GPIO_PE5_U5TX);
    GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    UARTConfigSetExpClk(UART5_BASE, SysCtlClockGet(),
                        115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    UARTFIFODisable(UART5_BASE);
    IntEnable(INT_UART5);
    UARTIntEnable(UART5_BASE, UART_INT_RX);

    //初始化校准
    sendJY62Command(command1);
    sendJY62Command(command2);

}

void sendJY62Command(int *command)
{
    while (*command)
    {
        UARTCharPut(UART5_BASE,*command++);
    }
}
