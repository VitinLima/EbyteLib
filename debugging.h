#ifndef DEBUGGING_H
#define DEBUGGING_H

// #define DEBUG_ON
// #define DEBUG2_ON
// #define DEBUG3_ON
// #define DEBUG4_ON

#define GET_MACRO(_1,_2,NAME,...) NAME
#ifdef DEBUG_ON
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

#ifdef DEBUG2_ON
#define D2Serial(...) GET_MACRO(__VA_ARGS__, DSerial2, DSerial1)(__VA_ARGS__)
#define D2Serialln(...) GET_MACRO(__VA_ARGS__, DSerialln2, DSerialln1)(__VA_ARGS__)
#define ON_DEBUG2(x) {x};
#else
#define D2Serial(...)
#define D2Serialln(...)
#define ON_DEBUG2(x)
#endif

#ifdef DEBUG3_ON
#define D3Serial(...) GET_MACRO(__VA_ARGS__, DSerial2, DSerial1)(__VA_ARGS__)
#define D3Serialln(...) GET_MACRO(__VA_ARGS__, DSerialln2, DSerialln1)(__VA_ARGS__)
#define ON_DEBUG3(x) {x};
#else
#define D3Serial(...)
#define D3Serialln(...)
#define ON_DEBUG3(x)
#endif

#ifdef DEBUG4_ON
#define D4Serial(...) GET_MACRO(__VA_ARGS__, DSerial2, DSerial1)(__VA_ARGS__)
#define D4Serialln(...) GET_MACRO(__VA_ARGS__, DSerialln2, DSerialln1)(__VA_ARGS__)
#define ON_DEBUG4(x) {x};
#else
#define D4Serial(...)
#define D4Serialln(...)
#define ON_DEBUG4(x)
#endif

#define DSerial1(x) {Serial.print(x);}
#define DSerial2(x,y) {Serial.print(x,y);}
#define DSerialln1(x) {Serial.println(x);}
#define DSerialln2(x,y) {Serial.println(x,y);}

#ifdef DEBUGGING

extern long unsigned int ti;

void tic();
void toc();
String input(const char* message);
void printHEX(uint8_t* buffer, unsigned int size);

#endif

#endif