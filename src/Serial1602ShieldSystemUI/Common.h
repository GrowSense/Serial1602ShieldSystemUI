#ifndef COMMON_H_
#define COMMON_H_

#define VERSION "1-0-0-1"
#define BOARD_TYPE "uno"

extern const int ANALOG_MAX;

extern bool isDebugMode;

extern long loopNumber;

void serialPrintLoopHeader();
void serialPrintLoopFooter();

int readInt(char* text, int startPosition, int digitCount);

unsigned long secondsToMilliseconds(int seconds);
float millisecondsToSecondsWithDecimal(int milliseconds);

void forceSerialOutput();

#endif
/* COMMON_H_ */
