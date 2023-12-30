/*
    Asynchronous Fixed Transmitter => Asynchronous Fixed Receiver (TODO)
    Asynchronous Fixed Transmitter => Fixed Receiver
*/

#include "EbyteLib.h"

uint8_t txChan = 23;
uint8_t txAddh = 0xa1;
uint8_t txAddl = 0x06;
uint8_t rxChan = 10;
uint8_t rxAddh = 0x8f;
uint8_t rxAddl = 0xf7;

#define N 50 // Size of the added array (+1 for an end of line char)  
struct Message{
  const unsigned int length = sizeof(Message);
  const char type[10] = "Telemetry";
  const char message_1[13] = "Hello There!";
  const char message_2[16] = "General Kenobi!";
  float value_1 = 0.1;
  float value_2 = 0.2;
  float value_3 = 0.3;
  // float value_4 = 0.4;
  uint8_t bytes[N+1]; // Add array so that the message requires more than one packet to be sent
} message; // sending a struct with multiple fields

void setup(){
  Serial.begin(57600);

  Serial.println("Testing e32serial asynchronous fixed transmitter");

  initE32();
  Serial.println("Device initiated successfully");

  setHEAD(SAVE_ON_POWER_DOWN);
  setADDH(rxAddh);
  setADDL(rxAddl);
  setChannel(rxChan);
  setParity(UART_PARITY_BIT_8N1);
  setBaudRate(TTL_UART_baud_rate_2400);
  setAirDataRate(Air_Data_Rate_9600);
  setTransmissionMode(FIXED_TRANSMISSION_MODE);
  setIODriveMode(IO_DRIVE_MODE_PUSH_PULL);
  setWirelessWakeUpTime(WIRELESS_WAKE_UP_TIME_250ms);
  setFECSwitch(FEC_SWITCH_ON);
  setTransmissionPower(TRANSMISSION_POWER_20dBm);
  setConfiguration();
  
  printConfiguration();
  setNormalMode();

  for(uint8_t i = 0; i < N; i++){ // Initialize array
    message.bytes[i] = 0xA1;
  }
  message.bytes[N] = (uint8_t)'\n';

  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");
}

String receiving_message = "";
String received_message = "";
bool message_received = false;

bool state_sending = false;

void loop(){
  checkSerials();

  if(state_sending){
    asynchronousWriteFixedTransmission(txAddh, txAddl, txChan, (uint8_t*)&message, sizeof(message));
    printTransmissionResult(5000);
  }

  delay(500);
}

void checkSerials(){
  checkSerial();
  checkE32Serial();
}

void checkE32Serial(){
  char c;
  if(e32serial.available()){
    c = e32serial.read();
    Serial.println((uint8_t)c, HEX);
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
      state_sending = !state_sending;
      Serial.print("Toggled sending state to ");
      Serial.println(state_sending);
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

