#include "Arduino.h"
#include "Transmission.h"

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