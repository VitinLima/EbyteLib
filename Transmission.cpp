#include "Arduino.h"
#include "Transmission.h"

bool asyncronousTransmissionFlag = false;
bool fifo_buffer_overflow = false;
const uint8_t fifo_buffer[FIFO_BUFFER_MAX_LENGTH];
uint8_t fifo_buffer_length = 0;
uint8_t fifo_buffer_pointer = 0;
uint8_t write_fifo_buffer_pointer = 0;
const uint8_t fifo_buffer_max_length = FIFO_BUFFER_MAX_LENGTH;
const uint8_t fifo_helper_buffer[58];

void asyncronousTransmissionCallback(){
  uint8_t *transmitting_buffer;
  if(fifo_buffer_pointer+fifo_buffer_length < fifo_buffer_max_length){ // faster
    transmitting_buffer = (uint8_t*)&fifo_buffer[fifo_buffer_pointer];
  } else{ // use helper fifo, have to copy contents, slightly slower
    transmitting_buffer = (uint8_t*)fifo_helper_buffer;
    uint8_t k = 0;
    uint8_t i = fifo_buffer_pointer;
    uint8_t r;
    if(fifo_buffer_length < 58){
      r = fifo_buffer_length;
    } else{
      r = 58;
    }
    while(i < fifo_buffer_max_length){
      ((uint8_t*)fifo_helper_buffer)[k++] = fifo_buffer[i++];
    }
    i = 0;
    while(k < r){
      ((uint8_t*)fifo_helper_buffer)[k++] = fifo_buffer[i++];
    }
  }
  if(fifo_buffer_length < 58){ // can send all remaining bytes together
    waitForAuxReady();
    write(transmitting_buffer, fifo_buffer_length);
    waitForAuxReady();
    asyncronousTransmissionFlag = false; // all bytes transmitted, end asyncronous transmission
    fifo_buffer_pointer += fifo_buffer_length;
    fifo_buffer_length = 0;
  } else{ // send next 58 bytes, wait for finishing and send the rest
    waitForAuxReady();
    write(transmitting_buffer, 58);
    waitForAuxReady();
    fifo_buffer_pointer += 58;
    fifo_buffer_length -= 58;
  }
}

void write_to_fifo(uint8_t* buffer, uint8_t size){
  uint8_t k = 0;
  while(k < size){
    ((uint8_t*)fifo_buffer)[write_fifo_buffer_pointer++] = buffer[k++];
  }
}

void print(const char* message){
  auxHighFlag = false;
  auxLowFlag = false;
  waitForAuxReady();
  e32serial.print(message);
  waitForAuxReady();
}

void printFixedTransmission(uint8_t ADDH, uint8_t ADDL, uint8_t CHAN, const char* message){
  uint8_t size = 0;
  while(message[size++] != '\0');
  uint8_t transmitting_buffer[3 + size];
  transmitting_buffer[0] = ADDH;
  transmitting_buffer[1] = ADDL;
  transmitting_buffer[2] = CHAN;
  for(uint8_t i = 0; i < size; i++){
    transmitting_buffer[i+3] = message[i];
  }
  waitForAuxReady();
  e32serial.print((const char*)transmitting_buffer);
  waitForAuxReady();
}

void write(uint8_t* buffer, unsigned int size){
  // noInterrupts();
  auxHighFlag = false;
  auxLowFlag = false;
  DSerial("Writing: ");
  printHEX(buffer, size);
  waitForAuxReady();
  e32serial.write(buffer, size);
  waitForAuxReady();
  // interrupts();
  DSerialln("done");
}

void write(uint8_t byte){
  // noInterrupts();
  auxHighFlag = false;
  auxLowFlag = false;
  // DSerial("Writing: ");
  // printHEX(buffer, size);
  waitForAuxReady();
  e32serial.write(byte);
  waitForAuxReady();
  // interrupts();
}

void asyncronousWrite(uint8_t* buffer, unsigned int size){
  if(size < 58){
    if(asyncronousTransmissionFlag){
      write_to_fifo(buffer, size);
    } else{
      write(buffer, size);
    }
  } else{
    write_to_fifo(buffer, size);
  }
}

void asyncronousWrite(uint8_t byte){

}

void writeFixedTransmission(uint8_t ADDH, uint8_t ADDL, uint8_t CHAN, uint8_t* buffer, unsigned int size){
  uint8_t transmitting_buffer[3 + size];
  transmitting_buffer[0] = ADDH;
  transmitting_buffer[1] = ADDL;
  transmitting_buffer[2] = CHAN;
  for(uint8_t i = 0; i < size; i++){
    transmitting_buffer[i+3] = buffer[i];
  }
  waitForAuxReady();
  write(transmitting_buffer, size+3);
  waitForAuxReady();
}

void writeFixedTransmission(uint8_t ADDH, uint8_t ADDL, uint8_t CHAN, uint8_t byte){
  uint8_t transmitting_buffer[4];
  transmitting_buffer[0] = ADDH;
  transmitting_buffer[1] = ADDL;
  transmitting_buffer[2] = CHAN;
  transmitting_buffer[3] = byte;
  write(transmitting_buffer, 4);
}

void asyncronouswriteFixedTransmission(uint8_t ADDH, uint8_t ADDL, uint8_t CHAN, uint8_t* buffer, unsigned int size){

}

void asyncronouswriteFixedTransmission(uint8_t ADDH, uint8_t ADDL, uint8_t CHAN, uint8_t byte){

}

void read(uint8_t* buffer, unsigned int size){
  unsigned int counter = 0;
  DSerial("Reading: ");
  while(counter<size){
    if(e32serial.available()){
      buffer[counter] = (uint8_t)e32serial.read();
      DSerial(buffer[counter++], HEX);
      DSerial(" ");
    }
  }
  DSerialln("");
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