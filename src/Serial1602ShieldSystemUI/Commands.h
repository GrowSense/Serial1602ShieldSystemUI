#ifndef COMMANDS_H_
#define COMMANDS_H_

#include "Common.h"
#include "duinocom2.h"
#include "EEPROMHelper.h"

void checkCommand();

void handleCommand(char* msg);

int getLineIndex(char* msg);

int getPipePosition(char* msg);

void substring(char msg[], int startIndex, int length, char output[]);

void restoreDefaultSettings();

#endif
