#include <duinocom2.h>
#include <SerialOutput.h>
#include <DeviceName.h>
#include "Commands.h"
#include "DisplayShield.h"

bool isDebug = false;

bool didChange = false;

void setup()
{
  Serial.begin(9600);
  
  Serial.println("Serial GrowSense System UI");
  Serial.println("Starting...");
  
  loadDeviceNameFromEEPROM();
  
  serialPrintDeviceInfo();
   
  lcd.begin(16, 2);
   
  lcd.print("GrowSense");
  delay(1000);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Waiting to");
  lcd.setCursor(0,1);
  lcd.print("connect...");
}
 
void loop()
{ 
  handleButtons();
  
  checkCommand();
  
  serialPrintData();
  
  delay(1);
}
