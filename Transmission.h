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
extern const uint8_t fifo_buffer[FIFO_BUFFER_MAX_LENGTH];
extern uint8_t fifo_buffer_length;
extern uint8_t fifo_buffer_pointer;
extern uint8_t write_fifo_buffer_pointer;
extern const uint8_t fifo_buffer_max_length;
extern const uint8_t fifo_helper_buffer[58];

void asyncronousTransmissionCallback();
void write_to_fifo();

void print(const char* message);
void printFixedTransmission(uint8_t ADDH, uint8_t ADDL, uint8_t CHAN, const char* message);

void write(uint8_t* buffer, unsigned int size);
void write(uint8_t byte);
void writeFixedTransmission(uint8_t ADDH, uint8_t ADDL, uint8_t CHAN, uint8_t* buffer, unsigned int size);
void writeFixedTransmission(uint8_t ADDH, uint8_t ADDL, uint8_t CHAN, uint8_t byte);
void asyncronousWrite(uint8_t* buffer, unsigned int size);
void asyncronousWrite(uint8_t byte);
void asyncronouswriteFixedTransmission(uint8_t ADDH, uint8_t ADDL, uint8_t CHAN, uint8_t* buffer, unsigned int size);
void asyncronouswriteFixedTransmission(uint8_t ADDH, uint8_t ADDL, uint8_t CHAN, uint8_t byte);

void read(uint8_t* buffer, unsigned int size);

const char* getTransmissionResult();
bool transmissionSuccess();

#endif