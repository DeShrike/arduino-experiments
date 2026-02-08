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
  menu.addItem(4, "RoundRects");
  menu.addItem(5, "Lines");
  menu.addItem(6, "Leds");
  menu.addItem(7, "Gradient");
  menu.addItem(8, "Lines");
  menu.addItem(9, "Annulus");
  menu.addItem(10, "Arcs");
  menu.addItem(11, "Text");
  menu.addItem(12, "Counter");
  
  Serial.println("Setup Done");
}

void loop()
{
    auto el = bluebox.leftButton.update();
    auto er = bluebox.rightButton.update();

    switch (mode)
    {
      case 0: // menu
        if (el == Button::Pressed)
        {
          menu.next();
        }
        else if (el == Button::DoublePressed)
        {
          menu.prev();
        }
        else if (el == Button::LongPressed)
        {
          mode = menu.current;
          bluebox.fill(ST7735_BLACK);
          break;
        }
        menu.draw();
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
      case 10: // Arc
        doMode10();
        if (el == Button::LongPressed) mode = 0;
        break;
      case 11: // Text
        doMode11();
        if (el == Button::LongPressed) mode = 0;
        break;
      case 12: // Counter
        doMode12();
        if (el == Button::LongPressed) mode = 0;
        break;
    }

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

/////////////////////////////////////////////////
// Mode 10
/////////////////////////////////////////////////

int degrees = 5;

void doMode10()
{
    //bluebox.fill(ST7735_BLACK);

    bluebox.drawThickArc(bluebox.width() / 2 , bluebox.height() / 2,
                           bluebox.height() / 2 - 10, 10,
                           0, degrees,
                           colors[colorIndex]);
    bluebox.drawThickArc(bluebox.width() / 2 , bluebox.height() / 2,
                           bluebox.height() / 2 - 20, 10,
                           90, (degrees + 90),
                           colors[(colorIndex + 1) % COLOR_COUNT]);
    bluebox.drawThickArc(bluebox.width() / 2 , bluebox.height() / 2,
                           bluebox.height() / 2 - 30, 10,
                           180, (degrees + 180),
                           colors[(colorIndex + 2) % COLOR_COUNT]);
    bluebox.drawThickArc(bluebox.width() / 2 , bluebox.height() / 2,
                           bluebox.height() / 2 - 40, 10,
                           270, (degrees + 270),
                           colors[(colorIndex + 3) % COLOR_COUNT]);
    degrees += 5;
    if (degrees > 360)
    {
      degrees = 5;
      bluebox.fill(ST7735_BLACK);
      colorIndex = (colorIndex + 1) % COLOR_COUNT;
    }

    delay(25);
}

/////////////////////////////////////////////////
// Mode 11
/////////////////////////////////////////////////

void doMode11()
{
    int y = (bluebox.height() - (5 * 20)) / 2;
    bluebox.drawTextCentered(y, "ESP32-C3", colors[colorIndex], ST7735_BLACK, 2);
    colorIndex = (colorIndex + 1) % COLOR_COUNT;
    y += 20;
    bluebox.drawTextCentered(y, "SuperMini", colors[colorIndex], ST7735_BLACK, 2);
    colorIndex = (colorIndex + 1) % COLOR_COUNT;
    y += 20;
    bluebox.drawTextCentered(y, "ST7735 TFT", colors[colorIndex], ST7735_BLACK, 2);
    colorIndex = (colorIndex + 1) % COLOR_COUNT;
    y += 20;
    bluebox.drawTextCentered(y, "CoderDojo", colors[colorIndex], ST7735_BLACK, 2);
    colorIndex = (colorIndex + 1) % COLOR_COUNT;
    y += 20;
    bluebox.drawTextCentered(y, "Roeselare", colors[colorIndex], ST7735_BLACK, 2);
    colorIndex = (colorIndex + 1) % COLOR_COUNT;

    delay(250);
}

/////////////////////////////////////////////////
// Mode 12
/////////////////////////////////////////////////

uint16_t counter = 0;

void doMode12()
{
  char temp[20];
  uint8_t fontSize = 3;
  uint8_t padding = 10;
  sprintf(temp, "%d", counter);
  int width, height;
  bluebox.measureText(temp, fontSize, &width, &height);

  uint8_t y = (bluebox.height() - height) / 2;
  uint8_t x = (bluebox.width() - width) / 2;
  bluebox.drawRoundedRect(x - padding - 1, y - padding, width + padding + padding + 1, height + padding + padding, 7, ST7735_BLUE, true);
  bluebox.drawRoundedRect(x - padding - 1, y - padding, width + padding + padding + 1, height + padding + padding, 7, ST7735_WHITE, false);
  bluebox.drawTextCentered(y, temp, ST7735_YELLOW, ST7735_BLUE, fontSize);

  counter++;
  if (counter % 100 == 0)
  {
    bluebox.toggleGreenLed();
  }

  if (counter % 1000 == 0)
  {
    bluebox.toggleRedLed();
  }

  delay(100);
}
