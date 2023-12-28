#include "Arduino.h"
#include "Transmission.h"

bool asyncronousTransmissionFlag = false;
bool transmission_finished = false;
bool fifo_buffer_overflow = false;
const uint8_t fifo_buffer[FIFO_BUFFER_MAX_LENGTH];
unsigned int fifo_buffer_length = 0;
unsigned int fifo_buffer_pointer = 0;
unsigned int write_fifo_buffer_pointer = 0;
const unsigned int fifo_buffer_max_length = FIFO_BUFFER_MAX_LENGTH;
const uint8_t fifo_helper_buffer[58];

void asyncronousTransmissionCallback(){
  uint8_t *transmitting_buffer;
  D3Serialln(fifo_buffer_length);
  D3Serialln(fifo_buffer_pointer);
  D3Serialln(fifo_buffer_max_length);
  D3Serialln("");
  if(fifo_buffer_pointer+fifo_buffer_length <= fifo_buffer_max_length){ // faster
    D3Serialln("Transmitting buffer fifo");
    transmitting_buffer = (uint8_t*)&fifo_buffer[fifo_buffer_pointer];
  } else{ // use helper fifo, have to copy contents, slightly slower
    D3Serialln("Transmitting buffer helper");
    transmitting_buffer = (uint8_t*)fifo_helper_buffer;
    uint8_t k = 0;
    // unsigned int i = ;
    unsigned int r;
    if(fifo_buffer_length < 58){
      r = fifo_buffer_length;
    } else{
      r = 58;
    }
    while(fifo_buffer_pointer < fifo_buffer_max_length){
      D3Serial(k);
      D3Serial(" ");
      D3Serial(fifo_buffer_pointer);
      D3Serial(" ");
      D3Serialln(r);
      ((uint8_t*)fifo_helper_buffer)[k++] = fifo_buffer[fifo_buffer_pointer++];
    }
    D3Serialln("Second part");
    fifo_buffer_pointer = 0;
    while(k < r){
      D3Serial(k);
      D3Serial(" ");
      D3Serial(fifo_buffer_pointer);
      D3Serial(" ");
      D3Serialln(r);
      ((uint8_t*)fifo_helper_buffer)[k++] = fifo_buffer[fifo_buffer_pointer++];
    }
  }
  if(fifo_buffer_length < 58){ // can send all remaining bytes together
    D3Serialln("Last packet");
    write(transmitting_buffer, fifo_buffer_length);
    D3Serialln("Transmission finished");
    asyncronousTransmissionFlag = false; // all bytes transmitted, end asyncronous transmission
    transmission_finished = true;
    // detachInterrupt(digitalPinToInterrupt(AUX));
    fifo_buffer_pointer += fifo_buffer_length;
    fifo_buffer_length = 0;
  } else{ // send next 58 bytes, wait for finishing and send the rest
    D3Serialln("Transmitting packet");
    write(transmitting_buffer, 58);
    fifo_buffer_pointer += 58;
    fifo_buffer_length -= 58;
  }
  if(fifo_buffer_pointer==fifo_buffer_max_length){
    fifo_buffer_pointer = 0;
  }
  D3Serialln(fifo_buffer_length);
  D3Serialln(fifo_buffer_pointer);
  D3Serialln("");
  D3Serialln("");
}

void write_to_fifo(uint8_t* buffer, unsigned int size){
  unsigned int k = 0;
  if(write_fifo_buffer_pointer+size <= fifo_buffer_max_length){
    while(k < size){
      ((uint8_t*)fifo_buffer)[write_fifo_buffer_pointer++] = buffer[k++];
    }
    if(write_fifo_buffer_pointer==fifo_buffer_max_length){
      write_fifo_buffer_pointer = 0;
    }
  } else {
    while(write_fifo_buffer_pointer < fifo_buffer_max_length){
      ((uint8_t*)fifo_buffer)[write_fifo_buffer_pointer++] = buffer[k++];
    }
    write_fifo_buffer_pointer = 0;
    while(k < size){
      ((uint8_t*)fifo_buffer)[write_fifo_buffer_pointer++] = buffer[k++];
    }
  }
  fifo_buffer_length += size;
}

// void write_to_fifo(uint8_t byte){
//   ((uint8_t*)fifo_buffer)[write_fifo_buffer_pointer++] = byte;
// }

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
  auxHighFlag = false;
  auxLowFlag = false;
  auxHighFlag = false;
  auxLowFlag = false;
  DSerial("Writing: ");
  ON_DEBUG(printHEX(buffer, size);)
  waitForAuxReady();
  e32serial.write(buffer, size);
  waitForAuxReady();
}

void write(uint8_t byte){
  while(asyncronousTransmissionFlag);
  auxHighFlag = false;
  auxLowFlag = false;
  // DSerial("Writing: ");
  // ON_DEBUG(printHEX(byte);)
  waitForAuxReady();
  e32serial.write(byte);
  waitForAuxReady();
}

void asyncronousWrite(uint8_t* buffer, unsigned int size){
  while(asyncronousTransmissionFlag);
  auxHighFlag = false;
  auxLowFlag = false;
  if(size < 58){ // If message is too small, simply write it
    // if(asyncronousTransmissionFlag){
    //   write_to_fifo(buffer, size);
    // } else{
      write(buffer, size);
    // }
  } else{ // If message is long, more packets are required, turn on asynchronous transmission
    attachInterrupt(digitalPinToInterrupt(AUX), asyncronousTransmissionCallback, RISING);
    D3Serialln("writing to fifo");
    write_to_fifo(&buffer[58], size);
    D3Serialln("Setting async flag");
    asyncronousTransmissionFlag = true;
    D3Serialln("First transmission"); // Send first 58 bytes, then the interrupts will ensure the rest is sent
    write(buffer, 58);
  //   noInterrupts();
  //   asyncronousTransmissionCallback();
  //   interrupts();
  }
}

// void asyncronousWrite(uint8_t byte){
//   while(asyncronousTransmissionFlag);
//   auxHighFlag = false;
//   auxLowFlag = false;
//   // if(asyncronousTransmissionFlag){
//   //   write_to_fifo(byte);
//   // } else{
//   //   write(byte);
//   // }
//   write(byte);
// }

void writeFixedTransmission(uint8_t ADDH, uint8_t ADDL, uint8_t CHAN, uint8_t* buffer, unsigned int size){
  while(asyncronousTransmissionFlag);
  auxHighFlag = false;
  auxLowFlag = false;
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
  while(asyncronousTransmissionFlag);
  auxHighFlag = false;
  auxLowFlag = false;
  uint8_t transmitting_buffer[4];
  transmitting_buffer[0] = ADDH;
  transmitting_buffer[1] = ADDL;
  transmitting_buffer[2] = CHAN;
  transmitting_buffer[3] = byte;
  write(transmitting_buffer, 4);
}

void asyncronouswriteFixedTransmission(uint8_t ADDH, uint8_t ADDL, uint8_t CHAN, uint8_t* buffer, unsigned int size){
  while(asyncronousTransmissionFlag);
  auxHighFlag = false;
  auxLowFlag = false;

}

// void asyncronouswriteFixedTransmission(uint8_t ADDH, uint8_t ADDL, uint8_t CHAN, uint8_t byte){
//   while(asyncronousTransmissionFlag);
//   auxHighFlag = false;
//   auxLowFlag = false;
//   uint8_t transmitting_buffer[4];
//   transmitting_buffer[0] = ADDH;
//   transmitting_buffer[1] = ADDL;
//   transmitting_buffer[2] = CHAN;
//   transmitting_buffer[3] = byte;
//   write_to_fifo(transmitting_buffer, 4);
// }

void read(uint8_t* buffer, unsigned int size){
  unsigned int counter = 0;
  D2Serial("Reading: ");
  while(counter<size){
    if(e32serial.available()){
      buffer[counter++] = (uint8_t)e32serial.read();
      DSerial(buffer[counter-1], HEX);
      DSerial(" ");
    }
  }
  D2Serialln("");
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