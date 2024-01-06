/*
    Fixed Transmitter => Fixed Receiver
    Asynchronous Fixed Transmitter => Fixed Receiver
*/

#include "EbyteLib.h"

uint8_t rxtxChan = 23;
uint8_t rxtxAddh = 0xa1;
uint8_t rxtxAddl = 0x06;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);

  Serial.println("Testing e32serial transparent receiver");

  initE32();
  Serial.println("Device initiated successfully");

  resetModule(); // Self check, not required

  setHEAD(SAVE_ON_POWER_DOWN);
  setADDH(rxtxAddh);
  setADDL(rxtxAddl);
  setChannel(rxtxChan);
  setParity(UART_PARITY_BIT_8N1);
  setBaudRate(TTL_UART_BAUD_RATE_9600);
  setAirDataRate(AIR_DATA_RATE_2400);
  setTransmissionMode(TRANSPARENT_TRANSMISSION_MODE);
  setIODriveMode(IO_DRIVE_MODE_PUSH_PULL);
  setWirelessWakeUpTime(WIRELESS_WAKE_UP_TIME_250ms);
  setFECSwitch(FEC_SWITCH_ON);
  setTransmissionPower(TRANSMISSION_POWER_20dBm);
  setConfiguration();
  
  printConfiguration();
  setNormalMode();

  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");
}

String receiving_message = "";
String received_message = "";
bool message_received = false;

#define N 50 // Size of the added array (+1 for an end of line char) 
struct Message{
  unsigned int length;
  char type[10];
  char message_1[13];
  char message_2[16];
  float value_1;
  float value_2;
  float value_3;
  float value_4;
  uint8_t bytes[N+1]; // Add array so that the message requires more than one packet to be sent
};

Message message;
unsigned int message_index = 0;

void loop() {
  // put your main code here, to run repeatedly:
  checkSerials();
}

void checkSerials(){
  checkSerial();
  checkE32Serial();
}

void checkE32Serial(){
  char c;
  while(e32serial.available()){
    if(e32serial.overflow()){
      Serial.println("Serial OVERFLOW");
    }
    ((uint8_t*)&message)[message_index++] = (uint8_t)e32serial.read();
    if(message_index < 2){
      
    } else if(message_index == 2){
      Serial.print("\n\nReceiving message of length ");
      Serial.println(message.length);
    } else if(message_index==message.length){
      message_index = 0;
      Serial.print("Message received!\n\n");
      Serial.print("Message type: ");Serial.println(message.type);
      Serial.print("Message 1: ");Serial.println(message.message_1);
      Serial.print("Message 2: ");Serial.println(message.message_2);
      Serial.print("Value 1: ");Serial.println(message.value_1);
      Serial.print("Value 2: ");Serial.println(message.value_2);
      Serial.print("Value 3: ");Serial.println(message.value_3);
      Serial.print("Value 4: ");Serial.println(message.value_4);
      if(message.length==108){
        for(uint8_t i = 0; i < N; i++){
          if(message.bytes[i] != 0xa1){
            Serial.print("Error on byte ");
            Serial.print(i);
            Serial.print(". Expected 0xa1, received 0x");
            Serial.println(message.bytes[i], HEX);
          }
        }
        if(message.bytes[N] != (uint8_t)'\n'){
          Serial.print("Error on last byte (");
          Serial.print(N);
          Serial.print("), expected ");
          Serial.print((uint8_t)'\n', HEX);
          Serial.print(", received ");
          Serial.println(message.bytes[N], HEX);
        }
      }
    }
  }
}

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
      message_index = 0;
      Serial.println("Set message index to 0");
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