#include "EbyteLib.h"

uint8_t chan = 23;

const int N = 100;
uint8_t buffer[N+1]; // sending a buffer byte by byte

void setup() {
  // put your setup code here, to run once:
  Serial.begin(1200);

  Serial.println("Testing e32serial asynchronous transparent transmitter");

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  initE32();

  // resetModule();
  // waitForAuxReady();
  // readConfiguration();
  getVersionInformation();
  Serial.println("Device initiated successfully");

  // setHEAD(DONT_SAVE_ON_POWER_DOWN);
  setADDH(0xff);
  setADDL(0xff);
  setChannel(chan);
  setParity(UART_PARITY_BIT_8N1);
  setBaudRate(TTL_UART_baud_rate_9600);
  setAirDataRate(Air_Data_Rate_9600);
  setTransmissionMode(TRANSPARENT_TRANSMISSION_MODE);
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
  for(int i = 0; i < N; i++){
    buffer[i] = 0xA1;
  }
  buffer[N] = (uint8_t)'\n';
}

String receiving_message = "";
String received_message = "";
bool message_received = false;

bool state_sending = false;

struct Message{
  const char type[10] = "Telemetry";
  const char message_1[13] = "Hello There!";
  const char message_2[16] = "General Kenobi!";
  float value_1 = 0.1;
  float value_2 = 0.2;
  float value_3 = 0.3;
  float value_4 = 0.4;
} message;

void loop() {
  // put your main code here, to run repeatedly:
  checkSerials();

  if(state_sending){
    asynchronousWrite(buffer, N+1);
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
    } else{
      parseMessage(received_message);
    }
  }
}
