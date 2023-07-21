#include "Flag.h"

bool SerialCompleteFlag;
bool BluetoothCompleteFlag;
bool Key1SinglePressedFlag;
bool Key1DoublePressedFlag;
bool Key2SinglePressedFlag;
bool Key2DoublePressedFlag;
bool AngleReadOnceFlag;
bool OpenmvTrackReadOnceFlag;
bool RotateLeftFlag, RotateRightFlag;

struct DelayStruct CrossPassDelayFlag;

void initDelayStructs()
{
    createDelayStruct(CrossPassDelayFlag, "crosspassDelay", 100);
}

void countDelay(struct DelayStruct delayStruct)
{
    if (delayStruct.flag)
    {
        delayStruct.count++;
        if (delayStruct.count >= delayStruct.times)
        {
            delayStruct.flag = false;
            delayStruct.trigger = true;
        }
    }
}

void createDelayStruct(struct DelayStruct delayStruct, char *name, int times)
{
    delayStruct.count = 0;
    delayStruct.times = times;
    delayStruct.flag = false;
    delayStruct.name = name;
    delayStruct.trigger = false;
}
