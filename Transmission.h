#ifndef TRANSMISSION_H
#define TRANSMISSION_H

#include "EbyteLib.h"
#include "definitions.h"
#include "modes.h"
#include "configurations.h"
#include "debugging.h"
#include "Utils.h"

void print(const char* message);
void write(uint8_t* buffer, unsigned int size);
void write(uint8_t byte);
void writeFixedTransmission(uint8_t ADDH, uint8_t ADDL, uint8_t CHAN, uint8_t* buffer, unsigned int size);
void writeFixedTransmission(uint8_t ADDH, uint8_t ADDL, uint8_t CHAN, uint8_t byte);
void read(uint8_t* buffer, unsigned int size);
const char* getTransmissionResult();
bool transmissionSuccess();

#endif