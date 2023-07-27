#include "Key.h"

#define KEY1 GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)
#define KEY2 GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)

void initKeys()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    // 解锁PF0
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) = 0xFF;
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4,
                     GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    // 定时器timer1中断
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
    // 5ms
    TimerLoadSet(TIMER1_BASE, TIMER_A, SysCtlClockGet() / 200);
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    IntEnable(INT_TIMER1A);
    TimerEnable(TIMER1_BASE, TIMER_A);
}

int key1Pressed(int time)
{
    static uint16_t flag_key = 0, count_key = 0, double_key = 0;
    static uint16_t count_single = 0, Forever_count = 0;
    // 长按标志位未置1
    if (KEY1 == 0)
        Forever_count++;
    else
        Forever_count = 0;
    // 按键按下，且按键按下标志为0
    if (KEY1 == 0 && flag_key == 0)
        flag_key = 1;
    // 第一次为0
    if (count_key == 0)
    {
        // 按键按下一次，double_key加一次，count=1
        if (flag_key == 1)
        {
            double_key++;
            count_key = 1;
        }
        // 双击执行的指令
        if (double_key == 2)
        {
            double_key = 0;
            count_single = 0;
            return 2;
        }
    }
    // 按键未按下
    if (KEY1)
        flag_key = 0, count_key = 0;
    // 按键已经按下一次
    if (double_key == 1)
    {
        // 超过等待时间
        count_single++;
        // 单击执行的指令
        if (count_single > time && Forever_count < time)
        {
            double_key = 0;
            count_single = 0;
            return 1;
        }
        if (Forever_count > time)
        {
            double_key = 0;
            count_single = 0;
        }
    }
    return 0;
}

int key2Pressed(int time)
{
    static uint16_t flag_key = 0, count_key = 0, double_key = 0;
    static uint16_t count_single = 0, Forever_count = 0;
    // 长按标志位未置1
    if (KEY2 == 0)
        Forever_count++;
    else
        Forever_count = 0;
    // 按键按下，且按键按下标志为0
    if (KEY2 == 0 && flag_key == 0)
        flag_key = 1;
    // 第一次为0
    if (count_key == 0)
    {
        // 按键按下一次，double_key加一次，count=1
        if (flag_key == 1)
        {
            double_key++;
            count_key = 1;
        }
        // 双击执行的指令
        if (double_key == 2)
        {
            double_key = 0;
            count_single = 0;
            return 2;
        }
    }
    // 按键未按下
    if (KEY2)
        flag_key = 0, count_key = 0;
    // 按键已经按下一次
    if (double_key == 1)
    {
        // 超过等待时间
        count_single++;
        // 单击执行的指令
        if (count_single > time && Forever_count < time)
        {
            double_key = 0;
            count_single = 0;
            return 1;
        }
        if (Forever_count > time)
        {
            double_key = 0;
            count_single = 0;
        }
    }
    return 0;
}
