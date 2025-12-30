#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

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

#define SPACING1 20
#define SPACING2 19
uint8_t y = 4;
uint8_t color = 0;
uint8_t ledstate = LOW;

#define LED_PIN  5

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

uint8_t draw(uint8_t y)
{
  tft.setCursor(10, y);
  tft.setTextColor(colors[color]);
  tft.setTextSize(2);
  tft.println("CoderDojo");
  y += SPACING1;
  color = (color + 1) % COLOR_COUNT;
  
  tft.setCursor(8, y);
  tft.setTextColor(colors[color]);
  tft.setTextSize(1);
  tft.println("ESP32 C3 Super Mini");
  y += SPACING2;
  color = (color + 1) % COLOR_COUNT;

  return y;
}

void setup()
{
  SPI.begin(SCK, MISO, MOSI);  

  tft.initR(INITR_BLACKTAB);  // Most 1.8" AZ-Delivery displays
  tft.fillScreen(ST7735_BLACK);
  delay(1000);
  
  pinMode(LED_PIN, OUTPUT);
}

void loop()
{
  y = 4;
  y = draw(y);
  y = draw(y);
  y = draw(y);
  y = draw(y);
  color = (color + 1) % COLOR_COUNT;

  digitalWrite(LED_PIN, ledstate);
  ledstate = ledstate == HIGH ? LOW : HIGH;
  delay(1000);
}
