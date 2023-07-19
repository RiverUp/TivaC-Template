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

	// 16mhz
	// SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
	// 			   SYSCTL_XTAL_16MHZ);
	// 40mhz
	SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

	IntMasterEnable();

	// 配置小灯用于测试
	initLights();

	initSerial();
	initBlueTooth();
	initKeys();


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
			if (!strcmp(blueToothDataBuffer, "on"))
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
		// 按键
		{
			// key1单击
			if (Key1SinglePressedFlag)
			{
				sendMsgBySerial("k1sp");

				Key1SinglePressedFlag = false;
			}
			// key1双击
			if (Key1DoublePressedFlag)
			{
				sendMsgBySerial("k1dp");

				Key1DoublePressedFlag = false;
			}
			// key2单击
			if (Key2SinglePressedFlag)
			{
				sendMsgBySerial("k2sp");

				Key2SinglePressedFlag = false;
			}
			// key2双击
			if (Key2DoublePressedFlag)
			{
				sendMsgBySerial("k2dp");

				Key2DoublePressedFlag = false;
			}
		}

		
	}
}
