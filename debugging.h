#ifndef DEBUGGING_H
#define DEBUGGING_H

extern long unsigned int ti;

void tic();
void toc();
String input(const char* message);
void printHEX(uint8_t* buffer, unsigned int size);

#endif