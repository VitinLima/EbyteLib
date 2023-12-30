/*
    Transparent Transmitter => Asynchronous Transparent Receiver (TODO)
    Transparent Transmitter => Transparent Receiver
*/

#include "EbyteLib.h"

uint8_t rxtxChan = 23;
uint8_t rxtxAddh = 0xa1;
uint8_t rxtxAddl = 0x06;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);

  Serial.println("Testing e32serial transparent transmitter");

  initE32();
  Serial.println("Device initiated successfully");

  resetModule(); // Self check, not required

  setHEAD(SAVE_ON_POWER_DOWN);
  setADDH(rxtxAddh);
  setADDL(rxtxAddl);
  setChannel(rxtxChan);
  setParity(UART_PARITY_BIT_8N1);
  setBaudRate(TTL_UART_baud_rate_9600);
  setAirDataRate(Air_Data_Rate_2400);
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

bool state_sending = false;

struct Message{
  const unsigned int length = sizeof(Message); // The lenght of the message, so that the receiver can determine if it is the synchronous or asynchronous examples messages automatically, since they have different lengths
  const char type[10] = "Telemetry";
  const char message_1[13] = "Hello There!";
  const char message_2[16] = "General Kenobi!";
  float value_1 = 0.1;
  float value_2 = 0.2;
  float value_3 = 0.3;
  // float value_4 = 0.4;
} message; // sending a struct with multiple fields

void loop() {
  // put your main code here, to run repeatedly:
  checkSerials();

  if(state_sending){
    write((uint8_t*)&message, sizeof(message));
    printTransmissionResult(2000);
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
