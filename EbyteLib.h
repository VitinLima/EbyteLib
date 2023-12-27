#ifndef EBYTE_LIB_H
#define EBYTE_LIB_H

#include "definitions.h"
#include "modes.h"
#include "Transmission.h"
#include "configurations.h"
#include "debugging.h"
#include "Utils.h"

extern uint8_t ver[];

class EByte32LoRa{
    public:

    private:

};

void initE32();
void waitForAuxReady();
void resetModule();
void getVersionInformation();

void flush_e32serial(String msg);

void auxChangeISR();
void auxRisingISR();
void auxFallingISR();

#endif
