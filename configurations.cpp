#include "Arduino.h"
#include "configurations.h"

const int Rx = 9;
const int Tx = 10;

SoftwareSerial e32serial(Rx, Tx);

uint8_t getHEAD(){
  return configuration.parameters.HEAD;
}

uint8_t getADDH(){
  return configuration.parameters.ADDH;
}

uint8_t getADDL(){
  return configuration.parameters.ADDL;
}

long unsigned int getBaudRate(){
  long unsigned int baudRate;
  // uint8_t b = ;//(param[2]&0x38)>>3
  switch(configuration.parameters.SPED.bits.TTL_UART_baud_rate){
    case TTL_UART_baud_rate_1200:
      baudRate = 1200;
      break;
    case TTL_UART_baud_rate_2400:
      baudRate = 2400;
      break;
    case TTL_UART_baud_rate_4800:
      baudRate = 4800;
      break;
    case TTL_UART_baud_rate_9600:
      baudRate = 9600;
      break;
    case TTL_UART_baud_rate_19200:
      baudRate = 19200;
      break;
    case TTL_UART_baud_rate_38400:
      baudRate = 38400;
      break;
    case TTL_UART_baud_rate_57600:
      baudRate = 57600;
      break;
    case TTL_UART_baud_rate_115200:
      baudRate = 115200;
      break;
  }
  return baudRate;
}

unsigned int getAirDataRate(){
  int airDataRate;
  // (param[2]&0x07)
  switch(configuration.parameters.SPED.bits.Air_data_rate){
    case Air_Data_Rate_300:
      airDataRate = 300;
      break;
    case Air_Data_Rate_1200:
      airDataRate = 1200;
      break;
    case Air_Data_Rate_2400:
      airDataRate = 2400;
      break;
    case Air_Data_Rate_4800:
      airDataRate = 4800;
      break;
    case Air_Data_Rate_9600:
      airDataRate = 9600;
      break;
    case Air_Data_Rate_19200:
      airDataRate = 19200;
      break;
    default:
      airDataRate = 19200;
      break;
  }
  return airDataRate;
}

uint8_t getChannel(){
  return configuration.parameters.CHAN;
}

FIXED_TRANSMISSION_ENABLING_BIT getTransmissionMode(){
  FIXED_TRANSMISSION_ENABLING_BIT transmissionMode;
  switch(configuration.parameters.OPTION.bits.FTEB){
    case TRANSPARENT_TRANSMISSION_MODE:
      transmissionMode = TRANSPARENT_TRANSMISSION_MODE;
      break;
    case FIXED_TRANSMISSION_MODE:
      transmissionMode = FIXED_TRANSMISSION_MODE;
      break;
  }
  return transmissionMode;
}

IO_DRIVE_MODE setIODriveMode(){
  IO_DRIVE_MODE IODriveMode;
  switch(configuration.parameters.OPTION.bits.IODM){
    case IO_DRIVE_MODE_PUSH_PULL:
      IODriveMode = IO_DRIVE_MODE_PUSH_PULL;
      break;
    case IO_DRIVE_MODE_OPEN_COLLECTOR:
      IODriveMode = IO_DRIVE_MODE_OPEN_COLLECTOR;
      break;
  }
  return IODriveMode;
}

WIRELESS_WAKE_UP_TIME setWirelessWakeUpTime(){
  WIRELESS_WAKE_UP_TIME wirelessWakeUpTime;
  switch(configuration.parameters.OPTION.bits.WWUT){
    case WIRELESS_WAKE_UP_TIME_250ms:
      wirelessWakeUpTime = WIRELESS_WAKE_UP_TIME_250ms;
      break;
    case WIRELESS_WAKE_UP_TIME_500ms:
      wirelessWakeUpTime = WIRELESS_WAKE_UP_TIME_500ms;
      break;
    case WIRELESS_WAKE_UP_TIME_750ms:
      wirelessWakeUpTime = WIRELESS_WAKE_UP_TIME_750ms;
      break;
    case WIRELESS_WAKE_UP_TIME_1000ms:
      wirelessWakeUpTime = WIRELESS_WAKE_UP_TIME_1000ms;
      break;
    case WIRELESS_WAKE_UP_TIME_1250ms:
      wirelessWakeUpTime = WIRELESS_WAKE_UP_TIME_1250ms;
      break;
    case WIRELESS_WAKE_UP_TIME_1500ms:
      wirelessWakeUpTime = WIRELESS_WAKE_UP_TIME_1500ms;
      break;
    case WIRELESS_WAKE_UP_TIME_1750ms:
      wirelessWakeUpTime = WIRELESS_WAKE_UP_TIME_1750ms;
      break;
    case WIRELESS_WAKE_UP_TIME_2000ms:
      wirelessWakeUpTime = WIRELESS_WAKE_UP_TIME_2000ms;
      break;
  }
  return wirelessWakeUpTime;
}

FEC_SWITCH setFECSwitch(){
  FEC_SWITCH FECSwitch;
  switch(configuration.parameters.OPTION.bits.FECS){
    case FEC_SWITCH_OFF:
      FECSwitch = FEC_SWITCH_OFF;
      break;
    case FEC_SWITCH_ON:
      FECSwitch = FEC_SWITCH_ON;
      break;
  }
  return FECSwitch;
}

TRANSMISSION_POWER getTransmissionPower(){
  switch(configuration.parameters.OPTION.bits.TXPW){
    case TRANSMISSION_POWER_20dBm:
      return TRANSMISSION_POWER_20dBm;
      break;
    case TRANSMISSION_POWER_17dBm:
      return TRANSMISSION_POWER_17dBm;
      break;
    case TRANSMISSION_POWER_14dBm:
      return TRANSMISSION_POWER_14dBm;
      break;
    case TRANSMISSION_POWER_10dBm:
      return TRANSMISSION_POWER_10dBm;
      break;
  }
}

void printHEAD(){
  Serial.print("Head: ");Serial.println(configuration.parameters.HEAD, HEX);
}

void printADDH(){
  Serial.print("ADDH: ");Serial.println(configuration.parameters.ADDH, HEX);
}

void printADDL(){
  Serial.print("ADDL: ");Serial.println(configuration.parameters.ADDL, HEX);
}

void printParity(){
  const char *parity;
  // (param[2]&0xC0)>>6
  Serial.print("UART parity bit: ");
  switch(configuration.parameters.SPED.bits.UART_parity_bit){
    case UART_PARITY_BIT_8N1:
      Serial.println("8N1");
      break;
    case UART_PARITY_BIT_8o1:
      Serial.println("8o1");
      break;
    case UART_PARITY_BIT_8E1:
      Serial.println("8E1");
      break;
    case UART_PARITY_BIT_8N1_11:
      Serial.println("8N1");
      break;
  }
}

void printBaudRate(){
  Serial.print("TTL UART baud rate (bps): ");Serial.println(getBaudRate());
}

void printAirDataRate(){
  Serial.print("Air data rate (bps): ");Serial.println(getAirDataRate());
}

void printChannel(){
  Serial.print("chan: ");Serial.println(configuration.parameters.CHAN, HEX);
}

void printTransmissionMode(){
  Serial.print("Transmission mode: ");
  switch(configuration.parameters.OPTION.bits.FTEB){
    case TRANSPARENT_TRANSMISSION_MODE:
      Serial.println("Transparent transmission mode");
      break;
    case FIXED_TRANSMISSION_MODE:
      Serial.println("Fixed transmission mode");
      break;
  }
}

void printTransmissionPower(){
  Serial.print("Transmission power: ");
  switch(configuration.parameters.OPTION.bits.TXPW){
    case TRANSMISSION_POWER_20dBm:
      Serial.println("20dBm");
      break;
    case TRANSMISSION_POWER_17dBm:
      Serial.println("17dBm");
      break;
    case TRANSMISSION_POWER_14dBm:
      Serial.println("14dBm");
      break;
    case TRANSMISSION_POWER_10dBm:
      Serial.println("10dBm");
      break;
  }
}

void setHEAD(HEAD head){
  configuration.parameters.HEAD = head;
}

void setADDH(uint8_t addh){
  configuration.parameters.ADDH = addh;
}

void setADDL(uint8_t addl){
  configuration.parameters.ADDL = addl;
}

void setParity(UART_PARITY_BIT parity){
  Serial.println(configuration.parameters.SPED.byte, BIN);
  configuration.parameters.SPED.bits.UART_parity_bit = parity;
  Serial.println(parity, BIN);
  Serial.println(configuration.parameters.SPED.byte, BIN);
}

void setBaudRate(TTL_UART_BAUD_RATE baudRate){
  configuration.parameters.SPED.bits.TTL_UART_baud_rate = baudRate;
}

void setBaudRate(unsigned long baudRate){
  switch(baudRate){
    case 1200:
      configuration.parameters.SPED.bits.TTL_UART_baud_rate = TTL_UART_baud_rate_1200;
      break;
    case 2400:
      configuration.parameters.SPED.bits.TTL_UART_baud_rate = TTL_UART_baud_rate_2400;
      break;
    case 4800:
      configuration.parameters.SPED.bits.TTL_UART_baud_rate = TTL_UART_baud_rate_4800;
      break;
    case 9600:
      configuration.parameters.SPED.bits.TTL_UART_baud_rate = TTL_UART_baud_rate_9600;
      break;
    case 19200:
      configuration.parameters.SPED.bits.TTL_UART_baud_rate = TTL_UART_baud_rate_19200;
      break;
    case 38400:
      configuration.parameters.SPED.bits.TTL_UART_baud_rate = TTL_UART_baud_rate_38400;
      break;
    case 57600:
      configuration.parameters.SPED.bits.TTL_UART_baud_rate = TTL_UART_baud_rate_57600;
      break;
    case 115200:
      configuration.parameters.SPED.bits.TTL_UART_baud_rate = TTL_UART_baud_rate_115200;
      break;
  }
}

void setAirDataRate(AIR_DATA_RATE airDataRate){
  configuration.parameters.SPED.bits.Air_data_rate = airDataRate;
}

void setAirDataRate(unsigned long airDataRate){
  switch(airDataRate){
    case 300:
      configuration.parameters.SPED.bits.TTL_UART_baud_rate = Air_Data_Rate_300;
      break;
    case 1200:
      configuration.parameters.SPED.bits.TTL_UART_baud_rate = Air_Data_Rate_1200;
      break;
    case 2400:
      configuration.parameters.SPED.bits.TTL_UART_baud_rate = Air_Data_Rate_2400;
      break;
    case 4800:
      configuration.parameters.SPED.bits.TTL_UART_baud_rate = Air_Data_Rate_4800;
      break;
    case 9600:
      configuration.parameters.SPED.bits.TTL_UART_baud_rate = Air_Data_Rate_9600;
      break;
    case 19200:
      configuration.parameters.SPED.bits.TTL_UART_baud_rate = Air_Data_Rate_19200;
      break;
  }
}

void setChannel(uint8_t channel){
  configuration.parameters.CHAN = channel;
}

void setTransmissionMode(FIXED_TRANSMISSION_ENABLING_BIT transmissionMode){
  configuration.parameters.OPTION.bits.FTEB = transmissionMode;
}

void setIODriveMode(IO_DRIVE_MODE IODriveMode){
  configuration.parameters.OPTION.bits.IODM = IODriveMode;
}

void setWirelessWakeUpTime(WIRELESS_WAKE_UP_TIME wirelessWakeUpTime){
  configuration.parameters.OPTION.bits.WWUT = wirelessWakeUpTime;
}

void setFECSwitch(FEC_SWITCH FECSwitch){
  configuration.parameters.OPTION.bits.FECS = FECSwitch;
}

void setTransmissionPower(TRANSMISSION_POWER power){
  configuration.parameters.OPTION.bits.TXPW = power;
}

void setTransmissionPower(uint8_t power){
  switch(power){
    case 10:
      configuration.parameters.OPTION.bits.TXPW = TRANSMISSION_POWER_10dBm;
      break;
    case 14:
      configuration.parameters.OPTION.bits.TXPW = TRANSMISSION_POWER_14dBm;
      break;
    case 17:
      configuration.parameters.OPTION.bits.TXPW = TRANSMISSION_POWER_17dBm;
      break;
    case 20:
      configuration.parameters.OPTION.bits.TXPW = TRANSMISSION_POWER_20dBm;
      break;
  }
}

void readConfiguration(){
  setSleepMode();
  // flush_e32serial("flush get1");

  Serial.println("Reading configuration from module");

  waitForAuxReady();

  // e32serial.write(0xC1);
  // e32serial.write(0xC1);
  // e32serial.write(0xC1);
  uint8_t buffer[] = {0xC1, 0xC1, 0xC1};
  write(buffer, 3);

  int counter = 0;
  while(counter<6){
    if(e32serial.available()){
      configuration.bytes[counter++] = e32serial.read();
    }
  }

  waitForAuxReady();
  Serial.print("\t");printHEAD();
  Serial.print("\t");printADDH();
  Serial.print("\t");printADDL();
  Serial.print("\t");printParity();
  Serial.print("\t");printBaudRate();
  Serial.print("\t");printAirDataRate();
  Serial.print("\t");printChannel();
  Serial.print("\t");printTransmissionMode();
  Serial.print("\t");printTransmissionPower();
}

void setConfiguration(){
  setSleepMode();

  Serial.println("Setting configuration to module");

  Serial.print("\t");
  write(configuration.bytes, 6);

  Serial.println("Waiting for response");
  uint8_t params[6];

  Serial.print("\t");
  read(params, 6);

  uint8_t b;
  for(uint8_t i = 0; i < 6; i++){
    b = params[i];
    if(configuration.bytes[i] != b){
      
      Serial.print("Failed to set configurations in byte ");
      Serial.println(i);
      Serial.print(configuration.bytes[i], HEX);
      Serial.print(" != ");
      Serial.println(b, HEX);
      while(1);
    }
  }
}
