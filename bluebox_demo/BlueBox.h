#ifndef _BLUEBOX_H_
#define _BLUEBOX_H_

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <glcdfont.c>
#include <SPI.h>
#include "Button.h"

// TFT pin definitions
#define TFT_CS   7
#define TFT_DC   8
#define TFT_RST  9

#define SCK   4 // GPIO4
#define MOSI  6 // GPIO6
#define MISO -1 // MISO = not used (-1)

#define RGBto565(r,g,b) ((((r) & 0xF8) << 8) | (((g) & 0xFC) << 3) | ((b) >> 3)) 
#define MAP(x,in_min,in_max,out_min,out_max) \
  ((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min)

/* https://www.otronic.nl/en/esp32-c3-wi-fi-ble.html
   TFT      ESP32-C3-SuperMini
   ------   ----------------------
   LED      3.3V
   SCK      GPIO4 SCK
   SDA      GPIO6 MOSI
   AO       GPIO8 SDA
   RESET    GPIO9 SCL
   CS       GPIO7 SS
   GND      GND
   VCC      3.3V
   ------   ----------------------
*/

#define LED1_PIN  1
#define LED2_PIN  0

#define BUZZER_PIN 2

#define RIGHTBUTTON_PIN  10
#define LEFTBUTTON_PIN  20

#define BACKLIGHT_PIN 5

class BlueBox
{
  private:
    static constexpr int WIDTH = 160;
    static constexpr int HEIGHT = 128;

    Adafruit_ST7735 tft;
    uint16_t buffer[WIDTH * HEIGHT];

    int dirtyX1 = WIDTH, dirtyY1 = HEIGHT;
    int dirtyX2 = 0, dirtyY2 = 0;
    bool initialized = false;

    bool redLedState = false;
    bool greenLedState = false;

    void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);
    void drawRoundedCorner(uint16_t x0, uint16_t y0, uint16_t r, uint8_t corner, uint16_t color);
    void drawLineLow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
    void drawLineHigh(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
    void markDirty(int x1, int y1, int x2, int y2);

  public:
    BlueBox();
    BlueBox(const BlueBox&) = delete;
    BlueBox& operator=(const BlueBox&) = delete;

    uint8_t width() { return WIDTH; }
    uint8_t height() { return HEIGHT; }

    // Display
    void init();
    void flush();

    // Drawing
    void fill(uint16_t color);
    void drawPixel(int x, int y, uint16_t color);
    void drawPixelUnchecked(int x, int y, uint16_t color);

    void clearRect(int x, int y, int w, int h);
    void fillRect(int x, int y, int w, int h, uint16_t color);
    void drawRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);

    void drawRoundedRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint16_t color, bool filled);

    void measureText(const char* text, uint8_t fontSize, int* width, int* height);
    void drawText(int x, int y, const char* text, uint16_t fgcolor = ST7735_WHITE, uint16_t bgcolor = ST7735_BLACK, uint8_t fontSize = 1);
    void drawTextMulti(int x, int y, const char* text, uint16_t fgcolor = ST7735_WHITE, uint16_t bgcolor = ST7735_BLACK, uint8_t fontSize = 1);
    void drawTextCentered(int y, const char* text, uint16_t fgcolor, uint16_t bgcolor, uint8_t fontSize);

    void drawSprite(int x, int y, const uint8_t* sprite, int w, int h, uint16_t color);
    void drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color, bool filled);

    void drawVLine(uint16_t x, uint16_t y1, uint16_t y2, uint16_t color);
    void drawHLine(uint16_t x1, uint16_t x2, uint16_t y, uint16_t color);
    void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
    
    void drawThickArc(int cx, int cy, int radius, int thickness,
                      float startDeg, float endDeg, uint16_t color, float step = 1.0);
    void drawFilledThickArc(int cx, int cy,
                            int innerRadius, int outerRadius,
                            float startDeg, float endDeg,
                            uint16_t color, float step = 1.0);
    void drawAntiAliasedThickArc(int cx, int cy,
                                 int radius, int thickness,
                                 float startDeg, float endDeg,
                                 uint16_t color, float step = 0.5);
    void drawFilledAntiAliasedArc(int cx, int cy,
                                  int innerRadius, int outerRadius,
                                  float startDeg, float endDeg,
                                  uint16_t color, float step = 0.5);

    void scrollHorizontally(int dx);

    // Buttons
    Button leftButton = Button(LEFTBUTTON_PIN);
    Button rightButton = Button(RIGHTBUTTON_PIN);
    void beginInput();
    void processInput();

    // Leds
    void greenLed(bool flag);
    void redLed(bool flag);
    void toggleGreenLed();
    void toggleRedLed();

    // Backlight
    void backLight(uint8_t brightness);
};

#endif
