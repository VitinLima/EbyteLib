#include "Arduino.h"
#include "EbyteLib.h"

bool receivingFlag = false;
extern bool writing_to_device = false;
bool transmission_finished = false;
bool transmission_started = false;

void waitForAuxReady(){
  DSerial("Waiting for aux ready");
  while(!digitalRead(AUX));
  delay(5);
  DSerial("Done");
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
  DSerial("Reading configuration");
  readConfiguration();
  DSerial("done");
  waitForAuxReady();

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
  // delay(1000);
  DSerial("Module has been reset successfully");
}

// void auxChangeISR(){
//   // DSerial("Aux change");
// }

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
  switch(current_operation_mode){
    case NORMAL:
      if(asyncronousTransmissionFlag){
        asyncronousTransmissionCallback();
      } else if(transmission_started){
        transmission_finished;
      }else{
        attachInterrupt(digitalPinToInterrupt(AUX), auxFallingISR, FALLING);
      }
      break;
    case SLEEP:
      attachInterrupt(digitalPinToInterrupt(AUX), auxFallingISR, FALLING);
      break;
  }
}
void auxFallingISR(){
  DSerialln("Aux falling");
  auxLowFlag = true;
  switch(current_operation_mode){
    case NORMAL:
      attachInterrupt(digitalPinToInterrupt(AUX), auxRisingISR, RISING);
      break;
    case SLEEP:
      attachInterrupt(digitalPinToInterrupt(AUX), auxFallingISR, FALLING);
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

// void onAuxHigh(){
//   DSerial("Aux High");
//   auxHighInterrupt = true;
// }

// void onAuxLow(){
//   DSerial("Aux Low");
//   auxLowInterrupt = true;
// }
