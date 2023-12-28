#include "Arduino.h"
#include "Utils.h"

uint8_t ver[4];

void flush_e32serial(String msg){
  delay(2000);
  Serial.print(msg);Serial.print(" ");
  while(e32serial.available()){
    Serial.print((uint8_t)e32serial.read(), HEX);
    Serial.print(" ");
  }
  Serial.println("");
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

void parseMessage(String received_message){
  String s = "";
  switch(received_message[0]){
    case 'P':
      switch(received_message[1]){
        case 'N':
          Serial.println("Paritiy changed to 8N1");
          setParity(UART_PARITY_BIT_8N1);
          break;
        case 'E':
          Serial.println("Parity changed to 8E1");
          setParity(UART_PARITY_BIT_8E1);
          break;
        case 'O':
          Serial.println("Parity changed to 8o1");
          setParity(UART_PARITY_BIT_8o1);
          break;
      }
      break;
    case 'B':
      s = received_message.substring(1);
      Serial.print("UART baud rate changed to "); Serial.println(s.toInt());
      setBaudRate(s.toInt());
      break;
    case 'A':
      s = received_message.substring(1);
      Serial.print("Air data rate changed to "); Serial.println(s.toInt());
      setAirDataRate(s.toInt());
      break;
    case 'T':
      s = received_message.substring(1);
      Serial.print("Transmission power changed to "); Serial.println(s.toInt());
      setTransmissionPower(s.toInt());
      break;
    case 'H':
      s = received_message.substring(1);
      Serial.print("ADDH changed to "); Serial.println(s);
      setADDH(s.toInt());
      break;
    case 'L':
      s = received_message.substring(1);
      Serial.print("ADDL changed to "); Serial.println(s);
      setADDL(s.toInt());
      break;
    default:
      s = received_message.substring(0);
      Serial.print("Channel changed to ");
      Serial.println(s);
      setChannel(s.toInt());
      break;
  }
  setConfiguration();
  setNormalMode();
}

void printConfiguration(){
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

void printVersionInformation(){
  Serial.print("Module version information: ");

  Serial.print("\t");
  Serial.print(ver[0], HEX);
  Serial.print(" ");
  Serial.print(ver[1], HEX);
  Serial.print(" ");
  Serial.print(ver[2], HEX);
  Serial.print(" ");
  Serial.println(ver[3], HEX);
}