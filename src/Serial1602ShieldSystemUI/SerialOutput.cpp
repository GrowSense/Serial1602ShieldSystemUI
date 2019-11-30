#include <Arduino.h>
#include <EEPROM.h>

#include "Common.h"
#include "DeviceName.h"

unsigned long lastSerialOutputTime = 0;
long serialOutputIntervalInSeconds = 5;

void serialPrintDeviceInfo()
{
  Serial.println("");
  Serial.println("-- Start Device Info");
  Serial.println("Family: GrowSense");
  Serial.println("Group: ui");
  Serial.println("Project: Serial1602ShieldSystemUI");
  Serial.print("Device name: ");
  Serial.println(deviceName);
  Serial.print("Board: ");
  Serial.println(BOARD_TYPE);
  Serial.print("Version: ");
  Serial.println(VERSION);
  Serial.println("ScriptCode: ui");
  Serial.println("-- End Device Info");
  Serial.println("");
}

void serialPrintData()
{
  bool isTimeToPrintData = lastSerialOutputTime == 0
   || millis() - lastSerialOutputTime >= secondsToMilliseconds(serialOutputIntervalInSeconds);
   
  if (isTimeToPrintData)
  {
    lastSerialOutputTime = millis();
    
    Serial.print("D;"); // This prefix indicates that the line contains data.
    Serial.print("Name:");
    Serial.print(deviceName);
    Serial.print(";");
    Serial.print("V:");
    Serial.print(VERSION);
    Serial.println(";;");
  }
}

void forceSerialOutput()
{
    // Reset the last serial output time 
    lastSerialOutputTime = 0;//millis()-secondsToMilliseconds(serialOutputIntervalInSeconds);
}
