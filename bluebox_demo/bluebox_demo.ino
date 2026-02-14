#include "BlueBox.h"
#include "Menu.h"
#include "TextFader.h"
#include "cat1.h"
#include "cat2.h"
#include "cat3.h"
#include "cat4.h"
#include "jane.h"
#include "notes.h"

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

// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

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
  menu.addItem(13, "Bitmap");
  menu.addItem(14, "FadeInOut");
  menu.addItem(15, "Buzzer");
  menu.addItem(16, "Sprite");
  
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
      case 13: // Bitmap
        doMode13();
        if (el == Button::LongPressed) mode = 0;
        break;
      case 14: // FadeInOut
        doMode14();
        if (el == Button::LongPressed) mode = 0;
        break;
      case 15: // Buzzer
        doMode15();
        if (el == Button::LongPressed) mode = 0;
        break;
      case 16: // Sprite
        doMode16();
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

uint8_t gradientMode = 0;

void doMode7()
{
  uint8_t r, g, b;
  for (uint8_t x = 0; x < bluebox.width(); x++)
  {
    switch (gradientMode)
    {
      case 0:
        r = MAP(x, 0, bluebox.width() - 1, 255, 0);
        g = 0;
        b = 0;
        break;
      case 1:
        r = 0;
        g = MAP(x, 0, bluebox.width() - 1, 255, 0);
        b = 0;
        break;
      case 2:
        r = 0;
        g = 0;
        b = MAP(x, 0, bluebox.width() - 1, 255, 0);
        break;
      case 3:
        r = 0;
        g = MAP(x, 0, bluebox.width() - 1, 255, 0);
        b = MAP(x, 0, bluebox.width() - 1, 255, 0);
        break;
      case 4:
        r = MAP(x, 0, bluebox.width() - 1, 255, 0);
        g = 0;
        b = MAP(x, 0, bluebox.width() - 1, 255, 0);
        break;
      case 5:
        r = MAP(x, 0, bluebox.width() - 1, 255, 0);
        g = MAP(x, 0, bluebox.width() - 1, 255, 0);
        b = 0;
        break;
      case 6:
        r = MAP(x, 0, bluebox.width() - 1, 255, 0);
        g = MAP(x, 0, bluebox.width() - 1, 255, 0);
        b = MAP(x, 0, bluebox.width() - 1, 255, 0);
        break;
    }

    for (uint8_t y = 0; y < bluebox.height(); y++)
    {
      bluebox.drawPixelUnchecked(x, y, RGBto565(r, g, b));
    }
  }

  gradientMode = (gradientMode  + 1) % 7;

  delay(1000);
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

/////////////////////////////////////////////////
// Mode 13
/////////////////////////////////////////////////

int16_t catx = 13, caty = 0, catdx = 1, catdy = 1, cat = 1;

void doMode13()
{
  //bluebox.redLed(true);
  if (cat == 0)
  {
    bluebox.drawRGBBitmap(0, 0,
                           cat1,
                           CAT1_WIDTH, CAT1_HEIGHT);
  }
  else if (cat == 1)
  {
    bluebox.drawRGBBitmap(0, 0,
                           cat2,
                           CAT2_WIDTH, CAT2_HEIGHT);    
  }
  else if (cat == 2)
  {
    bluebox.drawRGBBitmap(0, 0,
                           cat3,
                           CAT3_WIDTH, CAT3_HEIGHT);    
  }

  bluebox.drawRGBBitmap(catx, caty,
                         cat4,
                         CAT4_WIDTH, CAT4_HEIGHT);

  catx += catdx;
  caty += catdy;
  if (catx < 0) { catx = 0; catdx = -catdx; cat = (cat + 1) % 3; }
  if (caty < 0) { caty = 0; catdy = -catdy; }
  if (catx + CAT4_WIDTH >= bluebox.width()) { catx = bluebox.width() - CAT4_WIDTH; catdx = -catdx; }
  if (caty + CAT4_HEIGHT >= bluebox.height()) { caty = bluebox.height() - CAT4_HEIGHT; catdy = -catdy; }

  //bluebox.redLed(false);
  //delay(10);
}

/////////////////////////////////////////////////
// Mode 14
/////////////////////////////////////////////////
TextFader* text1 = nullptr;
TextFader* text2 = nullptr;
TextFader* text3 = nullptr;

void doMode14()
{
    if (text1 == nullptr)
    {
        text1 = new TextFader(&bluebox, 10, 10, "ESP32-C3", 2, ST7735_RED, ST7735_YELLOW);
        text1->onStateChange([](FaderState state) {
            //Serial.print("Text1 onStateChange ");
            //Serial.println(text1->stateName());
            if (state == FaderState::FadedIn)
            {
                bluebox.toggleGreenLed();
                text2->fadeIn();
            }
            else if (state == FaderState::FadedOut)
            {
                text1->fadeIn();
            }
        });

        text1->fadeIn();
        bluebox.fill(ST7735_YELLOW);
    }

    if (text2 == nullptr)
    {
        text2 = new TextFader(&bluebox, 10, 30, "CoderDojo", 2, ST7735_BLUE, ST7735_YELLOW);
        text2->onStateChange([](FaderState state) {
            //Serial.print("Text2 onStateChange ");
            //Serial.println(text2->stateName());
            if (state == FaderState::FadedIn)
            {
                text3->fadeIn();
            }
            else if (state == FaderState::FadedOut)
            {
                text1->fadeOut();
            }
        });
    }

    if (text3 == nullptr)
    {
        text3 = new TextFader(&bluebox, 10, 50, "Roeselare", 2, ST7735_GREEN, ST7735_YELLOW);
        text3->onStateChange([](FaderState state) {
            //Serial.print("Text3 onStateChange ");
            //Serial.println(text3->stateName());
            if (state == FaderState::FadedIn)
            {
                text3->fadeOut();
            }
            else if (state == FaderState::FadedOut)
            {
                text2->fadeOut();
            }
        });
    }

    text1->update();
    text2->update();
    text3->update();
}

/////////////////////////////////////////////////
// Mode 15
/////////////////////////////////////////////////

void playMelody()
{
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 8; thisNote++)
  {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(BUZZER_PIN, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(BUZZER_PIN);
  }
}

void doMode15()
{
  bluebox.fill(ST7735_GREEN);
  int y = (bluebox.height() - (5 * 20)) / 2;
  bluebox.drawTextCentered(y, "Buzzer", ST7735_RED, ST7735_GREEN, 3);
  bluebox.flush();

  bluebox.greenLed(true);
  playMelody();
  bluebox.greenLed(false);

  mode = 0;
}

/////////////////////////////////////////////////
// Mode 16
/////////////////////////////////////////////////
void doMode16()
{
  int x = (bluebox.width() - JANE_WIDTH) / 2;
  int y = (bluebox.height() - JANE_HEIGHT) / 2;
  bluebox.drawSprite(x, y, jane_bmp, JANE_WIDTH, JANE_HEIGHT, colors[colorIndex]);  
  colorIndex = (colorIndex + 1) % COLOR_COUNT;
  delay(500);
}
