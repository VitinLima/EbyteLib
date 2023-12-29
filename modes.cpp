#include "Arduino.h"
#include "modes.h"

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

void setSleepMode(){
  if(current_operation_mode == SLEEP) return;

  waitForAuxReady();
  DSerialln("Setting module in sleep mode");
  digitalWrite(M0, HIGH);
  digitalWrite(M1, HIGH);

  waitForAuxReady();
  e32serial.begin(9600);
  current_operation_mode = SLEEP;
}

void setNormalMode(){
  if(current_operation_mode == NORMAL) return;

  waitForAuxReady();
  DSerialln("Setting module in normal mode");
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);

  waitForAuxReady();
  e32serial.begin(getBaudRate());
  current_operation_mode = NORMAL;
}

void setWakeUpMode(){
  if(current_operation_mode == WAKE_UP) return;

  waitForAuxReady();
  DSerialln("Setting module in wake up mode");
  digitalWrite(M0, LOW);
  digitalWrite(M1, HIGH);

  waitForAuxReady();
  e32serial.begin(getBaudRate());
  current_operation_mode = WAKE_UP;
}

void setPowerSavingMode(){
  if(current_operation_mode == POWER_SAVING) return;

  waitForAuxReady();
  DSerialln("Setting module in power saving mode");
  digitalWrite(M0, HIGH);
  digitalWrite(M1, LOW);

  waitForAuxReady();
  e32serial.begin(getBaudRate());
  current_operation_mode = POWER_SAVING;
}

OperationMode getOperationMode(){
  return current_operation_mode;
}

void setOperationMode(OperationMode op){
  switch(op){
    case NORMAL:
      setNormalMode();
      break;
    case WAKE_UP:
      setNormalMode();
      break;
    case POWER_SAVING:
      setNormalMode();
      break;
    case SLEEP:
      setNormalMode();
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