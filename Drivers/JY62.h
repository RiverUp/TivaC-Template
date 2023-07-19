#ifndef __JY62_H
#define __JY62_H

#include "main.h"

extern float Roll,Pitch,Yaw;

void initJY62();
void readAngle();
void sendJY62Command(int *command);



#endif // !__JY62_H
