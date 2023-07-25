#ifndef __CONTROL_H
#define __CONTROL_H

#include "main.h"

extern int Track_Bias;
extern float Basic_Velocity;
extern int CrossNum;

// 初始化一个timer进行pwm控制
void initControl();
void Control();
int trackTurn(float bias);
int limitPWM(int input, int max, int min);
void passCross();
int rotateTurn();
void setRotateTarget(int angle, bool direction, int current_yaw);
int velocity(int encoderLeft, int encoderRight);

#endif // !__CONTROL_H