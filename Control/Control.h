#ifndef __CONTROL_H
#define __CONTROL_H

#include "main.h"

extern int Track_Bias;

//初始化一个timer进行pwm控制
void initControl();
void Control();
int trackTurn(float bias);
void limitPWM(int input,int max,int min);

#endif // !__CONTROL_H