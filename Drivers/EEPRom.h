#ifndef __EEPROM_H
#define __EEPROM_H

#include "main.h"

#define GRAYTHRESHOLDADDR 0x0000
#define RGBTHRESHOLD 0x0040

struct EEPRomForGray
{
    int high_thre;
    int low_thre;
};

extern struct EEPRomForGray ee_gray;

void initE2PRom(void);

#endif // !__EEPROM_H
