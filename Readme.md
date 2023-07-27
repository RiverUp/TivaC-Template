# Tiva C 123系列

## 项目结构

Root->

    |Control：电机控制逻辑

    |Drivers：硬件底层驱动、中断回调、各种标志

    |Driverlib：tivasdk

`main()`的while主循环中进行轮询

## 中断

在Drivers路径下的Interrupt文件中

1. Timer0A：逻辑控制
2. Timer1A：按键检测
3. Timer2A：超声波触发
4. UART0：电脑串口
5. UART1：蓝牙串口
6. UART2：循迹的openmv串口
7. UART5：jy62角度传感器串口
8. GPIOA：PA7：超声回声；PA4，PA5：减速电机编码器读取
9. GPIOE：PE2，PE3：减速电机编码器读取
10. ADCSeq3：驱动板电压读取

## 硬件连接

### TB6612(减速电机驱动)

| TB6612 | TivaC123 |
| ------ | -------- |
| E2A    | PE2      |
| E2B    | PE3      |
| E1A    | PA5      |
| E1B    | PA4      |
| ADC    | PE1      |
| PWMB   | PC5      |
| BIN2   | PB7      |
| BIN1   | PB6      |
| STBY   | PA2      |
| AIN1   | PD2      |
| AIN2   | PD3      |
| PWMA   | PC4      |

### HCSR04(超声波)

| HCSR04  | TivaC123 |
| ------- | -------- |
| Trigger | PA6      |
| Echo    | PA7      |

### Sg90（舵机）

| Sg90 | TivaC123 |
| ---- | -------- |
| PWM  | PD1      |

### JY62（角度传感器）

| JY62 | TivaC123 |
| ---- | -------- |
| RX   | PE5      |
| TX   | PE4      |

### OLED

| OLED | TivaC123 |
| ---- | -------- |
| SCL  | PD0      |
| SDA  | PD1      |

### Openmv（循迹）

| Openmv | TivaC123 |
| ------ | -------- |
| RX     | PD7      |
| TX     | PD6      |

### D302A（步进电机驱动）

| D302A | TivaC123 |
| ----- | -------- |
| 输出  | PC4，PB6 |
| 使能  | PA2      |
| 方向  | PD2，PD3 |
|       |          |
|       |          |
