#include "Arduino.h"
#include "modes.h"

void setSleepMode(){
  if(current_operation_mode == SLEEP) return;

  DSerial("Setting module in sleep mode");
  digitalWrite(M0, HIGH);
  digitalWrite(M1, HIGH);

  waitForAuxReady();
  current_operation_mode = SLEEP;
}

void setNormalMode(){
  if(current_operation_mode == NORMAL) return;

  DSerial("Setting module in normal mode");
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);

  waitForAuxReady();
  current_operation_mode = NORMAL;
}

void setWakeUpMode(){
  if(current_operation_mode == WAKE_UP) return;

  DSerial("Setting module in wake up mode");
  digitalWrite(M0, LOW);
  digitalWrite(M1, HIGH);

  waitForAuxReady();
  current_operation_mode = WAKE_UP;
}

void setPowerSavingMode(){
  if(current_operation_mode == POWER_SAVING) return;

  DSerial("Setting module in power saving mode");
  digitalWrite(M0, HIGH);
  digitalWrite(M1, LOW);

  waitForAuxReady();
  current_operation_mode = POWER_SAVING;
}