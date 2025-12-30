#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

// Taken from https://github.com/Bodmer/TFT_ST7735

// TFT pin definitions
#define TFT_CS   7
#define TFT_DC   8
#define TFT_RST  9

#define SCK   4 // GPIO4
#define MOSI  6 // GPIO6
#define MISO -1 // MISO = not used (-1)

/* https://www.otronic.nl/en/esp32-c3-wi-fi-ble.html
 * TFT      ESP32-C3-SuperMini  
 * ------   ----------------------
 * LED      3.3V
 * SCK      GPIO4 SCK
 * SDA      GPIO6 MOSI
 * AO       GPIO8 SDA
 * RESET    GPIO9 SCL
 * CS       GPIO7 SS
 * GND      GND 
 * VCC      3.3V
 * ------   ----------------------
 */

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

#define ST7735_GREY 0xBDF7

float sx = 0, sy = 1, mx = 1, my = 0, hx = -1, hy = 0;    // Saved H, M, S x & y multipliers
float sdeg = 0, mdeg = 0, hdeg = 0;
uint16_t osx = 64, osy = 64, omx = 64, omy = 64, ohx = 64, ohy = 64;  // Saved H, M, S x & y coords
uint16_t x0 = 0, x1 = 0, y00 = 0, y01 = 0;
uint32_t targetTime = 0;                    // for next 1 second timeout

static uint8_t conv2d(const char* p)
{
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9')
  {
    v = *p - '0';
  }
  return 10 * v + *++p - '0';
}

uint8_t hh=conv2d(__TIME__), mm=conv2d(__TIME__+3), ss=conv2d(__TIME__+6);  // Get H, M, S from compile time

boolean initial = 1;

void setup()
{
  SPI.begin(SCK, MISO, MOSI);  

  tft.initR(INITR_BLACKTAB);  // Most 1.8" AZ-Delivery displays

  tft.setRotation(0);
  tft.fillScreen(ST7735_BLACK);
  delay(1000);

  tft.setTextColor(ST7735_GREEN, ST7735_GREY);  // Adding a black background colour erases previous text automatically
  
  // Draw clock face
  tft.fillCircle(64, 64, 61, ST7735_BLUE);
  tft.fillCircle(64, 64, 57, ST7735_BLACK);

  // Draw 12 lines
  for(int i = 0; i < 360; i += 30)
  {
    sx = cos((i - 90) * 0.0174532925);
    sy = sin((i - 90) * 0.0174532925);
    x0 = sx * 57 + 64;
    y00 = sy * 57 + 64;
    x1 = sx * 50 + 64;
    y01 = sy * 50 + 64;

    tft.drawLine(x0, y00, x1, y01, ST7735_BLUE);
  }

  // Draw 60 dots
  for(int i = 0; i < 360; i += 6)
  {
    sx = cos((i - 90) * 0.0174532925);
    sy = sin((i - 90) * 0.0174532925);
    x0 = sx * 53 + 64;
    y00 = sy * 53 + 64;
    
    tft.drawPixel(x0, y00, ST7735_BLUE);
    if(i == 0 || i == 180) tft.fillCircle(x0, y00, 1, ST7735_CYAN);
    if(i == 0 || i == 180) tft.fillCircle(x0 + 1, y00, 1, ST7735_CYAN);
    if(i == 90 || i == 270) tft.fillCircle(x0, y00, 1, ST7735_CYAN);
    if(i == 90 || i == 270) tft.fillCircle(x0 + 1, y00, 1, ST7735_CYAN);
  }

  tft.fillCircle(65, 65, 3, ST7735_RED);

  // Draw text at position 64,125 using fonts 4
  // Only font numbers 2,4,6,7 are valid. Font 6 only contains characters [space] 0 1 2 3 4 5 6 7 8 9 : . a p m
  // Font 7 is a 7 segment font and only contains characters [space] 0 1 2 3 4 5 6 7 8 9 : .
  //tft.drawCentreString("Time flies", 64, 130, 4);

  targetTime = millis() + 1000; 
}

void loop()
{
  if (targetTime < millis()) 
  {
    targetTime = millis()+1000;
    ss++;              // Advance second
    if (ss == 60)
    {
      ss = 0;
      mm++;            // Advance minute
      if (mm > 59)
      {
        mm = 0;
        hh++;          // Advance hour
        if (hh > 23)
        {
          hh = 0;
        }
      }
    }

    // Pre-compute hand degrees, x & y coords for a fast screen update
    sdeg = ss * 6;                  // 0-59 -> 0-354
    mdeg = mm * 6 + sdeg * 0.01666667;  // 0-59 -> 0-360 - includes seconds
    hdeg = hh * 30 + mdeg * 0.0833333;  // 0-11 -> 0-360 - includes minutes and seconds
    hx = cos((hdeg - 90) * 0.0174532925);    
    hy = sin((hdeg - 90) * 0.0174532925);
    mx = cos((mdeg - 90) * 0.0174532925);    
    my = sin((mdeg - 90) * 0.0174532925);
    sx = cos((sdeg - 90) * 0.0174532925);    
    sy = sin((sdeg - 90) * 0.0174532925);

    if (ss == 0 || initial)
    {
      initial = 0;
      // Erase hour and minute hand positions every minute
      tft.drawLine(ohx, ohy, 65, 65, ST7735_BLACK);
      ohx = hx * 33 + 65;    
      ohy = hy * 33 + 65;
      tft.drawLine(omx, omy, 65, 65, ST7735_BLACK);
      omx = mx * 44 + 65;    
      omy = my * 44 + 65;
    }

    // Redraw new hand positions, hour and minute hands not erased here to avoid flicker
    tft.drawLine(osx, osy, 65, 65, ST7735_BLACK);
    tft.drawLine(ohx, ohy, 65, 65, ST7735_WHITE);
    tft.drawLine(omx, omy, 65, 65, ST7735_WHITE);
    osx = sx*47+65;    
    osy = sy*47+65;
    tft.drawLine(osx, osy, 65, 65, ST7735_RED);

    tft.fillCircle(65, 65, 3, ST7735_RED);
  }
}
