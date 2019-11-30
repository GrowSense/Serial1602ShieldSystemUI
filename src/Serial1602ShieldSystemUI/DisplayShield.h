#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <LiquidCrystal.h>

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

extern LiquidCrystal lcd;

extern int lcd_key;
extern int adc_key_in;

extern long lastButtonPress;
extern long buttonInterval;

void handleButtons();

void handleButton(char lcd_key);

int read_LCD_buttons();

#endif
/* DISPLAY_H_ */
