#include <Arduino.h>
#include <EEPROM.h>

#include "Common.h"
#include "Commands.h"
#include "DeviceName.h"
#include "SerialOutput.h"
#include "DisplayShield.h"

void checkCommand()
{
  /*if (isDebugMode)
  {
    Serial.println("Checking incoming serial commands");
  }*/

  if (checkMsgReady())
  {
    char* msg = getMsg();
       
    handleCommand(msg);
  }
}

void handleCommand(char* msg)
{
  if (isDebugMode)
  {
    Serial.println("");
    Serial.println("Handling command...");  
  }

  Serial.print("Received message: ");
  Serial.println(msg);
        
  char letter = msg[0];

  Serial.print("Received message: ");
  Serial.println(msg);

  if (isKeyValue(msg))
  {
    Serial.println("  Is key value");
  
    char* key = getKey(msg);
  
    Serial.print("  Key: \"");
    Serial.print(key);
    Serial.println("\"");
  
    char* value = getValue(msg);
  
    Serial.print("  Value: \"");
    Serial.print(value);
    Serial.println("\"");

    if (strcmp(key, "Name") == 0)
    {
      if (isDebugMode)
        Serial.println("  Set device name");
      setDeviceName(value);
    }
  }
  else
  {
    if (letter == '#')
    {
      serialPrintDeviceInfo();
    }
    else if (letter == "Q")
    {
      forceSerialOutput();
    }
    else
    {
      int length = strlen(msg);

      int lineIndex = getLineIndex(msg);
      
      if (isDebugMode)
      {
        Serial.print("Line index: ");
        Serial.println(lineIndex);
      }
      
      int pipePosition = getPipePosition(msg);
      
      char text[16];
      substring(msg, pipePosition+1, strlen(msg)-pipePosition-1, text);

      if (isDebugMode)
      {
        Serial.print("Value: ");
        Serial.println(text);
      }
      
      if (length > 0 && !isWhitespace(text))
      {
        lcd.setCursor(0,lineIndex);
        lcd.print("                    ");
        lcd.setCursor(0,lineIndex);
        lcd.print(text);
      }
    }
  }
  
  forceSerialOutput();
  
  if (isDebugMode)
  {
    Serial.println("");
  }
}

int getLineIndex(char* msg)
{
  int pipePosition = getPipePosition(msg);
  
  int lineIndex = 0;
  
  if (pipePosition > 0)
  {
    lineIndex = readInt(msg, 0, pipePosition+1);
  }
  
  return lineIndex;
}

int getPipePosition(char* msg)
{

  if (isDebugMode)
  {
    Serial.print("Getting pipe position from: ");
    Serial.println(msg);
  }

  int pipePosition = 0;

  unsigned int numElements = strlen(msg);
  unsigned int i;
  for(i = 0; i < numElements; ++i) {
      if (msg[i] == '|') {
          pipePosition = i;
          break;
      }
  }

  return pipePosition;
}

void substring(char msg[], int startIndex, int length, char output[])
{
  //char buffer[15];

  if (isDebugMode)
    Serial.println("Substring");

  int total = 0;
  
  for (int i = 0; i < length; i++)
  {
    output[i] = msg[startIndex+i];

    if (isDebugMode)
      Serial.println(output[i]);
    
    total++;
  }
  
  output[total] = '\0';
}


void restoreDefaultSettings()
{
  Serial.println("Restoring default settings");
  
  EEPROMReset();
}
