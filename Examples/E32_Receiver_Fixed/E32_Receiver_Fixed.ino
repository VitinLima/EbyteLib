/*
    Transparent Transmitter => Transparent Receiver
    Asynchronous Transparent Transmitter => Transparent Receiver
*/

#include "EbyteLib.h"

uint8_t txChan = 10;
uint8_t txAddh = 0x8f;
uint8_t txAddl = 0xf7;
uint8_t rxChan = 23;
uint8_t rxAddh = 0xa1;
uint8_t rxAddl = 0x06;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);

  delay(1500);
  Serial.println("Testing e32serial fixed receiver");

  initE32();
  Serial.println("Device initiated successfully");

  resetModule(); // Self check, not required

  setHEAD(SAVE_ON_POWER_DOWN);
  setADDH(rxAddh);
  setADDL(rxAddl);
  setChannel(rxChan);
  setParity(UART_PARITY_BIT_8N1);
  setBaudRate(TTL_UART_baud_rate_9600);
  setAirDataRate(Air_Data_Rate_9600);
  setTransmissionMode(FIXED_TRANSMISSION_MODE);
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

struct Message{
  char type[10];
  char message_1[13];
  char message_2[16];
  float value_1;
  float value_2;
  float value_3;
  float value_4;
} message;

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
    ((uint8_t*)&message)[message_index++] = (uint8_t)e32serial.read();
    if(message_index==sizeof(message)){
      message_index = 0;
      Serial.print("\n\nMessage received!\n\n");
      Serial.print("Message type: ");Serial.println(message.type);
      Serial.print("Message 1: ");Serial.println(message.message_1);
      Serial.print("Message 2: ");Serial.println(message.message_2);
      Serial.print("Value 1: ");Serial.println(message.value_1);
      Serial.print("Value 2: ");Serial.println(message.value_2);
      Serial.print("Value 3: ");Serial.println(message.value_3);
      Serial.print("Value 4: ");Serial.println(message.value_4);
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
    } else if(received_message[0] == 'T'){
      if(received_message.startsWith("Tset tx chan ")){
        txChan = received_message.substring(13).toInt();
        Serial.print("Set tx channel to ");
        Serial.println(txChan);
      } else if(received_message.startsWith("Tset tx addh ")){
        txAddh = received_message.substring(13).toInt();
        Serial.print("Set tx addh to ");
        Serial.println(txAddh);
      } else if(received_message.startsWith("Tset tx addl ")){
        txAddl = received_message.substring(13).toInt();
        Serial.print("Set tx addl to ");
        Serial.println(txAddl);
      }
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