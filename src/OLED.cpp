#include "OLED.h"

bool m_DisplayChangedFlag = true;
SSD1306Wire display(0x3c, 5, 4);

//************************************************************
//************************************************************
void InitDisplay()
{
  // Initialising the UI will init the display too.
  display.init();
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);//TEXT_ALIGN_CENTER,TEXT_ALIGN_RIGHT
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
}

//************************************************************
//************************************************************
void DisplayClear()
{
  display.clear();
  m_DisplayChangedFlag = true;
}

//************************************************************
//************************************************************
void DisplayUpdate()
{
  if (m_DisplayChangedFlag)
  {
    display.display();
    m_DisplayChangedFlag = false;
  }
}

//************************************************************
//************************************************************
void DisplayPrint(byte x, byte y, const char* format, ...)
{
  char buf[100];
  va_list argv;
  va_start(argv, format);
  vsprintf(buf, format, argv);

//  display.drawStringMaxWidth(x, y, 128,buf );
  display.drawString(x, y, buf);
  
  va_end(argv);
  m_DisplayChangedFlag = true;
}

/*
//************************************************************
//************************************************************
void drawRectDemo() {
  // Draw a pixel at given position
  for (int i = 0; i < 10; i++) {
    display.setPixel(i, i);
    display.setPixel(10 - i, i);
  }
  display.drawRect(12, 12, 20, 20);

  // Fill the rectangle
  display.fillRect(14, 14, 17, 17);

  // Draw a line horizontally
  display.drawHorizontalLine(0, 40, 20);

  // Draw a line horizontally
  display.drawVerticalLine(40, 0, 20);
}
*/
