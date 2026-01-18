#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

#define DOUBLE_BUFFER

#include "3d.h"
#include "cube.h"

// TFT pin definitions
#define TFT_CS   7
#define TFT_DC   8
#define TFT_RST  9

#define SCK   4 // GPIO4
#define MOSI  6 // GPIO6
#define MISO -1 // MISO = not used (-1)

#define BACKLIGHT_PIN 5

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

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define SCREEN_WIDTH (tft.width())
#define SCREEN_HEIGHT (tft.height())

#ifdef DOUBLE_BUFFER
GFXcanvas16 canvas(SCREEN_WIDTH, SCREEN_HEIGHT);
#endif

void setup()
{
  SPI.begin(SCK, MISO, MOSI);  

  pinMode(BACKLIGHT_PIN, OUTPUT);

  tft.initR(INITR_BLACKTAB);  // Most 1.8" AZ-Delivery displays
  tft.fillScreen(ST7735_BLACK);
  delay(1000);

  analogWrite(BACKLIGHT_PIN, 127);
}

void setPixel(uint8_t x, uint8_t y)
{
#ifdef DOUBLE_BUFFER
  canvas.drawPixel(x, y, ST7735_WHITE);
#else
  tft.drawPixel(x, y, ST7735_WHITE);
#endif
}

void drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
#ifdef DOUBLE_BUFFER
  canvas.drawLine(x1, y1, x2, y2, ST7735_WHITE);
#else
  tft.drawLine(x1, y1, x2, y2, ST7735_WHITE);
#endif
}

float dz = 0.7;
float angle = 0;
float dangle = 0.1;

void draw_shape(Point* shape, int point_count, uint8_t* faces, uint8_t faces_count)
{
  uint8_t sx, sy;
  uint8_t ix = 0;
  uint8_t px = 0;
  uint8_t py = 0;
  uint8_t c = 0;
 
  // draw vertices
  /*
  for (int i = 0; i < point_count; i++)
  {
    calc(shape[i], &sx, &sy,
             dz, angle,
             SCREEN_WIDTH, SCREEN_HEIGHT);
    setPixel(sx, sy);
  }
  */

  // draw edges
  while (ix < faces_count)
  {
      c = faces[ix];
      calc(shape[faces[ix + 0 + 1]], &px, &py, dz, angle, SCREEN_WIDTH, SCREEN_HEIGHT);
      for (uint8_t i = 1; i < c; i++)
      {
        calc(shape[faces[ix + i + 1]], &sx, &sy, dz, angle, SCREEN_WIDTH, SCREEN_HEIGHT);
        drawLine(px, py, sx, sy);			
        px = sx;
        py = sy;
	  }

      ix += c + 1;
  }
}

void loop()
{
#ifdef DOUBLE_BUFFER
  canvas.fillScreen(ST7735_BLACK);
#else
  tft.fillScreen(ST7735_BLACK);
#endif

  draw_shape(cube, ARRAY_SIZE(cube), cube_faces, ARRAY_SIZE(cube_faces));
#ifdef DOUBLE_BUFFER
  tft.drawRGBBitmap(0, 0, canvas.getBuffer(), canvas.width(), canvas.height());
#endif

  angle += dangle;

  delay(25);
}
