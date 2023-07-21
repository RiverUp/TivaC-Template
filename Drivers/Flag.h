#ifndef __FLAG_H
#define __FLAG_H

#include "stdbool.h"
#include "main.h"

extern bool SerialCompleteFlag;
extern bool BluetoothCompleteFlag;
extern bool Key1SinglePressedFlag, Key1DoublePressedFlag, Key2SinglePressedFlag, Key2DoublePressedFlag;
extern bool AngleReadOnceFlag;
extern bool OpenmvTrackReadOnceFlag;
extern bool RotateLeftFlag, RotateRightFlag;
extern struct DelayStruct CrossPassDelayFlag;

void countDelay(struct DelayStruct delayStruct);
void createDelayStruct(struct DelayStruct delayStruct, char *name, int times);
void initDelayStructs();
#endif // !__FLAG_H
