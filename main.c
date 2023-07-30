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

	initOled();
	clearOled();

	//	 OLED_Init();
	//	 OLED_Clear();
	initSerial();
	initBlueTooth();
	initKeys();
	initMotor();
	// initMotor2();
	// initEncoder();
	initJY62();
	initOpenmvTrack();
	initDelayStructs();
	// initHcsr04();
	initControl();
	// initBattery();
	// initSg90();
	initBeep();

	// triggerHcsr04();

	// rotateSg90(0);
	//   主循环里进行各个事情的轮询
	while (1)
	{

		setOledCursor(2, 0);
		showStringOnOled("hello");
		// 处理电脑串口指令
		// ADCProcessorTrigger(ADC0_BASE, 3);
		if (SerialCompleteFlag)
		{
			if (!strcmp(serialDataBuffer, "on"))
			{
				if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_3))
					turnOffLights(Green);
				else
					turnOnLights(Green);
			}
			// 控制舵机旋转
			if (serialDataBuffer[0] == 'r')
			{
				int a, b, c;
				a = serialDataBuffer[1] - '0';
				b = serialDataBuffer[2] - '0';
				c = serialDataBuffer[3] - '0';
				int angle = 100 * a + 10 * b + c;
				rotateSg90(angle);
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
			// 控制小车旋转
			if (blueToothDataBuffer[0] == 'r')
			{
				int a, b, c;
				a = blueToothDataBuffer[2] - '0';
				b = blueToothDataBuffer[3] - '0';
				c = blueToothDataBuffer[4] - '0';
				int angle = 100 * a + 10 * b + c;
				if (blueToothDataBuffer[1] == 'l')
					setRotateTarget(angle, LEFT, Yaw);
				else if (blueToothDataBuffer[1] == 'r')
					setRotateTarget(angle, RIGHT, Yaw);
			}
			// 启动小车
			if (!strcmp(blueToothDataBuffer, "start"))
				turnOnMotor();
			// 停止小车
			if (!strcmp(blueToothDataBuffer, "stop"))
				turnOffMotor();

			if (blueToothDataBuffer[0] == 'v')
			{
				int a, b, c, d;
				a = blueToothDataBuffer[2] - '0';
				b = blueToothDataBuffer[3] - '0';
				c = blueToothDataBuffer[4] - '0';
				d = blueToothDataBuffer[5] - '0';
				int speed = 1000 * a + 100 * b + 10 * c + d;
				if (blueToothDataBuffer[1] == 'n')
					speed = -speed;
				Basic_Velocity = speed;
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
				turnOnMotor();

				Key1SinglePressedFlag = false;
			}
			// key1双击
			if (Key1DoublePressedFlag)
			{
				sendMsgBySerial("k1dp");
				ring(1000);

				Key1DoublePressedFlag = false;
			}
			// key2单击
			if (Key2SinglePressedFlag)
			{
				turnOffMotor();

				Key2SinglePressedFlag = false;
			}
			// key2双击
			if (Key2DoublePressedFlag)
			{
				setOledCursor(1, 0);
				showStringOnOled("k2dp");

				Key2DoublePressedFlag = false;
			}
		}
		// 接受到一次jy62数据包
		if (AngleReadOnceFlag)
		{
			char angleText[40];
			sprintf(angleText, "Roll: %d Pitch: %d Yaw: %d\r\n",
					(int)Roll, (int)Pitch, (int)Yaw);
			sendMsgBySerial(angleText);

			AngleReadOnceFlag = false;
		}
		// 接受到一次循迹openmv数据包
		if (OpenmvTrackReadOnceFlag)
		{
			char trackText[40];
			sprintf(trackText, "trackBias: %d", Track_Bias);
			setOledCursor(0,0);
			showStringOnOled(trackText);
			sendMsgByBT(trackText);

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
