#ifndef TRANSMISSION_NO_BUFFER_H
#define TRANSMISSION_NO_BUFFER_H

#include "EbyteLib.h"
// #include "definitions.h"
// #include "modes.h"
// #include "configurations.h"
// #include "debugging.h"
// #include "Utils.h"

#ifdef UNBUFFERED_COMPILE

#pragma message("Compiling transmission without buffer")

extern bool asyncronousTransmissionFlag;
extern uint8_t* transmitting_buffer;
extern unsigned int fifo_buffer_length;
extern unsigned int fifo_buffer_pointer;
extern unsigned int write_fifo_buffer_pointer;
extern unsigned int payload_max_length;

extern bool hasCRC;
extern uint8_t crcKey[2];
extern uint8_t crcCode[2];

extern bool hasFixedTransmission;
extern uint8_t FT_ADDH_ADDL_CHAN[3];

void asyncronousTransmissionCallback();
void set_fifo(uint8_t* buffer, unsigned int size);

void print(const char* message);
void printFixedTransmission(uint8_t ADDH, uint8_t ADDL, uint8_t CHAN, const char* message);

void write(uint8_t* buffer, unsigned int size);
void write(uint8_t byte);
void writeFixedTransmission(uint8_t ADDH, uint8_t ADDL, uint8_t CHAN, uint8_t* buffer, unsigned int size);
void writeFixedTransmission(uint8_t ADDH, uint8_t ADDL, uint8_t CHAN, uint8_t byte);

extern void (*transmitting_function)(uint8_t *buffer, unsigned int size);
void transmit(uint8_t *buffer, unsigned int size);
void FTtransmit(uint8_t *buffer, unsigned int size);
void transmitCRC(uint8_t *buffer, unsigned int size);
void FTtransmitCRC(uint8_t *buffer, unsigned int size);

void asynchronousWrite(uint8_t* buffer, unsigned int size);
// void asynchronousWrite(uint8_t byte);
void asynchronousWriteFixedTransmission(uint8_t ADDH, uint8_t ADDL, uint8_t CHAN, uint8_t* buffer, unsigned int size);
// void asynchronousWriteFixedTransmission(uint8_t ADDH, uint8_t ADDL, uint8_t CHAN, uint8_t byte);

bool read(uint8_t* buffer, unsigned int size);
bool read(uint8_t* buffer, unsigned int size, unsigned long int timeout);
bool getTransmissionResult(unsigned long int timeout);

bool computeCRC(uint8_t *buffer, unsigned int size);

unsigned long int estimateTimeOfTransmission(unsigned long int size);

#endif

#endif