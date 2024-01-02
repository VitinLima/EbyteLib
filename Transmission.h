#ifndef TRANSMISSION_H
#define TRANSMISSION_H

#include "EbyteLib.h"
#include "definitions.h"
#include "modes.h"
#include "configurations.h"
#include "debugging.h"
#include "Utils.h"

#define FIFO_BUFFER_MAX_LENGTH 256

extern bool asyncronousTransmissionFlag;
extern bool fifo_buffer_overflow;
extern uint8_t fifo_buffer[FIFO_BUFFER_MAX_LENGTH];
extern unsigned int fifo_buffer_length;
extern unsigned int fifo_buffer_pointer;
extern unsigned int write_fifo_buffer_pointer;
extern const unsigned int fifo_buffer_max_length;
extern unsigned int payload_max_length;
extern uint8_t fifo_helper_buffer[58];

extern bool hasCRC;
extern uint8_t crcKey[2];
extern uint8_t crcCode[2];

extern bool hasFixedTransmission;
extern uint8_t FT_ADDH_ADDL_CHAN[3];

void asyncronousTransmissionCallback();
void write_to_fifo(uint8_t* buffer, unsigned int size);

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

#endif