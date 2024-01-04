#include "Arduino.h"
#include "Transmission.h"

bool asyncronousTransmissionFlag = false;
bool fifo_buffer_overflow = false;
uint8_t fifo_buffer[FIFO_BUFFER_MAX_LENGTH];
unsigned int fifo_buffer_length = 0;
unsigned int fifo_buffer_pointer = 0;
unsigned int write_fifo_buffer_pointer = 0;
const unsigned int fifo_buffer_max_length = FIFO_BUFFER_MAX_LENGTH;
unsigned int payload_max_length = 58;
uint8_t fifo_helper_buffer[58];

bool hasCRC = false;
uint8_t crcKey[2] = {0xaa, 0x1f};
uint8_t crcCode[2];

bool hasFixedTransmission = false;
uint8_t FT_ADDH_ADDL_CHAN[3];

unsigned int packet_index;

void (*transmitting_function)(uint8_t *buffer, unsigned int size) = transmit;

// #define DBG
#ifdef DBG
#define DSerial(...) GET_MACRO(__VA_ARGS__, DSerial2, DSerial1)(__VA_ARGS__)
#define DSerialln(...) GET_MACRO(__VA_ARGS__, DSerialln2, DSerialln1)(__VA_ARGS__)
#define ON_DEBUG(x) {x};
#define Dinput(x) {input(x);}
#else
#define DSerial(...)
#define DSerialln(...)
#define ON_DEBUG(x)
#define Dinput(x)
#endif

void asyncronousTransmissionCallback(){
  uint8_t *transmitting_buffer;
  DSerialln(fifo_buffer_length);
  DSerialln(fifo_buffer_pointer);
  DSerialln(fifo_buffer_max_length);
  DSerialln("");
  unsigned int r; // Current packet size
  bool last_packet;
  if(fifo_buffer_length <= payload_max_length){
    r = fifo_buffer_length;
    last_packet = true;
  } else{
    r = payload_max_length;
    last_packet = false;
  }
  if(fifo_buffer_pointer+r <= fifo_buffer_max_length){ // faster
    DSerialln("Transmitting buffer fifo");
    transmitting_buffer = (uint8_t*)&fifo_buffer[fifo_buffer_pointer];
    fifo_buffer_pointer += r;
  } else{ // use helper fifo, have to copy contents, slightly slower
    DSerialln("Transmitting buffer helper");
    transmitting_buffer = (uint8_t*)fifo_helper_buffer;
    uint8_t k = 0;
    // unsigned int i = ;
    while(fifo_buffer_pointer < fifo_buffer_max_length){
      DSerial(k);
      DSerial(" ");
      DSerial(fifo_buffer_pointer);
      DSerial(" ");
      DSerial(fifo_buffer[fifo_buffer_pointer]);
      DSerial(" ");
      DSerialln(r);
      fifo_helper_buffer[k++] = fifo_buffer[fifo_buffer_pointer++];
    }
    DSerialln("Second part");
    fifo_buffer_pointer = 0;
    while(k < r){
      DSerial(k);
      DSerial(" ");
      DSerial(fifo_buffer_pointer);
      DSerial(" ");
      DSerial(fifo_buffer[fifo_buffer_pointer]);
      DSerial(" ");
      DSerialln(r);
      fifo_helper_buffer[k++] = fifo_buffer[fifo_buffer_pointer++];
    }
  }
  write(transmitting_buffer, r);
  fifo_buffer_length -= r;
  if(last_packet){ // can send all remaining bytes together
    DSerialln("Last packet");
    asyncronousTransmissionFlag = false; // all bytes transmitted, end asyncronous transmission
    DSerialln("Asynchronous transmission finished");
  } else{ // send next payload_max_length bytes, wait for finishing and send the rest
    DSerialln("Transmitting packet");
  }
  if(fifo_buffer_pointer==fifo_buffer_max_length){
    fifo_buffer_pointer = 0;
  }
  DSerialln(fifo_buffer_length);
  DSerialln(fifo_buffer_pointer);
  DSerialln("");
  DSerialln("");
}

void write_to_fifo(uint8_t* buffer, unsigned int size){
  unsigned int k = 0;
  DSerialln("Writing to fifo");
  if(write_fifo_buffer_pointer+size <= fifo_buffer_max_length){
    while(k < size){
      fifo_buffer[write_fifo_buffer_pointer++] = buffer[k++];
      DSerialln(fifo_buffer[write_fifo_buffer_pointer-1], HEX);
    }
    if(write_fifo_buffer_pointer==fifo_buffer_max_length){
      write_fifo_buffer_pointer = 0;
    }
  } else {
    while(write_fifo_buffer_pointer < fifo_buffer_max_length){
      fifo_buffer[write_fifo_buffer_pointer++] = buffer[k++];
      DSerialln(fifo_buffer[write_fifo_buffer_pointer-1], HEX);
    }
    DSerialln("Second part");
    write_fifo_buffer_pointer = 0;
    while(k < size){
      fifo_buffer[write_fifo_buffer_pointer++] = buffer[k++];
      DSerialln(fifo_buffer[write_fifo_buffer_pointer-1], HEX);
    }
  }
  fifo_buffer_length += size;
}

#ifdef DBG
#undef DBG
#define DSerial(...)
#define DSerialln(...)
#define ON_DEBUG(x)
#define Dinput(x)
#endif

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

// #define DBG
#ifdef DBG
#define DSerial(...) GET_MACRO(__VA_ARGS__, DSerial2, DSerial1)(__VA_ARGS__)
#define DSerialln(...) GET_MACRO(__VA_ARGS__, DSerialln2, DSerialln1)(__VA_ARGS__)
#define ON_DEBUG(x) {x};
#define Dinput(x) {input(x);}
#else
#define DSerial(...)
#define DSerialln(...)
#define ON_DEBUG(x)
#define Dinput(x)
#endif

void write(uint8_t* buffer, unsigned int size){
  auxHighFlag = false;
  auxLowFlag = false;
  if(size <= payload_max_length){
    transmitting_function(buffer, size);
  } else{
    // DSerial("En écrivant: ");
    // ON_DEBUG(printHEX(buffer, payload_max_length);)
    transmitting_function(buffer, payload_max_length);
    unsigned int r;
    for(int i = payload_max_length; i < size; i+=payload_max_length){
      r = size - i;
      if(r > payload_max_length){
        r = payload_max_length;
      }
      DSerialln("En attendant le début de l'émission radio");
      if(!waitForTimeout(&transmission_started, 50)){
        DSerialln("Cela prend trop de temp pour démarrer le streaming");
      }
      DSerialln("En attendant le fin de l'émission radio");
      if(!waitForTimeout(&transmission_finished, 20 + r/getAirDataRate())){
        DSerialln("Cela prend trop de temp pour terminer le streaming");
      }
      // DSerial("En écrivant: ");
      // ON_DEBUG(printHEX(&(buffer[i]), r);)
      transmitting_function(&(buffer[i]), r);
    }
  }
}

#ifdef DBG
#undef DBG
#define DSerial(...)
#define DSerialln(...)
#define ON_DEBUG(x)
#define Dinput(x)
#endif

// #define DBG
#ifdef DBG
#define DSerial(...) GET_MACRO(__VA_ARGS__, DSerial2, DSerial1)(__VA_ARGS__)
#define DSerialln(...) GET_MACRO(__VA_ARGS__, DSerialln2, DSerialln1)(__VA_ARGS__)
#define ON_DEBUG(x) {x};
#define Dinput(x) {input(x);}
#else
#define DSerial(...)
#define DSerialln(...)
#define ON_DEBUG(x)
#define Dinput(x)
#endif

void transmit(uint8_t *buffer, unsigned int size){
  writing_to_device = true;
  transmission_started = false;
  transmission_finished = false;
  DSerial("En écrivant: ");
  ON_DEBUG(printHEX(buffer, size);)
  waitForAuxReady();
  e32serial.write(buffer, size);
}

void FTtransmit(uint8_t *buffer, unsigned int size){
  writing_to_device = true;
  transmission_started = false;
  transmission_finished = false;
  DSerial("FT En écrivant: ");
  ON_DEBUG(printHEX(FT_ADDH_ADDL_CHAN, 3);)
  ON_DEBUG(printHEX(buffer, size);)
  DSerialln(size);
  waitForAuxReady();
  e32serial.write(FT_ADDH_ADDL_CHAN, 3);
  e32serial.write(buffer, size);
}

void transmitCRC(uint8_t *buffer, unsigned int size){
  writing_to_device = true;
  transmission_started = false;
  transmission_finished = false;
  DSerial("CRC En écrivant: ");
  ON_DEBUG(printHEX(buffer, size);)
  ON_DEBUG(printHEX(crcCode, 2);)
  computeCRC(buffer, size);
  waitForAuxReady();
  e32serial.write(buffer, size);
  e32serial.write(crcCode, 2);
}

void FTtransmitCRC(uint8_t *buffer, unsigned int size){
  writing_to_device = true;
  transmission_started = false;
  transmission_finished = false;
  DSerial("FT-CRC En écrivant: ");
  ON_DEBUG(printHEX(FT_ADDH_ADDL_CHAN, 3);)
  ON_DEBUG(printHEX(buffer, size);)
  ON_DEBUG(printHEX(crcCode, 2);)
  computeCRC(buffer, size);
  waitForAuxReady();
  e32serial.write(FT_ADDH_ADDL_CHAN, 3);
  e32serial.write(buffer, size);
  e32serial.write(crcCode, 2);
}

#ifdef DBG
#undef DBG
#define DSerial(...)
#define DSerialln(...)
#define ON_DEBUG(x)
#define Dinput(x)
#endif

// void write(uint8_t byte){
//   while(asyncronousTransmissionFlag);
//   auxHighFlag = false;
//   auxLowFlag = false;
//   waitForAuxReady();
//   e32serial.write(byte);
//   waitForAuxReady();
// }

void asynchronousWrite(uint8_t* buffer, unsigned int size){
  while(asyncronousTransmissionFlag);
  if(size <= payload_max_length){ // If message is too small, simply write it
    transmitting_function(buffer, size);
  } else{ // If message is long, more packets are required, turn on asynchronous transmission
    DSerialln("Starting asynchronous transmission");
    DSerialln("writing to fifo");
    DSerialln(size-payload_max_length);
    write_to_fifo(&buffer[payload_max_length], size-payload_max_length);
    DSerialln("Setting async flag");
    asyncronousTransmissionFlag = true;
    DSerialln("First transmission"); // Send first payload_max_length bytes, then the interrupts will ensure the rest is sent
    transmitting_function(buffer, payload_max_length);
  }
}

// void asynchronousWrite(uint8_t byte){
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
  FT_ADDH_ADDL_CHAN[0] = ADDH;
  FT_ADDH_ADDL_CHAN[1] = ADDL;
  FT_ADDH_ADDL_CHAN[2] = CHAN;
  write(buffer, size);
}

// void writeFixedTransmission(uint8_t ADDH, uint8_t ADDL, uint8_t CHAN, uint8_t byte){
//   while(asyncronousTransmissionFlag);
//   FT_ADDH_ADDL_CHAN[0] = ADDH;
//   FT_ADDH_ADDL_CHAN[1] = ADDL;
//   FT_ADDH_ADDL_CHAN[2] = CHAN;
//   write(byte);
// }

void asynchronousWriteFixedTransmission(uint8_t ADDH, uint8_t ADDL, uint8_t CHAN, uint8_t* buffer, unsigned int size){
  while(asyncronousTransmissionFlag);
  FT_ADDH_ADDL_CHAN[0] = ADDH;
  FT_ADDH_ADDL_CHAN[1] = ADDL;
  FT_ADDH_ADDL_CHAN[2] = CHAN;
  asynchronousWrite(buffer, size);
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

bool read(uint8_t* buffer, unsigned int size){
  return read(buffer, size, 50);
}

bool read(uint8_t* buffer, unsigned int size, unsigned long int timeout){
  long unsigned int timeout_limit = millis() + timeout;
  unsigned int counter = 0;
  D2Serial("Reading: ");
  while(millis() < timeout_limit){
    if(e32serial.available()){
      buffer[counter++] = (uint8_t)e32serial.read();
      DSerial(buffer[counter-1], HEX);
      DSerial(" ");
    }
    if(counter==size){
      return true;
    }
  }
  D2Serialln("");
  return false;
}

bool getTransmissionResult(unsigned long int timeout){
  long unsigned int timeout_limit = millis() + timeout;
  while(millis() < timeout_limit){
    if(transmission_started && transmission_finished && !asyncronousTransmissionFlag){
      return true;
    }
  }
  return false;
}

bool computeCRC(uint8_t *buffer, unsigned int size){ // TODO
  crcCode[0] = 0x00;
  crcCode[1] = 0x00;
  return true;
}

unsigned long int estimateTimeOfTransmission(unsigned long int size){
  return size / getAirDataRate();
}