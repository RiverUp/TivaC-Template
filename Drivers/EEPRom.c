#include "EEPRom.h"

struct EEPRomForGray ee_gray = {
    30, 20};

void initE2PRom()
{
    int returnCode;
    SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);
    do
    {
        returnCode = EEPROMInit(); // EEPROM start
    } while (returnCode != EEPROM_INIT_OK);
    EEPROMRead((uint32_t *)&ee_gray, GRAYTHRESHOLDADDR, sizeof(ee_gray));
}
