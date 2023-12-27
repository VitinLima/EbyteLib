#include "Arduino.h"
#include "debugging.h"

void tic(){
  ti = millis();
}

void toc(){
  Serial.print("Toc ");
  Serial.println(millis() - ti);
}

String input(const char *message){
  Serial.println(message);
  String in = "";
  char c;
  bool flag = true;
  while(flag){
    while(!Serial.available());
    c = Serial.read();
    if(c=='\n'){
      flag = false;
    } else{
      in += c;
    }
  }
  return in;
}

void printHEX(uint8_t* buffer, unsigned int size){
  for(unsigned int i = 0; i < size; i++){
    Serial.print(buffer[i], HEX);
    Serial.print(" ");
  }
  Serial.println("");
}