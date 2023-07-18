#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H

#include "main.h"

#define BLUETOOTH_BUFFER_SIZE 20

extern char blueToothDataBuffer[];
extern int blueToothBufferPtr;

void initBlueTooth(void) ;
void sendMsgByBT(char *msg);

#endif // !__BLUETH
