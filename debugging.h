#ifndef DEBUGGING_H
#define DEBUGGING_H

#define DEBUG_ON

#define GET_MACRO(_1,_2,NAME,...) NAME
#define DSerial(...) GET_MACRO(__VA_ARGS__, DSerial2, DSerial1)(__VA_ARGS__)
#define DSerialln(...) GET_MACRO(__VA_ARGS__, DSerialln2, DSerialln1)(__VA_ARGS__)

#ifdef DEBUG_ON
#define DSerial1(x) {Serial.print(x);}
#define DSerial2(x,y) {Serial.print(x,y);}
#define DSerialln1(x) {Serial.println(x);}
#define DSerialln2(x,y) {Serial.println(x,y);}
#else
#define DSerial1(x)
#define DSerial2(x)
#define DSerialln1(x)
#define DSerialln2(x)
#endif

extern long unsigned int ti;

void tic();
void toc();
String input(const char* message);
void printHEX(uint8_t* buffer, unsigned int size);

#endif