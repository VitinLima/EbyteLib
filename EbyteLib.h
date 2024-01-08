#ifndef EBYTE_LIB_H
#define EBYTE_LIB_H

#define UNBUFFERED_COMPILE

#include "definitions.h"
#include "modes.h"
#ifdef UNBUFFERED_COMPILE
#include "Transmission_no_buffer.h"
#else
#include "Transmission.h"
#endif
#include "configurations.h"
#include "debugging.h"
#include "Utils.h"

extern bool receivingFlag;
extern bool writing_to_device;
extern bool transmission_started;
extern bool transmission_finished;

extern bool reception_started;
extern bool reception_finished;

extern void (*receiveCallback)();
void defaultReceiveCallback();

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
