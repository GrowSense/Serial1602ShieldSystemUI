#include <LiquidCrystal.h>
#include <duinocom.h>

bool isDebug = false;

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int lcd_key     = 0;
int adc_key_in  = 0;

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

long lastButtonPress = 0;
long buttonInterval = 200;

bool didChange = false;

#define VERSION "1-0-0-1"
#define BOARD_TYPE "uno"

unsigned long lastSerialDataTime = 0;
long serialDataInterval = 5 * 1000;

void setup()
{
  Serial.begin(9600);
  
  Serial.println("Serial GrowSense System UI");
  Serial.println("Starting...");
  
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
  
  handleMsg();
  
  serialPrintData();
  
  delay(1);
}

void serialPrintDeviceInfo()
{
  Serial.println("");
  Serial.println("-- Start Device Info");
  Serial.println("Family: GrowSense");
  Serial.println("Group: ui");
  Serial.println("Project: Serial1602ShieldSystemUI");
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
  bool isTimeToPrintData = lastSerialDataTime == 0
   || millis() - lastSerialDataTime >= serialDataInterval;
  
  if (isTimeToPrintData)
  {
    lastSerialDataTime = millis();
    Serial.print("D;");
    Serial.print("V:");
    Serial.print(VERSION);
    Serial.println(";;");
  }
}

void handleButtons()
{
  // Buttons are temporarily disabled to avoid duplicate presses
  bool buttonsAreAvailable = lastButtonPress + buttonInterval < millis();
  
  if (buttonsAreAvailable)
  {
    lcd_key = read_LCD_buttons();  // read the buttons

    handleButton(lcd_key);
    
    lastButtonPress = millis();
  }
}

int read_LCD_buttons()
{
  adc_key_in = analogRead(0);      // read the value from the sensor 
 
  //Serial.println(adc_key_in);
 
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 
 // For V1.1 us this threshold
/* if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 250)  return btnUP; 
 if (adc_key_in < 450)  return btnDOWN; 
 if (adc_key_in < 650)  return btnLEFT; 
 if (adc_key_in < 850)  return btnSELECT; */

 // For V1.0 comment the other threshold and use the one below:

 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 195)  return btnUP; 
 if (adc_key_in < 380)  return btnDOWN; 
 if (adc_key_in < 555)  return btnLEFT; 
 if (adc_key_in < 790)  return btnSELECT;   



 return btnNONE;  // when all others fail, return this...
}

void handleMsg()
{
  if (checkMsgReady())
  {
    char* msg = getMsg();
        
    char letter = msg[0];

    Serial.print("Received message: ");
    Serial.println(msg);
      
    if (letter == '#')
    {
      serialPrintDeviceInfo();
    }
    else if (letter == "Q")
    {
      lastSerialDataTime = 0;
    }
    else
    {
      int length = strlen(msg);

      int lineIndex = getLineIndex(msg);
      
      if (isDebug)
      {
        Serial.print("Line index: ");
        Serial.println(lineIndex);
      }
      
      int pipePosition = getPipePosition(msg);
      
      char text[16];
      substring(msg, pipePosition+1, strlen(msg)-pipePosition-1, text);

      if (isDebug)
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
}

int getPipePosition(char* msg)
{

  if (isDebug)
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

void handleButton(char lcd_key)
{
 switch (lcd_key)
 {
   case btnRIGHT:
   {
      Serial.println("right");
      break;
   }
   case btnLEFT:
   {
      Serial.println("left");
     break;
   }
   case btnUP:
   {
      Serial.println("up");
     break;
   }
   case btnDOWN:
   {
      Serial.println("down");
     break;
   }
   case btnSELECT:
   {
      Serial.println("select");
     break;
   }
   case btnNONE:
   {
     break;
   }
 }
}

void substring(char msg[], int startIndex, int length, char output[])
{
  //char buffer[15];

  if (isDebug)
    Serial.println("Substring");

  int total = 0;
  
  for (int i = 0; i < length; i++)
  {
    output[i] = msg[startIndex+i];

    if (isDebug)
      Serial.println(output[i]);
    
    total++;
  }
  
  output[total] = '\0';
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
