#ifndef EBYTE_LIB_H
#define EBYTE_LIB_H

#include "definitions.h"
#include "modes.h"
#include "configurations.h"
#include "debugging.h"

extern uint8_t ver[];

class EByte32LoRa{
    public:

    private:

};

void initE32();
void waitForAuxReady();
void resetModule();
void getVersionInformation();
void print(const char* message);
void write(uint8_t* buffer, unsigned int size);
void write(uint8_t byte);
void writeFixedTransmission(uint8_t ADDH, uint8_t ADDL, uint8_t CHAN, uint8_t* buffer, unsigned int size);
void writeFixedTransmission(uint8_t ADDH, uint8_t ADDL, uint8_t CHAN, uint8_t byte);
void read(uint8_t* buffer, unsigned int size);
const char* getTransmissionResult();
bool transmissionSuccess();
void parseMessage(String received_message);

void flush_e32serial(String msg);

void auxChangeISR();
void auxRisingISR();
void auxFallingISR();

#endif
