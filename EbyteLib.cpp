#include "Arduino.h"
#include "EbyteLib.h"

void waitForAuxReady(){
  delay(50);
  while(!digitalRead(AUX));
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

void auxChangeISR(){
  // DSerial("Aux change");
}

void auxRisingISR(){
  // DSerial("Aux rising");
  auxHighFlag = true;
  attachInterrupt(digitalPinToInterrupt(AUX), auxFallingISR, FALLING);
  // if(asyncronousTransmissionFlag){
  //   asyncronousTransmissionCallback();
  // }
}
void auxFallingISR(){
  // DSerial("Aux falling");
  auxLowFlag = true;
  attachInterrupt(digitalPinToInterrupt(AUX), auxRisingISR, RISING);
  // while(!digitalRead(AUX));
  // DSerial("Aux rising");
}

// void onAuxHigh(){
//   DSerial("Aux High");
//   auxHighInterrupt = true;
// }

// void onAuxLow(){
//   DSerial("Aux Low");
//   auxLowInterrupt = true;
// }
