#include "main.h"

#include "string.h"
#include "stdlib.h"
#include "stdio.h"

#include "Serial.h"
#include "Flag.h"



/**
 * main.c
 */
int main(void)
{

	MAP_FPUEnable();
	MAP_FPULazyStackingEnable();

	// 16mhz
	SysCtlClockSet( SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

	IntMasterEnable();

	initSerial();

	// 配置小灯用于测试
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);

	while (1)
	{
		// 处理串口指令
		if (SerialCompleteFlag)
		{
			if (strcmp(serialDataBuffer, "on") == 0)
			{
				 if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_3))
				 	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
				 else
				 	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
			}
			SerialCompleteFlag = false;
			sendMsgBySerial(serialDataBuffer);
			memset(serialDataBuffer, 0, SERIAL_BUFFER_SIZE);
			serialBufferPtr = 0;
		}
	}
}
