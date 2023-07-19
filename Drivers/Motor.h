#ifndef __MOTOR_H
#define __MOTOR_H

#include "main.h"

void initMotor(void);
void turnOffMotor(void);
void turnOnMotor(void);
void setPWM(int left, int right);


#endif // !__MOTOR_H
