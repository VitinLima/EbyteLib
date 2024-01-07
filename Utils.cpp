#include "Arduino.h"
#include "Utils.h"

uint8_t ver[4];

// void flush_e32serial(String msg){
//   delay(2000);
//   DSerial(msg);DSerial(" ");
//   while(e32serial.available()){
//     DSerial((uint8_t)e32serial.read(), HEX);
//     DSerial(" ");
//   }
//   DSerialln("");
// }

void flush(){
  while(e32serial.available()){
    e32serial.read();
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

// void parseMessage(String received_message){
//   String s = "";
//   if(received_message.length() > 0){
//     switch(received_message[0]){
//       case 'P':
//         if(received_message.length() == 2){
//           switch(received_message[1]){
//             case 'N':
//               DSerialln("Paritiy changed to 8N1");
//               setParity(UART_PARITY_BIT_8N1);
//               break;
//             case 'E':
//               DSerialln("Parity changed to 8E1");
//               setParity(UART_PARITY_BIT_8E1);
//               break;
//             case 'O':
//               DSerialln("Parity changed to 8o1");
//               setParity(UART_PARITY_BIT_8o1);
//               break;
//           }
//         }
//         break;
//       case 'B':
//         s = received_message.substring(1);
//         DSerial("UART baud rate changed to "); DSerialln(s.toInt());
//         setBaudRate(s.toInt());
//         break;
//       case 'A':
//         s = received_message.substring(1);
//         DSerial("Air data rate changed to "); DSerialln(s.toInt());
//         setAirDataRate(s.toInt());
//         break;
//       case 'T':
//         s = received_message.substring(1);
//         DSerial("Transmission power changed to "); DSerialln(s.toInt());
//         setTransmissionPower(s.toInt());
//         break;
//       case 'H':
//         s = received_message.substring(1);
//         DSerial("ADDH changed to "); DSerialln(s);
//         setADDH(s.toInt());
//         break;
//       case 'L':
//         s = received_message.substring(1);
//         DSerial("ADDL changed to "); DSerialln(s);
//         setADDL(s.toInt());
//         break;
//       case 'C':
//         s = received_message.substring(1);
//         DSerial("Channel changed to ");
//         DSerialln(s);
//         setChannel(s.toInt());
//         break;
//     }
//   }
//   setConfiguration();
//   // setNormalMode();
// }

#ifdef DBG
#undef DBG
#define DSerial(...)
#define DSerialln(...)
#define ON_DEBUG(x)
#define Dinput(x)
#endif

void printConfiguration(){
  printHEAD();
  printADDH();
  printADDL();
  printParity();
  printBaudRate();
  printAirDataRate();
  printChannel();
  printTransmissionMode();
  printTransmissionPower();
}

void getVersionInformation(){
  setSleepMode();

  e32serial.write(0xC3);
  e32serial.write(0xC3);
  e32serial.write(0xC3);

  int counter = 0;
  while(counter<4){
    if(e32serial.available()){
      // Serial.println(e32serial.peek(), HEX);
      ver[counter++] = e32serial.read();
    }
  }

  waitForAuxReady();
}

void printTransmissionResult(unsigned long int timeout){
  long unsigned int timeout_limit = millis() + timeout;
  while(millis() < timeout_limit){
    if(transmission_started && transmission_finished && !asyncronousTransmissionFlag){
      Serial.println("Success");
      return;
    }
  }
  Serial.println("Timeout");
}

bool waitForTimeout(bool (*foo()), unsigned long int timeout){
  long unsigned int timeout_limit = millis() + timeout;
  while(millis() < timeout_limit){
    if(foo()){
      return true;
    }
  }
  return false;
}

bool waitForTimeout(bool *foo, unsigned long int timeout){
  long unsigned int timeout_limit = millis() + timeout;
  while(millis() < timeout_limit){
    if(*foo){
      return true;
    }
  }
  return false;
}

void printVersionInformation(){
  Serial.print("Module version information: ");

  Serial.print(ver[0], HEX);
  Serial.print(" ");
  Serial.print(ver[1], HEX);
  Serial.print(" ");
  Serial.print(ver[2], HEX);
  Serial.print(" ");
  Serial.println(ver[3], HEX);
}

/* 
  Found this elegant code here: https://forum.arduino.cc/t/quickly-reversing-a-byte/115529/3
  Ended up not using it though
*/
uint8_t inverse_byte(uint8_t b){
  b = ((b>>1) & 0x55) | ((b<<1) & 0xAA);
  b = ((b>>2) & 0x33) | ((b<<2) & 0xCC);
  b = (b>>4) | (b<<4);
  return b;
}