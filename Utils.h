#ifndef UTILS_H
#define UTILS_H

#include "EbyteLib.h"
#include "definitions.h"
#include "Transmission.h"
#include "modes.h"
#include "configurations.h"
#include "debugging.h"

extern uint8_t ver[];

void flush_e32serial(String msg);

void getVersionInformation();

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

void parseMessage(String received_message);

#endif