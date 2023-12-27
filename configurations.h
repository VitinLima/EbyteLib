#ifndef CONFIGURATIONS_H
#define CONFIGURATIONS_H

#include <SoftwareSerial.h>

#include "definitions.h"
#include "modes.h"
#include "EbyteLib.h"

extern const int Rx;
extern const int Tx;

// extern uint8_t param[];

extern SoftwareSerial e32serial;

uint8_t getHEAD();
uint8_t getADDH();
uint8_t getADDL();
long unsigned int getBaudRate();
unsigned int getAirDataRate();
uint8_t getChannel();
FIXED_TRANSMISSION_ENABLING_BIT getTransmissionMode();
IO_DRIVE_MODE getIODriveMode();
WIRELESS_WAKE_UP_TIME getWirelessWakeUpTime();
FEC_SWITCH getFECSwitch();
TRANSMISSION_POWER getTransmissionPower();

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

void setHEAD(HEAD head);
void setADDH(uint8_t addh);
void setADDL(uint8_t addl);
void setParity(UART_PARITY_BIT parity);
void setBaudRate(TTL_UART_BAUD_RATE baudRate);
void setBaudRate(unsigned long baudRate);
void setAirDataRate(AIR_DATA_RATE airDataRate);
void setAirDataRate(unsigned long airDataRate);
void setChannel(uint8_t channel);
void setTransmissionMode(FIXED_TRANSMISSION_ENABLING_BIT transmissionMode);
void setIODriveMode(IO_DRIVE_MODE IODriveMode);
void setWirelessWakeUpTime(WIRELESS_WAKE_UP_TIME wirelessWakeUpTime);
void setFECSwitch(FEC_SWITCH FECSwitch);
void setTransmissionPower(TRANSMISSION_POWER power);
void setTransmissionPower(uint8_t power);

void readConfiguration();
void setConfiguration();
// void setConfiguration(HEAD head, uint8_t CHAN);

#endif
