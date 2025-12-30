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

#define RGBto565(r,g,b) ((((r) & 0xF8) << 8) | (((g) & 0xFC) << 3) | ((b) >> 3)) 

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

#define SPACING 10
uint8_t y = 4;

uint8_t draw(uint8_t y, const char* label, const char* value)
{
  tft.setTextWrap(false);
  
  tft.setCursor(2, y);
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(1);
  tft.println(label);
  
  tft.setCursor(70, y);
  tft.setTextColor(ST7735_GREEN);
  tft.setTextSize(1);
  tft.println(value);
  y += SPACING;

  return y;
}

void setup()
{
  char temp[100];
  SPI.begin(SCK, MISO, MOSI);  

  tft.initR(INITR_BLACKTAB);  // Most 1.8" AZ-Delivery displays
  tft.fillScreen(ST7735_BLACK);
  delay(1000);

  tft.setRotation(1);

  tft.drawLine(tft.width()-1, tft.height()-1, 0, y, ST7735_WHITE);

  y = draw(y, "Processor:", ESP.getChipModel());

  sprintf(temp, "%d", ESP.getChipRevision());
  y = draw(y, "Revision:", temp);

  sprintf(temp, "%d", ESP.getChipCores());
  y = draw(y, "Cores:", temp);

  sprintf(temp, "%d MHz", ESP.getCpuFreqMHz());
  y = draw(y, "CPU Freq:", temp);

  sprintf(temp, "%d mb", ESP.getFlashChipSize() / (1024.0 * 1024));
  y = draw(y, "Flash:", temp);

  sprintf(temp, "%d mb", ESP.getPsramSize() / (1024.0 * 1024));
  y = draw(y, "PSRAM:", temp);

  sprintf(temp, "%d kb", ESP.getHeapSize() / 1024.0);
  y = draw(y, "HEAP:", temp);

  y = draw(y, "SDK:", ESP.getSdkVersion());
}

void loop()
{
  delay(1000);
}
