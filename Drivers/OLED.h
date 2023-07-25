/*
 * OLED.h
 *
 *  Created on: 2023Äê7ÔÂ23ÈÕ
 *      Author: 71800
 */

#ifndef OLED_H_
#define OLED_H_

#include "main.h"

void OLED_I2C_Init();
void I2CSendSingleByte(uint8_t slave_addr, char data);
void I2CSendMultipleBytes(uint8_t slave_addr, uint8_t numOfBytes, char by[]);

void OLED_Command( uint8_t c);
void  OLED_Data(uint8_t c);
void OLED_Init();
void OLED_YX(unsigned char Row, unsigned char Column);
void OLED_PutChar(char ch );
void OLED_Clear();
void OLED_Write_String( char *s );
void OLED_Write_Integer(uint8_t i);
void OLED_Write_Float(float f);

#endif /* OLED_H_ */
