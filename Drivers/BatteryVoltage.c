#include "BatteryVoltage.h"

uint32_t pui32ADC0Value[1];
float Voltage;

// PE1
void initBattery()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_1);
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0,
                             ADC_CTL_CH2 | ADC_CTL_IE | ADC_CTL_END);
    ADCIntEnable(ADC0_BASE, 3);
    IntEnable(INT_ADC0SS3);
    ADCSequenceEnable(ADC0_BASE, 3);
    ADCIntClear(ADC0_BASE, 3);
}
float getBatteryVoltage(void)
{
    ADCSequenceDataGet(ADC0_BASE, 3, pui32ADC0Value);
    float voltage = 1.0*pui32ADC0Value[0] / 4096 * 3.3 *18*100;
    return voltage;
}
