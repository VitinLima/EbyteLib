#ifndef EBYTE_LIB_H
#define EBYTE_LIB_H

#include "definitions.h"
#include "modes.h"
#include "Transmission.h"
#include "configurations.h"
#include "debugging.h"
#include "Utils.h"

class EByte32LoRa{
    public:

    private:

};

void initE32();
void waitForAuxReady();
void resetModule();

void auxChangeISR();
void auxRisingISR();
void auxFallingISR();

#endif
