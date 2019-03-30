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

void setup()
{
  Serial.begin(9600);
  
  Serial.println("Serial GreenSense System UI");
   
  lcd.begin(16, 2);              // start the library
   
  lcd.write("Loading...");
}
 
void loop()
{ 
  handleButtons();
  
  handleMsg();

  //drawMenu();
  
  delay(1);
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

    int length = strlen(msg);

    Serial.print("Received message: ");
    Serial.println(msg);
    
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
    
    if (length > 0)
    {
      lcd.setCursor(0,lineIndex);
      lcd.print("                    ");
      lcd.setCursor(0,lineIndex);
      lcd.print(text);
    }
  }
}

/*void getValue(char msg[], char value[])
{
  if (isDebug)
  {
    Serial.println("Getting value");
  }

  int pipePosition = getPipePosition(msg);

  if (isDebug)
  {
    Serial.print("  Pipe position: ");
    Serial.println(pipePosition);
  }

  if (pipePosition > 0)
  {
    int valueLength = strlen(msg)-pipePosition-1;

    if (isDebug)
    {
      Serial.print("  Value length: ");
      Serial.println(valueLength);
    }

    int valueStartPosition = pipePosition+1;

    if (isDebug)
    {
      Serial.print("  Value start position: ");
      Serial.println(valueStartPosition);
    }

    substring(msg, valueStartPosition, valueLength, value);
    
  }
  else
  {
    value = msg;
  }
  
  if (isDebug)
  {
    Serial.print("  Value: ");
    Serial.println(value);

    Serial.println("Finished getting value");
    Serial.println("");
  }
}*/

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

//  output = buffer;
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

/*
void sendThresholdCommand()
{
  
  Serial.println("Sending threshold command");

  String cmd = "T";
  cmd = cmd + soilMoistureThreshold;
  
  Serial.println(cmd);
  
  Serial.println(cmd);
  
  lcd.setCursor(0, 1);
  lcd.write("              ->");
  
  clearSerialBuffer();
}

void decreaseThreshold()
{
  soilMoistureThreshold--;
  
  didChange = true;
}

void increaseThreshold()
{
  soilMoistureThreshold++;
  
  didChange = true;
}

void sendPumpBurstOnDurationCommand()
{
  Serial.println("Sending pump burst on time command");

  String cmd = "B";
  cmd = cmd + pumpBurstOnDuration;
  
  Serial.println(cmd);
  
  Serial.println(cmd);
  
  lcd.setCursor(0, 1);
  lcd.write("              ->");
}

void decreasePumpBurstOnDuration()
{
  pumpBurstOnDuration--;
  
  didChange = true;
}

void increasePumpBurstOnDuration()
{
  pumpBurstOnDuration++;
  
  didChange = true;
}

void sendPumpBurstOffDurationCommand()
{
  
  Serial.println("Sending pump burst off time command");

  String cmd = "O";
  cmd = cmd + pumpBurstOffDuration;
  
  Serial.println(cmd);
  
  Serial.println(cmd);
  
  lcd.setCursor(0, 1);
  lcd.write("              ->");
}

void decreasePumpBurstOffDuration()
{
  pumpBurstOffDuration--;
  
  didChange = true;
}

void increasePumpBurstOffDuration()
{
  pumpBurstOffDuration++;
  
  didChange = true;
}

void sendReadingIntervalCommand()
{
  
  Serial.println("Sending reading interval command");

  String cmd = "V";
  cmd = cmd + readingInterval;
  
  Serial.println(cmd);
  
  Serial.println(cmd);
  
  lcd.setCursor(0, 1);
  lcd.write("              ->");
}

void decreaseReadingInterval()
{
  readingInterval--;
  
  didChange = true;
}

void increaseReadingInterval()
{
  readingInterval++;
  
  didChange = true;
}

void sendPumpStatusCommand()
{
  
  Serial.println("Sending pump status command");

  String cmd = "P";
  cmd = cmd + pumpStatus;
  
  Serial.println(cmd);
  
  Serial.println(cmd);
  
  lcd.setCursor(0, 1);
  lcd.write("              ->");
}

void pumpLeftOption()
{
  if (pumpStatus <= 0)
    pumpStatus = 2;
  else
    pumpStatus--;
  
  Serial.print("Pump status: ");
  Serial.println(pumpStatus);
  
  didChange = true;
}

void pumpUpOption()
{
  if (pumpStatus >= 2)
    pumpStatus = 0;
  else
    pumpStatus++;
    
  Serial.print("Pump status: ");
  Serial.println(pumpStatus);
  
  didChange = true;
}

void clearSerialBuffer()
{
  Serial.println("Clearing serial buffer");
  
  while(Serial.available() > 0) {
    char t = Serial.read();
  }
}

void parseData(char* data)
{
  if (isDebug)
  {
    Serial.println("Parsing data...");
    Serial.println(data);
	}

  int colonPosition = getColonPosition(data);


  int keyLength = colonPosition;

  if (isDebug)
  {
    Serial.print("Key length: ");
    Serial.println(keyLength);
  }

  char key[10];
  
  // Figure out a cleaner way to do this
  for (int i = 0; i < 10; i++)
  {
    if (i < keyLength)
      key[i] = data[i];
    else
      key[i] = '\0';
  }


  if (isDebug)
  {
    Serial.print("Key: ");
    Serial.println(key);
  }
  
	int value = getValue(data);

  if (isDebug)
  {
    Serial.print("Value: ");
    Serial.println(value);
  }

	setData(key, value);

  if (isDebug)
  {
    Serial.println();
    Serial.println();
	    
	}
}

void setData(char* key, int value)
{
	Serial.println("Setting data...");

  if (isDebug)
  {
    Serial.print("Key: ");
    Serial.println(key);
    
    Serial.print("Value: ");
    Serial.println(value);
  }


	if (strcmp(key, "C") == 0)
	{
			Serial.print("Soil moisture value (incoming):");
			Serial.println(value);
 
      if (soilMoistureValue != value)
      {
        soilMoistureValue = value;
      
        didChange = true;
      }
	}
	if (strcmp(key, "T") == 0)
	{
			Serial.print("Soil moisture threshold (incoming):");
			Serial.println(value);
 
      if (soilMoistureThreshold != value)
      {
        soilMoistureThreshold = value;
      
        didChange = true;
      }
	}
	if (strcmp(key, "V") == 0)
	{
			Serial.print("Reading interval (incoming):");
			Serial.println(value);
 
      if (readingInterval != value)
      {
        readingInterval = value;
      
        didChange = true;
      }
	}
	if (strcmp(key, "B") == 0)
	{
			Serial.print("Pump burst on duration (incoming):");
			Serial.println(value);
 
      if (pumpBurstOnDuration != value)
      {
        pumpBurstOnDuration = value;
      
        didChange = true;
      }
	}
	if (strcmp(key, "O") == 0)
	{
			Serial.print("Pump burst off duration (incoming):");
			Serial.println(value);
 
      if (pumpBurstOffDuration != value)
      {
        pumpBurstOffDuration = value;
      
        didChange = true;
      }
	}
	if (strcmp(key, "P") == 0)
	{
			Serial.print("Pump status (incoming):");
			Serial.println(value);
 
      if (pumpStatus != value)
      {
        pumpStatus = value;
      
        didChange = true;
      }
	}
}

char* getKey(char* msg)
{

  if (isDebug)
  {
   Serial.println("Getting key");
   Serial.println(msg);
  }

  int colonPosition = getColonPosition(msg);

  if (isDebug)
  {
    Serial.print("  Colon position: ");
    Serial.println(colonPosition);
  }

  int keyLength = colonPosition;

  if (isDebug)
  {
    Serial.print("  Key length: ");
    Serial.println(keyLength);
  }

  char key[5];
  strncpy(key, msg, keyLength);
  
  if (isDebug)
  {
    Serial.print("  Key: ");
    Serial.println(key);

    Serial.println("Finished getting key");
    Serial.println("");
  }

  return key;
}


int getValue(char* msg)
{

  if (isDebug)
  {
    Serial.println("Getting value");
  }

  int colonPosition = getColonPosition(msg);

  if (isDebug)
  {
    Serial.print("  Colon position: ");
    Serial.println(colonPosition);
  }

  int valueLength = strlen(msg)-colonPosition-1;

  if (isDebug)
  {
    Serial.print("  Value length: ");
   Serial.println(valueLength);
  }

  int valueStartPosition = colonPosition+1;

  if (isDebug)
  {
    Serial.print("  Value start position: ");
    Serial.println(valueStartPosition);
  }

  int value = readInt(msg, valueStartPosition, valueLength);
  
  if (isDebug)
  {
    Serial.print("  Value: ");
    Serial.println(value);

    Serial.println("Finished getting value");
    Serial.println("");
  }

  return value;
}

int getColonPosition(char* msg)
{

  if (isDebug)
  {
    Serial.print("Getting colon position from: ");
    Serial.println(msg);
  }

  int colonPosition = 0;

  unsigned int numElements = strlen(msg);
  unsigned int i;
  for(i = 0; i < numElements; ++i) {
      if (msg[i] == ':') {
          colonPosition = i;
          break;
      }
  }

  return colonPosition;
}

char* getPumpStatusText(int pumpStatus)
{
  switch(pumpStatus)
  {
    case 0:
      return "Off";
      break;
    case 1:
      return "On";
      break;
    case 2:
      return "Auto";
      break;
  }
}
*/
