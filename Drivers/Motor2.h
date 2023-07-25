#ifndef __MOTOR2_H
#define __MOTOR2_H

#include "main.h"

void initMotor2();
void turnOffMotor2(void);
void turnOnMotor2(void);
void setPWM2(int left, int right);

#endif // !__MOTOR2_H