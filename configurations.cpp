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

IO_DRIVE_MODE getIODriveMode(){
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

WIRELESS_WAKE_UP_TIME getWirelessWakeUpTime(){
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

FEC_SWITCH getFECSwitch(){
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

// #define DBG
#ifdef DBG
#define DSerial(...) GET_MACRO(__VA_ARGS__, DSerial2, DSerial1)(__VA_ARGS__)
#define DSerialln(...) GET_MACRO(__VA_ARGS__, DSerialln2, DSerialln1)(__VA_ARGS__)
#define ON_DEBUG(x) {x};
#define Dinput(x) {input(x);}
#else
#define DSerial(...)
#define DSerialln(...)
#define ON_DEBUG(x)
#define Dinput(x)
#endif

void setHEAD(HEAD head){
  configuration.parameters.HEAD = head;
  ON_DEBUG(printChannel(););
}

void setADDH(uint8_t addh){
  configuration.parameters.ADDH = addh;
  ON_DEBUG(printADDH(););
}

void setADDL(uint8_t addl){
  configuration.parameters.ADDL = addl;
  ON_DEBUG(printADDL(););
}

void setParity(UART_PARITY_BIT parity){
  configuration.parameters.SPED.bits.UART_parity_bit = parity;
  ON_DEBUG(printParity(););
}

void setBaudRate(TTL_UART_BAUD_RATE baudRate){
  configuration.parameters.SPED.bits.TTL_UART_baud_rate = baudRate;
  ON_DEBUG(printBaudRate(););
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
  ON_DEBUG(printBaudRate(););
}

void setAirDataRate(AIR_DATA_RATE airDataRate){
  configuration.parameters.SPED.bits.Air_data_rate = airDataRate;
  ON_DEBUG(printAirDataRate(););
}

void setAirDataRate(unsigned long airDataRate){
  switch(airDataRate){
    case 300:
      configuration.parameters.SPED.bits.Air_data_rate = Air_Data_Rate_300;
      break;
    case 1200:
      configuration.parameters.SPED.bits.Air_data_rate = Air_Data_Rate_1200;
      break;
    case 2400:
      configuration.parameters.SPED.bits.Air_data_rate = Air_Data_Rate_2400;
      break;
    case 4800:
      configuration.parameters.SPED.bits.Air_data_rate = Air_Data_Rate_4800;
      break;
    case 9600:
      configuration.parameters.SPED.bits.Air_data_rate = Air_Data_Rate_9600;
      break;
    case 19200:
      configuration.parameters.SPED.bits.Air_data_rate = Air_Data_Rate_19200;
      break;
  }
  ON_DEBUG(printAirDataRate(););
}

void setChannel(uint8_t channel){
  configuration.parameters.CHAN = channel&0b00011111;
  ON_DEBUG(printChannel(););
}

void setTransmissionMode(FIXED_TRANSMISSION_ENABLING_BIT transmissionMode){
  configuration.parameters.OPTION.bits.FTEB = transmissionMode;
  ON_DEBUG(printTransmissionMode(););
  if(configuration.parameters.OPTION.bits.FTEB==FIXED_TRANSMISSION_MODE){
    hasFixedTransmission = true;
  } else{
    hasFixedTransmission = false;
  }
  // computeTransmissionFunction();
}

void setIODriveMode(IO_DRIVE_MODE IODriveMode){
  configuration.parameters.OPTION.bits.IODM = IODriveMode;
  // ON_DEBUG(printIODriveMode(););
}

void setWirelessWakeUpTime(WIRELESS_WAKE_UP_TIME wirelessWakeUpTime){
  configuration.parameters.OPTION.bits.WWUT = wirelessWakeUpTime;
  // ON_DEBUG(printWirelessWakeUpTime(););
}

void setFECSwitch(FEC_SWITCH FECSwitch){
  configuration.parameters.OPTION.bits.FECS = FECSwitch;
  // ON_DEBUG(printFECSwitch(););
}

void setTransmissionPower(TRANSMISSION_POWER power){
  configuration.parameters.OPTION.bits.TXPW = power;
  ON_DEBUG(printTransmissionPower(););
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
  ON_DEBUG(printTransmissionPower(););
}

void readConfiguration(){
  setSleepMode();
  // flush_e32serial("flush get1");

  DSerialln("Reading configuration from module");

  waitForAuxReady();

  uint8_t buffer[] = {0xC1, 0xC1, 0xC1};
  transmit(buffer, 3);

  int counter = 0;
  DSerialln("Waiting for response");
  while(counter<6){
    if(e32serial.available()){
      configuration.bytes[counter++] = e32serial.read();
    } else{
      DSerialln("nothing yet");
    }
  }

  if(configuration.parameters.OPTION.bits.FTEB==FIXED_TRANSMISSION_MODE){
    hasFixedTransmission = true;
  } else{
    hasFixedTransmission = false;
  }
  computeTransmissionFunction();
  waitForAuxReady();
  DSerial("\t");ON_DEBUG(printHEAD();)
  DSerial("\t");ON_DEBUG(printADDH();)
  DSerial("\t");ON_DEBUG(printADDL();)
  DSerial("\t");ON_DEBUG(printParity();)
  DSerial("\t");ON_DEBUG(printBaudRate();)
  DSerial("\t");ON_DEBUG(printAirDataRate();)
  DSerial("\t");ON_DEBUG(printChannel();)
  DSerial("\t");ON_DEBUG(printTransmissionMode();)
  DSerial("\t");ON_DEBUG(printTransmissionPower();)
}

bool setConfiguration(){
  OperationMode previousMode = getOperationMode();
  int attemps = 0;
  bool hasSucceeded = false;
  while(attemps<3 && !hasSucceeded){
    attemps++;
    // DSerialln("Setting configuration to module");
    setSleepMode();
    flush();

    // Dinput("Enter to send configuration bytes");
    transmit(configuration.bytes, 6);
    DSerial("\t");
    ON_DEBUG(for(int i = 0; i < 6; i++){Serial.print(configuration.bytes[i], HEX);Serial.print(" ");})
    DSerialln("");

    DSerialln("Waiting for response");
    uint8_t params[6];
    if(read(params, 6, 50)){
      DSerial("\t");
      ON_DEBUG(for(int i = 0; i < 6; i++){Serial.print(params[i], HEX);Serial.print(" ");})
      DSerialln("");

      uint8_t b;
      hasSucceeded = true;
      for(uint8_t i = 0; i < 6; i++){
        b = params[i];
        if(configuration.bytes[i] != b){
          DSerial("Failed to set configurations in byte ");
          DSerialln(i);
          DSerial(configuration.bytes[i], HEX);
          DSerial(" != ");
          DSerialln(b, HEX);
          hasSucceeded = false;
          break;
        }
      }
    }
    waitForAuxReady();
    // ON_DEBUG(readConfiguration(););
  }
  computeTransmissionFunction();
  setOperationMode(previousMode);
  return hasSucceeded;
}

#ifdef DBG
#undef DBG
#define DSerial(...)
#define DSerialln(...)
#define ON_DEBUG(x)
#define Dinput(x)
#endif

void CRC(){
  hasCRC = true;
  computeTransmissionFunction();
}

void CRC(uint8_t *_crcCode){
  crcKey[0] = _crcCode[0];
  crcKey[1] = _crcCode[1];
  hasCRC = true;
  computeTransmissionFunction();
}

void noCRC(){
  hasCRC = false;
  computeTransmissionFunction();
}

void computeTransmissionFunction(){
  payload_max_length = 58;
  if(hasCRC){
    payload_max_length -= 2;
  }
  if(hasFixedTransmission){
    payload_max_length -= 3;
  }
  switch(payload_max_length){
    case 58:
      transmitting_function = transmit;
      break;
    case 56:
      transmitting_function = transmitCRC;
      break;
    case 55:
      transmitting_function = FTtransmit;
      break;
    case 53:
      transmitting_function = FTtransmitCRC;
      break;
  }
  Serial.print("Payload max length ");
  Serial.println(payload_max_length);
}