#include "BlueBox.h"
#include "cat.h"

BlueBox bluebox;

uint8_t mode = 0;
unsigned long lastChange;

void setup()
{
  Serial.begin(9600);
  delay(500);

  bluebox.init();
  
  Serial.println("Setup Done");

  bluebox.fill(ST7735_BLUE);
  bluebox.drawTextCentered(50, "BlueBox", ST7735_WHITE, ST7735_BLUE, 3);

  bluebox.flush();
  bluebox.greenLed(true);
  bluebox.redLed(false);

  lastChange = millis();
}

void drawTitle(const char* title)
{
   int w, h;
   bluebox.measureText(title, 2, &w, &h);
   bluebox.fillRect(0, 0, bluebox.width(), h + 1, BLUEBOX_WHITE);
   bluebox.drawTextCentered(1, title, BLUEBOX_BLACK, BLUEBOX_WHITE, 2);
}

void loop()
{
  auto leftButtonState = bluebox.leftButton.update();
  auto rightButtonState = bluebox.rightButton.update();

  if (leftButtonState == Button::Pressed || rightButtonState == Button::Pressed || (millis() - lastChange > 10000))
  {
    lastChange = millis();

    bluebox.fill(ST7735_BLACK);

    bluebox.toggleGreenLed();
    bluebox.toggleRedLed();

    if (mode == 0)
    {
      drawTitle("Text");

      bluebox.drawText(13, 20, "Small", BLUEBOX_YELLOW, BLUEBOX_BLACK, 1);
      bluebox.drawText(50, 35, "Bigger", BLUEBOX_RED, BLUEBOX_BLACK, 2);
      bluebox.drawText(5, 60, "Bigger", BLUEBOX_GREEN, BLUEBOX_BLACK, 3);

      int w, h;
      bluebox.measureText("Mega", 4, &w, &h);
      bluebox.drawText(bluebox.width() - w, bluebox.height() - h, "Mega", BLUEBOX_COBALT, BLUEBOX_BLACK, 4);
    }
    
    if (mode == 1)
    {
      drawTitle("Lines");

      bluebox.drawLine(0, 20, bluebox.width() - 1, 40, BLUEBOX_COBALT);
      bluebox.drawLine(0, 20, bluebox.width() - 1, 50, BLUEBOX_PINK);
      bluebox.drawLine(0, 20, bluebox.width() - 1, 60, BLUEBOX_EMERALD);
      bluebox.drawLine(0, 20, bluebox.width() - 1, 70, BLUEBOX_YELLOW);
      bluebox.drawLine(0, 20, bluebox.width() - 1, 80, BLUEBOX_RED);
      bluebox.drawLine(0, 20, bluebox.width() - 1, 90, BLUEBOX_OLIVE);
      bluebox.drawLine(0, 20, bluebox.width() - 1, 100, BLUEBOX_BROWN);

      bluebox.drawHLine(20, 100, 50, BLUEBOX_WHITE);

      bluebox.drawVLine(20, 20, 100, BLUEBOX_WHITE);
      bluebox.drawVLine(30, 20, 100, BLUEBOX_WHITE);
      bluebox.drawVLine(40, 20, 100, BLUEBOX_WHITE);
      bluebox.drawVLine(50, 20, 100, BLUEBOX_WHITE);
      bluebox.drawVLine(60, 20, 100, BLUEBOX_WHITE);
    }

    if (mode == 2)
    {
      drawTitle("Circles");

      bluebox.drawThickCircle(bluebox.width() / 2 , bluebox.height() / 2 , 40, 3, BLUEBOX_YELLOW);

      bluebox.drawCircle(bluebox.width() / 2 , bluebox.height() / 2, 30, BLUEBOX_RED, false);
      bluebox.drawCircle(bluebox.width() / 2 , bluebox.height() / 2, 20, BLUEBOX_GREEN, true);
    }

    if (mode == 3)
    {
      drawTitle("Rectangles");

      bluebox.fillRect(0, 25, 150, 10, BLUEBOX_RED);
      bluebox.fillRect(20, 40, 15, bluebox.width() - 1, BLUEBOX_YELLOW);
      bluebox.drawRect(50, 50, 80, 120, BLUEBOX_PINK);
      bluebox.drawRoundedRect(30, 30, 100, 50, 20, BLUEBOX_GREEN, false);
    }

    if (mode == 4)
    {
      drawTitle("Bitmaps");

      bluebox.drawRGBBitmap(0, 30, cat, CAT_WIDTH, CAT_HEIGHT);
      bluebox.drawRGBBitmap(100, 40, cat, CAT_WIDTH, CAT_HEIGHT);
      bluebox.drawRGBBitmap(50, 30, cat, CAT_WIDTH, CAT_HEIGHT);
      bluebox.drawRGBBitmap(40, 80, cat, CAT_WIDTH, CAT_HEIGHT);
      bluebox.drawRGBBitmap(60, 60, cat, CAT_WIDTH, CAT_HEIGHT);
    }

    if (mode == 5)
    {
      drawTitle("Arcs");

      bluebox.drawThickArc(bluebox.width() / 2 , bluebox.height(), bluebox.height() / 2 -  0, 10, 0, 180, BLUEBOX_CYAN);
      bluebox.drawThickArc(bluebox.width() / 2 , bluebox.height(), bluebox.height() / 2 - 12, 10, 0, 180, BLUEBOX_COBALT);
      bluebox.drawThickArc(bluebox.width() / 2 , bluebox.height(), bluebox.height() / 2 - 24, 10, 0, 180, BLUEBOX_INDIGO);
      bluebox.drawThickArc(bluebox.width() / 2 , bluebox.height(), bluebox.height() / 2 - 36, 10, 0, 180, BLUEBOX_VIOLET);
      bluebox.drawThickArc(bluebox.width() / 2 , bluebox.height(), bluebox.height() / 2 - 48, 10, 0, 180, BLUEBOX_PINK);

      bluebox.drawThickArc(bluebox.width() / 2 , bluebox.height() / 2,
                           bluebox.height() / 2 - 30, 10,
                           10, 80,
                           BLUEBOX_YELLOW);

      bluebox.drawThickArc(bluebox.width() / 2 , bluebox.height() / 2,
                           bluebox.height() / 2 - 30, 10,
                           80, 200,
                           BLUEBOX_RED);

      bluebox.drawThickArc(bluebox.width() / 2 , bluebox.height() / 2,
                           bluebox.height() / 2 - 30, 10,
                           200, 285,
                           BLUEBOX_MAGENTA);

      bluebox.drawThickArc(bluebox.width() / 2 , bluebox.height() / 2,
                           bluebox.height() / 2 - 30, 10,
                           285, 350,
                           BLUEBOX_GREEN);

      bluebox.drawThickArc(bluebox.width() / 2 , bluebox.height() / 2,
                           bluebox.height() / 2 - 30, 10,
                           350, 10,
                           BLUEBOX_CYAN);
    }

    if (mode == 6)
    {
      drawTitle("Colors");

      const int rows = 5;
      const int cols = 4;

      const int topmargin = 20;
      int w = (bluebox.width() / cols);
      int h = ((bluebox.height() - topmargin) / rows);
      for (int c = 0; c < BLUEBOX_COLOR_COUNT; c++)
      {
        int x = (c % cols) * w;
        int y = (c / cols) * h + topmargin;
        bluebox.fillRect(x, y, w - 1, h - 4, bluebox_colors[c]);
        bluebox.drawText(x + 1, y + 5, bluebox_color_names[c], BLUEBOX_WHITE, bluebox_colors[c], 1);
      }
    }

    if (mode == 7)
    {
      drawTitle("Grays");
     
      bluebox.fillRect(0 * 26, 20, 25, 100, BLUEBOX_GRAY1); 
      bluebox.fillRect(1 * 26, 20, 25, 100, BLUEBOX_GRAY2); 
      bluebox.fillRect(2 * 26, 20, 25, 100, BLUEBOX_GRAY3); 
      bluebox.fillRect(3 * 26, 20, 25, 100, BLUEBOX_GRAY4); 
      bluebox.fillRect(4 * 26, 20, 25, 100, BLUEBOX_GRAY5); 
      bluebox.fillRect(5 * 26, 20, 25, 100, BLUEBOX_GRAY6); 
    }
    
    bluebox.flush();
    mode = (mode + 1) % 8;
  }
}
