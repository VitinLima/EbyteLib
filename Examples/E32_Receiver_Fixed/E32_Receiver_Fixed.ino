/*
    Transparent Transmitter => Transparent Receiver
    Asynchronous Transparent Transmitter => Transparent Receiver
*/

#include "EbyteLib.h"

uint8_t rxChan = 23;
uint8_t rxAddh = 0xa1;
uint8_t rxAddl = 0x06;

struct Message{
  char type[10];
  char message_1[13];
  char message_2[16];
  float value_1;
  float value_2;
  float value_3;
  float value_4;
};

Message message;
unsigned int message_index = 0;

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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(1200);

  delay(1500);
  Serial.println("Testing e32serial fixed receiver");

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

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
  setBaudRate(TTL_UART_baud_rate_9600);
  setAirDataRate(Air_Data_Rate_9600);
  setTransmissionMode(FIXED_TRANSMISSION_MODE);
  setIODriveMode(IO_DRIVE_MODE_PUSH_PULL);
  setWirelessWakeUpTime(WIRELESS_WAKE_UP_TIME_250ms);
  setFECSwitch(FEC_SWITCH_ON);
  setTransmissionPower(TRANSMISSION_POWER_20dBm);
  setConfiguration();
  readConfiguration();
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

void loop() {
  // put your main code here, to run repeatedly:

  if(message_received){
    message_received = false;
    if(received_message.length()==0){
      message_index = 0;
      Serial.println("Set message index to 0");
    } else{
      parseMessage(received_message);
    }
  }

  if(e32serial.available()){
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

void serialEvent(){
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
}