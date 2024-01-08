#ifndef MODES_H
#define MODES_H

#include "EbyteLib.h"
// #include "definitions.h"
// #include "configurations.h"

extern void setSleepMode();
extern void setNormalMode();
extern void setWakeUpMode();
extern void setPowerSavingMode();

OperationMode getOperationMode();
void setOperationMode(OperationMode op);

#endif
