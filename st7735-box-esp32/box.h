#ifndef _BOX_H_
#define _BOX_H_

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
  
#define LED1_PIN  1
#define LED2_PIN  0

#define BUZZER_PIN 2

#define BUTTON1_PIN  10
#define BUTTON2_PIN  20

#define BACKLIGHT_PIN 5

void setup_box()
{
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(BACKLIGHT_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
}

#endif
