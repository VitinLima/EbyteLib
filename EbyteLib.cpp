#include "Arduino.h"
#include "EbyteLib.h"

bool receivingFlag = false;
bool writing_to_device = false;
bool transmission_finished = false;
bool transmission_started = false;

bool reception_started = false;
bool reception_finished = false;

void (*receiveCallback)() = defaultReceiveCallback;
void defaultReceiveCallback(){

}

void waitForAuxReady(){
  DSerial("Waiting for aux ready");
  while(!digitalRead(AUX));
  delay(5);
  DSerial("Done");
}

void initE32(){
  e32serial.begin(9600);

  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(AUX, INPUT_PULLUP);

  digitalWrite(M0, HIGH);
  digitalWrite(M1, HIGH);
  current_operation_mode = SLEEP;

  waitForAuxReady();
  DSerial("Reading configuration");
  readConfiguration();
  DSerial("done");
  waitForAuxReady();

  // attachInterrupt(digitalPinToInterrupt(AUX), auxChangeISR, CHANGE);
  // attachInterrupt(digitalPinToInterrupt(AUX), auxRisingISR, RISING);
  attachInterrupt(digitalPinToInterrupt(AUX), auxFallingISR, FALLING);
  
  DSerial("Module initiated and ready to accept instructions");
}

void resetModule(){
  setSleepMode();

  DSerial("Reseting module");

  e32serial.write(0xC4);
  e32serial.write(0xC4);
  e32serial.write(0xC4);

  if(digitalRead(AUX)){
    DSerial("Module is not resetting, error");
    while(1);
  }

  waitForAuxReady();

  while(digitalRead(AUX));

  waitForAuxReady();
  DSerial("Module has reset successfully");
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

void auxRisingISR(){
  DSerialln("Aux rising");
  auxHighFlag = true;
  attachInterrupt(digitalPinToInterrupt(AUX), auxFallingISR, FALLING);
  switch(current_operation_mode){
    case NORMAL:
      if(asyncronousTransmissionFlag){
        asyncronousTransmissionCallback();
      } else if(transmission_started){
        DSerialln("Transmission finished");
        transmission_finished = true;
      } else if(reception_started){
        reception_finished = true;
        receiveCallback();
      }
      break;
    case WAKE_UP:
      break;
    case POWER_SAVING:
      break;
    case SLEEP:
      break;
  }
}
void auxFallingISR(){
  DSerialln("Aux falling");
  auxLowFlag = true;
  attachInterrupt(digitalPinToInterrupt(AUX), auxRisingISR, RISING);
  switch(current_operation_mode){
    case NORMAL:
      if(writing_to_device){
        DSerialln("Transmission started");
        transmission_started = true;
        writing_to_device = false;
      } else{
        reception_started = true;
      }
      break;
    case WAKE_UP:
      break;
    case POWER_SAVING:
      break;
    case SLEEP:
      break;
  }
}

#ifdef DBG
#undef DBG
#define DSerial(...)
#define DSerialln(...)
#define ON_DEBUG(x)
#define Dinput(x)
#endif

// void auxChangeISR(){
//   // DSerial("Aux change");
// }

// void onAuxHigh(){
//   DSerial("Aux High");
//   auxHighInterrupt = true;
// }

// void onAuxLow(){
//   DSerial("Aux Low");
//   auxLowInterrupt = true;
// }
