#ifndef __ENCODER_H
#define __ENCODER_H

#include "main.h"

extern int EncoderRight, EncoderLeft;

void initEncoder();
int readEncoder(uint8_t side);



#endif // !__ENCODER_H