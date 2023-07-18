#include "main.h"

#include "string.h"
#include "stdlib.h"
#include "stdio.h"


/**
 * main.c
 */
int main(void)
{

	MAP_FPUEnable();
	MAP_FPULazyStackingEnable();

	// 40mhz
	SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

	IntMasterEnable();

	initSerial();
	initBlueTooth();
	// 配置小灯用于测试
	initLights();
	

	// 主循环里进行各个事情的轮询
	while (1)
	{
		// 处理电脑串口指令
		if (SerialCompleteFlag)
		{
			if (!strcmp(serialDataBuffer, "on"))
			{
				if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_3))
					turnOffLights(Green);
				else
					turnOnLights(Green);
			}
			sendMsgBySerial(serialDataBuffer);

			SerialCompleteFlag = false;
			memset(serialDataBuffer, 0, SERIAL_BUFFER_SIZE);
			serialBufferPtr = 0;
		}
		// 处理蓝牙指令
		if (BluetoothCompleteFlag)
		{
			// 业务逻辑
			if(!strcmp(blueToothDataBuffer,"on"))
			{
				if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2))
					turnOffLights(Blue);
				else
					turnOnLights(Blue);
			}
			sendMsgByBT(blueToothDataBuffer);
			
			BluetoothCompleteFlag = false;
			memset(blueToothDataBuffer, 0, BLUETOOTH_BUFFER_SIZE);
			blueToothBufferPtr = 0;
		}
	}
}
