#ifndef __FLAG_H
#define __FLAG_H

#include "stdbool.h"

extern bool SerialCompleteFlag;
extern bool BluetoothCompleteFlag;
extern bool Key1SinglePressedFlag, Key1DoublePressedFlag, Key2SinglePressedFlag, Key2DoublePressedFlag;
extern bool AngleReadOnceFlag;
extern bool OpenmvTrackReadOnceFlag;

#endif // !__FLAG_H
