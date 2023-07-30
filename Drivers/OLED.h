#ifndef __OLED_H
#define __OLED_H

#include "main.h"

void initOled(void);
void I2CSendSingleByte(uint8_t slave_addr, char data);
void sendI2CMultipleBytes(uint8_t slave_addr, uint8_t numOfBytes, char by[]);

void sendOledCommand( uint8_t c);
void  OLED_Data(uint8_t c);
void OLED_Init();
void setOledCursor(unsigned char Row, unsigned char Column);
void putCharOnOled(char ch );
void clearOled();
void showStringOnOled( char *s );
void OLED_Write_Integer(uint8_t i);
void OLED_Write_Float(float f);

#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define slaveAddress 0x3C
#define OLED_SETCONTRAST 0x81
#define OLED_DISPLAYALLON_RESUME 0xA4
#define OLED_DISPLAYALLON 0xA5
#define OLED_NORMALDISPLAY 0xA6
#define OLED_INVERTDISPLAY 0xA7
#define OLED_DISPLAYOFF 0xAE
#define OLED_DISPLAYON 0xAF
#define OLED_SETDISPLAYOFFSET 0xD3
#define OLED_SETCOMPINS 0xDA
#define OLED_SETVCOMDETECT 0xDB
#define OLED_SETDISPLAYCLOCKDIV 0xD5
#define OLED_SETPRECHARGE 0xD9
#define OLED_SETMULTIPLEX 0xA8
#define OLED_SETLOWCOLUMN 0x00
#define OLED_SETHIGHCOLUMN 0x10
#define OLED_SETSTARTLINE 0x40
#define OLED_MEMORYMODE 0x20
#define OLED_COLUMNADDR 0x21
#define OLED_PAGEADDR 0x22
#define OLED_COMSCANINC 0xC0
#define OLED_COMSCANDEC 0xC8
#define OLED_SEGREMAP 0xA0
#define OLED_CHARGEPUMP 0x8D

#endif /* OLED_H_ */