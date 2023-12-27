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
