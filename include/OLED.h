#ifndef __OLED__
#define __OLED__

#include "Arduino.h"
#include "SSD1306Wire.h"


void InitDisplay();
void DisplayClear();
void DisplayUpdate();
void DisplayPrint(byte x, byte y, const char* format, ...);
//void drawRectDemo();

#endif