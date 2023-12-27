#include "Arduino.h"
#include "EbyteLib.h"

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

void waitForAuxReady(){
  // Serial.print("Waiting for aux ready pin ");
  // Serial.println(AUX);
  delay(50);
  while(!digitalRead(AUX));
  // Serial.println("Aux ready");
  delay(5);
}

void initE32(){
  e32serial.begin(9600);

  // attachInterrupt(digitalPinToInterrupt(AUX), auxChangeISR, CHANGE);
  // attachInterrupt(digitalPinToInterrupt(AUX), auxRisingISR, RISING);
  attachInterrupt(digitalPinToInterrupt(AUX), auxFallingISR, FALLING);

  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(AUX, INPUT);

  digitalWrite(M0, HIGH);
  digitalWrite(M1, HIGH);
  current_operation_mode = SLEEP;

  waitForAuxReady();
  readConfiguration();
  waitForAuxReady();

  Serial.println("Module initiated and ready to accept instructions");
}

void resetModule(){
  setSleepMode();

  Serial.println("Reseting module");

  e32serial.write(0xC4);
  e32serial.write(0xC4);
  e32serial.write(0xC4);

  if(digitalRead(AUX)){
    Serial.println("Module is not resetting, error");
    while(1);
  }

  waitForAuxReady();

  while(digitalRead(AUX));

  waitForAuxReady();
  // delay(1000);
  Serial.println("Module has been reset successfully");
}

void getVersionInformation(){
  setSleepMode();

  Serial.print("Module version information: ");

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
  Serial.print(ver[0], HEX);
  Serial.print(" ");
  Serial.print(ver[1], HEX);
  Serial.print(" ");
  Serial.print(ver[2], HEX);
  Serial.print(" ");
  Serial.println(ver[3], HEX);
}

void print(const char* message){
  waitForAuxReady();
  auxHighFlag = false;
  auxLowFlag = false;
  e32serial.print(message);
}

void write(uint8_t* buffer, unsigned int size){
  waitForAuxReady();
  noInterrupts();
  auxHighFlag = false;
  auxLowFlag = false;
  Serial.print("Writing: ");
  printHEX(buffer, size);
  e32serial.write(buffer, size);
  interrupts();
}

void write(uint8_t byte){
  waitForAuxReady();
  noInterrupts();
  auxHighFlag = false;
  auxLowFlag = false;
  // Serial.print("Writing: ");
  // printHEX(buffer, size);
  e32serial.write(byte);
  interrupts();
}

void writeFixedTransmission(uint8_t ADDH, uint8_t ADDL, uint8_t CHAN, uint8_t* buffer, unsigned int size){
  uint8_t transmitting_buffer[3 + size];
  transmitting_buffer[0] = ADDH;
  transmitting_buffer[1] = ADDL;
  transmitting_buffer[2] = CHAN;
  for(uint8_t i = 0; i < size; i++){
    transmitting_buffer[i+3] = buffer[i];
  }
  write(transmitting_buffer, size+3);
}

void writeFixedTransmission(uint8_t ADDH, uint8_t ADDL, uint8_t CHAN, uint8_t byte){
  uint8_t transmitting_buffer[4];
  transmitting_buffer[0] = ADDH;
  transmitting_buffer[1] = ADDL;
  transmitting_buffer[2] = CHAN;
  transmitting_buffer[3] = byte;
  write(transmitting_buffer, 4);
}

void read(uint8_t* buffer, unsigned int size){
  unsigned int counter = 0;
  Serial.print("Reading: ");
  while(counter<size){
    if(e32serial.available()){
      buffer[counter] = (uint8_t)e32serial.read();
      Serial.print(buffer[counter++], HEX);
      Serial.print(" ");
    }
  }
  Serial.println("");
}

const char* getTransmissionResult(){
  long unsigned int timeout_limit = millis() + timeout;
  while(millis() < timeout_limit){
    if(auxLowFlag){
      while(millis() < timeout_limit){
        if(digitalRead(AUX)){
          return "Success";
        }
      }
      return "Timeout";
    }
  }
  return "Timeout";
}

bool transmissionSuccess(){
  long unsigned int timeout_limit = millis() + timeout;
  while(millis() < timeout_limit){
    if(auxLowFlag){
      while(millis() < timeout_limit){
        if(digitalRead(AUX)){
          return true;
        }
      }
      return false;
    }
  }
  return false;
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
      Serial.print("UART baud rate changed to "); Serial.println(s);
      setBaudRate(s.toInt());
      break;
    case 'A':
      s = received_message.substring(1);
      Serial.print("Air data rate changed to "); Serial.println(s);
      setAirDataRate(s.toInt());
      break;
    case 'T':
      s = received_message.substring(1);
      Serial.print("Transmission power changed to "); Serial.println(s);
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

void auxChangeISR(){
  // Serial.println("Aux change");
}

void auxRisingISR(){
  // Serial.println("Aux rising");
  auxHighFlag = true;
  attachInterrupt(digitalPinToInterrupt(AUX), auxFallingISR, FALLING);
}
void auxFallingISR(){
  // Serial.println("Aux falling");
  auxLowFlag = true;
  attachInterrupt(digitalPinToInterrupt(AUX), auxRisingISR, RISING);
  // while(!digitalRead(AUX));
  // Serial.println("Aux rising");
}

// void onAuxHigh(){
//   Serial.println("Aux High");
//   auxHighInterrupt = true;
// }

// void onAuxLow(){
//   Serial.println("Aux Low");
//   auxLowInterrupt = true;
// }
