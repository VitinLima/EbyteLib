/*
    Fixed Transmitter => Fixed Receiver
    Asynchronous Fixed Transmitter => Fixed Receiver
*/

#include "EbyteLib.h"

uint8_t rxChan = 23;
uint8_t rxAddh = 0xa1;
uint8_t rxAddl = 0x06;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);

  Serial.println("Testing e32serial transparent receiver");

  initE32();

  // resetModule();
  // waitForAuxReady();
  // readConfiguration();
  getVersionInformation();
  Serial.println("Device initiated successfully");

  // setHEAD(DONT_SAVE_ON_POWER_DOWN);
  setADDH(rxAddh);
  setADDL(rxAddl);
  setChannel(rxChan);
  setParity(UART_PARITY_BIT_8N1);
  setBaudRate(TTL_UART_baud_rate_19200);
  setAirDataRate(Air_Data_Rate_2400);
  setTransmissionMode(TRANSPARENT_TRANSMISSION_MODE);
  setIODriveMode(IO_DRIVE_MODE_PUSH_PULL);
  setWirelessWakeUpTime(WIRELESS_WAKE_UP_TIME_250ms);
  setFECSwitch(FEC_SWITCH_ON);
  setTransmissionPower(TRANSMISSION_POWER_20dBm);
  setConfiguration();
  readConfiguration();
  setNormalMode();
  // detachInterrupt(digitalPinToInterrupt(AUX));

  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");
}

String receiving_message = "";
String received_message = "";
bool message_received = false;

unsigned int e32Counter = 0;

void loop() {
  // put your main code here, to run repeatedly:
  checkSerials();
}

void checkSerials(){
  checkSerial();
  checkE32Serial();
}

/*
  On receiving
*/
void checkE32Serial(){
  char c;
  while(e32serial.available()){
    c = (uint8_t)e32serial.read();
    if(c=='\n'){
      Serial.print("Message length: ");
      Serial.println(e32Counter);
      e32Counter = 0;
    } else{
      Serial.print("char hex: ");
      Serial.print((uint8_t)c, HEX);
      Serial.print(" (");
      Serial.print(e32Counter);
      Serial.println(")");
      e32Counter++;
    }
  }
}

/*
  Example utilities
*/
void checkSerial(){
  char c;
  while(Serial.available() && !message_received){
    c = Serial.read();
    if(c=='\n'){
      received_message = receiving_message;
      receiving_message = "";
      message_received = true;
    } else{
      receiving_message += c;
    }
  }

  if(message_received){
    message_received = false;
    if(received_message.length()==0){
      e32Counter = 0;
      Serial.println("Set message counter to 0");
    } else if(received_message[0] == 'G'){
      if(received_message.startsWith("GAddh")){
        printADDH();
      } else if(received_message.startsWith("GAddl")){
        printADDL();
      } else if(received_message.startsWith("GParity")){
        printParity();
      } else if(received_message.startsWith("GAirDataRate")){
        printAirDataRate();
      } else if(received_message.startsWith("GBaudRate")){
        printBaudRate();
      } else if(received_message.startsWith("GChannel")){
        printChannel();
      } else if(received_message.startsWith("GTxPower")){
        printTransmissionPower();
      }
    } else if(received_message[0] == 'S'){
      parseMessage(received_message.substring(1));
    }
  }
}