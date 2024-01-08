#ifndef UTILS_H
#define UTILS_H

#include "EbyteLib.h"
// #include "definitions.h"
// #include "Transmission.h"
// #include "modes.h"
// #include "configurations.h"
// #include "debugging.h"

// void flush_e32serial(String msg);
void flush();

bool waitForTimeout(bool (*foo()), unsigned long int timeout);
bool waitForTimeout(bool *foo, unsigned long int timeout);

uint8_t inverse_byte(uint8_t b);

#ifdef DEBUGGING

extern uint8_t ver[];

void printHEAD();
void printADDH();
void printADDL();
void printParity();
void printBaudRate();
void printAirDataRate();
void printChannel();
void printTransmissionMode();
void printIODriveMode();
void printWirelessWakeUpTime();
void printFECSwitch();
void printTransmissionPower();
void printConfiguration();
void getVersionInformation();
void printVersionInformation();

void printTransmissionResult(unsigned long int timeout);

// void parseMessage(String received_message);

#endif

#endif