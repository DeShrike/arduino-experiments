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

// Predefined Colors
#define BLUEBOX_LIME 0xa620 // Lime #A4C400 RGB(164, 196, 0)
#define BLUEBOX_GREEN 0x6542 // Green #60A917 RGB(96, 169, 23)
#define BLUEBOX_EMERALD 0x440 // Emerald #008A00 RGB(0, 138, 0)
#define BLUEBOX_TEAL 0x555 // Teal #00ABA9 RGB(0, 171, 169)
#define BLUEBOX_CYAN 0x1d1c // Cyan #1BA1E2 RGB(27, 161, 226)
#define BLUEBOX_COBALT 0x29d // Cobalt #0050EF RGB(0, 80, 239)
#define BLUEBOX_INDIGO 0x681f // Indigo #6A00FF RGB(106, 0, 255)
#define BLUEBOX_VIOLET 0xa81f // Violet #AA00FF RGB(170, 0, 255)
#define BLUEBOX_PINK 0xf39a // Pink #F472D0 RGB(244, 114, 208)
#define BLUEBOX_MAGENTA 0xd80e // Magenta #D80073 RGB(216, 0, 115)
#define BLUEBOX_CRIMSON 0xa004 // Crimson #A20025 RGB(162, 0, 37)
#define BLUEBOX_RED 0xe0a0 // Red #E51400 RGB(229, 20, 0)
#define BLUEBOX_ORANGE 0xfb40 // Orange #FA6800 RGB(250, 104, 0)
#define BLUEBOX_AMBER 0xf501 // Amber #F0A30A RGB(240, 163, 10)
#define BLUEBOX_YELLOW 0xe640 // Yellow #E3C800 RGB(227, 200, 0)
#define BLUEBOX_BROWN 0x82c5 // Brown #825A2C RGB(130, 90, 44)
#define BLUEBOX_OLIVE 0x6c2c // Olive #6D8764 RGB(109, 135, 100)
#define BLUEBOX_STEEL 0x63b0 // Steel #647687 RGB(100, 118, 135)
#define BLUEBOX_MAUVE 0x7311 // Mauve #76608A RGB(118, 96, 138)
#define BLUEBOX_TAUPE 0x83c9 // Taupe #87794E RGB(135, 121, 78)

#define BLUEBOX_GRAY1 0x2104
#define BLUEBOX_GRAY2 0x4208
#define BLUEBOX_GRAY3 0x630C
#define BLUEBOX_GRAY4 0x8410
#define BLUEBOX_GRAY5 0xa514
#define BLUEBOX_GRAY6 0xc618

#define BLUEBOX_BLACK 0x0000
#define BLUEBOX_WHITE 0xffff
#define BLUEBOX_BLUE  BLUEBOX_COBALT

extern const uint16_t bluebox_colors[];
extern const char* bluebox_color_names[];
extern const uint8_t BLUEBOX_COLOR_COUNT;

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
    void drawThickArcBase(uint16_t x, uint16_t y, uint16_t r, uint16_t lineThickness, uint16_t startAngle, uint16_t endAngle, uint16_t color);
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
    void drawRGBBitmap(int16_t x, int16_t y,
                       const uint16_t bitmap[],
                       int16_t w, int16_t h);

    void drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color, bool filled);
    void drawThickCircle(uint16_t x, uint16_t y, uint16_t r, uint16_t lineThickness, uint16_t color);

    void drawVLine(uint16_t x, uint16_t y1, uint16_t y2, uint16_t color);
    void drawHLine(uint16_t x1, uint16_t x2, uint16_t y, uint16_t color);
    void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);

    void drawThickArc(uint16_t x, uint16_t y, uint16_t r, uint16_t lineThickness, uint16_t startAngle, uint16_t endAngle, uint16_t color);

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
