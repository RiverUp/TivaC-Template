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
	//initMotor();
	initMotor2();
	initJY62();
	initOpenmvTrack();
	initDelayStructs();
	//initHcsr04();
	initEncoder();
	// OLED_I2C_Init();
	// OLED_Init();
	// OLED_Clear();
	initControl();

	// triggerHcsr04();

	// turnOnMotor();
	//  主循环里进行各个事情的轮询
	while (1)
	{
		//		OLED_YX(2, 0);
		//		OLED_Write_String("hello");
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
				// CrossPassDelayFlag.flag = true;
				turnOnMotor2();

				Key1SinglePressedFlag = false;
			}
			// key1双击
			if (Key1DoublePressedFlag)
			{

				Key1DoublePressedFlag = false;
			}
			// key2单击
			if (Key2SinglePressedFlag)
			{
				turnOffMotor2();

				Key2SinglePressedFlag = false;
			}
			// key2双击
			if (Key2DoublePressedFlag)
			{
				sendMsgBySerial("k2dp");

				Key2DoublePressedFlag = false;
			}
		}
		// 接受到一次jy62数据包
		if (AngleReadOnceFlag)
		{
			//			char angleText[40];
			//			sprintf(angleText, "Roll: %d Pitch: %d Yaw: %d\r\n",
			//					(int)Roll, (int)Pitch, (int)Yaw);
			//			sendMsgBySerial(angleText);

			AngleReadOnceFlag = false;
		}
		// 接受到一次循迹openmv数据包
		if (OpenmvTrackReadOnceFlag)
		{
			char trackText[40];
			sprintf(trackText, "trackBias: %d", Track_Bias);
			sendMsgBySerial(trackText);

			OpenmvTrackReadOnceFlag = false;
		}
		// 超声回传了一次
		if (CountDistanceEndFlag)
		{
			char distanceText[40];
			distance = distanceCount * 0.00017 * 2.5;
			sprintf(distanceText, "Distance: %d\r\n", (int)distance);
			sendMsgBySerial(distanceText);
			distanceCount = 0;
			triggerHcsr04();

			CountDistanceEndFlag = false;
		}
	}
}
