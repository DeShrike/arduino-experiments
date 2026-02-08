#include "BlueBox.h"
#include "Menu.h"

uint16_t colors[] = {
  ST7735_WHITE,
  ST7735_YELLOW,
  ST7735_RED,
  ST7735_GREEN,
  ST7735_BLUE,
  ST7735_MAGENTA,
  ST7735_CYAN,
  ST77XX_ORANGE
};

#define COLOR_COUNT (sizeof(colors) / sizeof(colors[0]))

BlueBox bluebox;
Menu menu(&bluebox);

uint8_t mode = 0;
uint8_t colorIndex = 0;

void setup()
{
  Serial.begin(9600);
  delay(500);

  bluebox.init();
  menu.addItem(1, "Ball");
  menu.addItem(2, "Circles");
  menu.addItem(3, "Rectangles");
  menu.addItem(4, "RoundRect");
  menu.addItem(5, "Lines");
  menu.addItem(6, "Leds");
  menu.addItem(7, "Gradient");
  menu.addItem(8, "Lines");
  menu.addItem(9, "Annulus");
  menu.addItem(10, "3D");
  
  Serial.println("Setup Done");
}

void loop()
{
    Button::Event el = bluebox.leftButton.update();
    Button::Event er = bluebox.rightButton.update();

    switch (mode)
    {
      case 0: // menu
        if (el == Button::Pressed)
        {
          menu.next();
        }
        else if (el == Button::LongPressed)
        {
          mode = menu.current;
          bluebox.fill(ST7735_BLACK);
          break;
        }
        menu.draw();
        // bluebox.drawLine(0, 0, bluebox.width(), bluebox.height(), ST7735_WHITE);
        break;
      case 1: // Ball
        doMode1();
        if (el == Button::LongPressed) mode = 0;
        break;
      case 2: // Cirlcles
        doMode2();
        if (el == Button::LongPressed) mode = 0;
        break;
      case 3: // Rectangles
        doMode3();
        if (el == Button::LongPressed) mode = 0;
        break;
      case 4: // Round Rectangles
        doMode4();
        if (el == Button::LongPressed) mode = 0;
        break;
      case 5: // Lines
        doMode5();
        if (el == Button::LongPressed) mode = 0;
        break;
      case 6: // Leds
        doMode6();
        if (el == Button::LongPressed) mode = 0;
        break;
      case 7: // Gradient
        doMode7();
        if (el == Button::LongPressed) mode = 0;
        break;
      case 8: // Lines
        doMode8();
        if (el == Button::LongPressed) mode = 0;
        break;
      case 9: // Annulus
        doMode9();
        if (el == Button::LongPressed) mode = 0;
        break;
    }


    /*
    bluebox.fill(ST7735_BLACK);

    bluebox.drawRoundedRect(30, 30, bluebox.width() - 60, bluebox.height() - 60, 15, ST7735_BLUE, true);
    bluebox.drawRoundedRect(30, 30, bluebox.width() - 60, bluebox.height() - 60, 15, ST7735_YELLOW, false);

    bluebox.fillRect(10, 10, 5, 5, ST7735_RED);
    bluebox.fillRect(bluebox.width() - 10 - 5, 10, 5, 5, ST7735_GREEN);
    bluebox.fillRect(bluebox.width() - 10 - 5, bluebox.height() - 10 - 5, 5, 5, ST7735_YELLOW);
    bluebox.fillRect(10, bluebox.height() - 10 - 5, 5, 5, ST7735_BLUE);

    for (int x = 0; x < 30; x += 5)
    {
      bluebox.drawRect(x, x, 100 - x, 100 - x, ST7735_WHITE);
    }

    if (a1) bluebox.drawText(20, 15, "ESP32", ST7735_RED, ST7735_BLACK, 2);
    if (a2) bluebox.drawTextMulti(20, 31, "ESP32-C3\nSuperMini\nBlueBox\nCoderdojo", ST7735_YELLOW, ST7735_BLACK, 2);
    */

    bluebox.flush();
}



/////////////////////////////////////////////////
// Mode 1
/////////////////////////////////////////////////

uint16_t cx = 20, cy = 20, cr = 20;
int16_t cdx = 1, cdy = 1;
bool move = true;

void doMode1()
{
    bluebox.drawCircle(cx, cy, cr, ST7735_BLACK, true);

    if (move)
    {
      cx += cdx;
      cy += cdy;
      if (cx - cr < 0)
      {
         cx = cr;
         cdx = -cdx;
         colorIndex = (colorIndex + 1) % COLOR_COUNT;
      }
      
      if (cx + cr >= bluebox.width())
      {
         cx = bluebox.width() - cr;
         cdx = -cdx;
      }
  
      if (cy - cr < 0)
      {
         cy = cr;
         cdy = -cdy;
      }
      
      if (cy + cr >= bluebox.height())
      {
         cy = bluebox.height() - cr;
         cdy = -cdy;
      }
    }

    bluebox.drawCircle(cx, cy, cr, colors[colorIndex], true);
    bluebox.drawCircle(cx, cy, cr, ST7735_WHITE, false);
}

/////////////////////////////////////////////////
// Mode 2
/////////////////////////////////////////////////

void doMode2()
{
    uint16_t cx = esp_random() % (bluebox.width() - 10);
    uint16_t cy = esp_random() % (bluebox.height() - 10);
    uint16_t cr = esp_random() % (bluebox.height() / 2) + 2;
  
    bluebox.drawCircle(cx, cy, cr, colors[colorIndex], true);
    bluebox.drawCircle(cx, cy, cr, ST7735_WHITE, false);

    colorIndex = (colorIndex + 1) % COLOR_COUNT;
    delay(25);
}

/////////////////////////////////////////////////
// Mode 3
/////////////////////////////////////////////////

void doMode3()
{
    uint16_t x = esp_random() % (bluebox.width());
    uint16_t y = esp_random() % (bluebox.height());
    uint16_t w = esp_random() % (bluebox.width() / 4) + 2;
    uint16_t h = esp_random() % (bluebox.height() / 4) + 2;

    bluebox.fillRect(x, y, w, h, colors[colorIndex]);

    colorIndex = (colorIndex + 1) % COLOR_COUNT;
    delay(25);
}

/////////////////////////////////////////////////
// Mode 4
/////////////////////////////////////////////////

void doMode4()
{
    uint16_t x = esp_random() % (bluebox.width());
    uint16_t y = esp_random() % (bluebox.height());
    uint16_t w = esp_random() % (bluebox.width() / 4) + 2;
    uint16_t h = esp_random() % (bluebox.height() / 4) + 2;

    bluebox.drawRoundedRect(x, y, w, h, 5, colors[colorIndex], true);

    colorIndex = (colorIndex + 1) % COLOR_COUNT;
    delay(25);
}

/////////////////////////////////////////////////
// Mode 5
/////////////////////////////////////////////////

void doMode5()
{
    uint16_t x = esp_random() % (bluebox.width());
    uint16_t y = esp_random() % (bluebox.height());
    uint16_t s = esp_random() % (bluebox.width() / 4) + 2;
    uint16_t d = esp_random() % 2;

    if (d == 0)
    {
      bluebox.drawHLine(x, y, x + s, colors[colorIndex]);
    }
    else
    {
      bluebox.drawVLine(x, y, y + s, colors[colorIndex]);
    }

    colorIndex = (colorIndex + 1) % COLOR_COUNT;
    delay(25);
}

/////////////////////////////////////////////////
// Mode 6
/////////////////////////////////////////////////

void doMode6()
{
    uint16_t r = esp_random() % 2;
    uint16_t g = esp_random() % 2;

    if (g == 1)
    {
      bluebox.drawCircle(bluebox.width() / 3, bluebox.height() / 2, 20, ST7735_GREEN, true);
      bluebox.drawCircle(bluebox.width() / 3, bluebox.height() / 2, 20, ST7735_WHITE, false);
      bluebox.greenLed(true);
    }
    else
    {
      bluebox.drawCircle(bluebox.width() / 3, bluebox.height() / 2, 20, ST7735_BLACK, true);
      bluebox.greenLed(false);
    }

    if (r == 1)
    {
      bluebox.drawCircle(bluebox.width() / 3 * 2, bluebox.height() / 2, 20, ST7735_RED, true);
      bluebox.drawCircle(bluebox.width() / 3 * 2 , bluebox.height() / 2, 20, ST7735_WHITE, false);
      bluebox.redLed(true);
    }
    else
    {
      bluebox.drawCircle(bluebox.width() / 3 * 2 , bluebox.height() / 2, 20, ST7735_BLACK, true);
      bluebox.redLed(false);
    }

    delay(100);
}

/////////////////////////////////////////////////
// Mode 7
/////////////////////////////////////////////////

uint8_t b = 0;

void doMode7()
{
  for (uint8_t x = 0; x < bluebox.width(); x++)
  {
    uint8_t r = MAP(x, 0, bluebox.width() - 1, 0, 255);
    for (uint8_t y = 0; y < bluebox.height(); y++)
    {
      uint8_t g = MAP(y, 0, bluebox.height() - 1, 0, 255);
      if (b % 2 == 0)
        bluebox.drawPixelUnchecked(x, y, RGBto565(r, g, b));
      else
        bluebox.drawPixelUnchecked(x, y, RGBto565(g, r, b));
    }
  }

  b = (b + 15) % 255;
  delay(250);
}

/////////////////////////////////////////////////
// Mode 8
/////////////////////////////////////////////////

void doMode8()
{
    uint16_t x1 = esp_random() % (bluebox.width());
    uint16_t y1 = esp_random() % (bluebox.height());
    uint16_t x2 = esp_random() % (bluebox.width());
    uint16_t y2 = esp_random() % (bluebox.height());

    bluebox.drawLine(x1, y1, x2, y2, colors[colorIndex]);

    colorIndex = (colorIndex + 1) % COLOR_COUNT;
    delay(25);
}

/////////////////////////////////////////////////
// Mode 9
/////////////////////////////////////////////////

void doMode9()
{
    uint16_t x1 = esp_random() % (bluebox.width());
    uint16_t y1 = esp_random() % (bluebox.height());
    uint16_t x2 = esp_random() % (bluebox.width());
    uint16_t y2 = esp_random() % (bluebox.height());

    if (colorIndex % 5 == 0)
      bluebox.drawThickCircle(bluebox.width() / 2 , bluebox.height() / 2 , bluebox.height() / 2 - 10, 5, colors[colorIndex]);
    else if (colorIndex % 5 == 1)
      bluebox.drawThickCircle(bluebox.width() / 2 , bluebox.height() / 2 , bluebox.height() / 2 - 20, 5, colors[colorIndex]);
    else if (colorIndex % 5 == 2)
      bluebox.drawThickCircle(bluebox.width() / 2 , bluebox.height() / 2 , bluebox.height() / 2 - 30, 5, colors[colorIndex]);
    else if (colorIndex % 5 == 3)
      bluebox.drawThickCircle(bluebox.width() / 2 , bluebox.height() / 2 , bluebox.height() / 2 - 40, 5, colors[colorIndex]);
    else
      bluebox.drawThickCircle(bluebox.width() / 2 , bluebox.height() / 2 , bluebox.height() / 2 - 50, 5, colors[colorIndex]);

    colorIndex = (colorIndex + 1) % COLOR_COUNT;
    delay(250);
}
