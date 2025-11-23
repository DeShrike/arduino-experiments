#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#define LED_RED 4
#define LED_GREEN 3
#define LED_ORANGE 2

int i = 0;
char buffer[100];

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);  // Adafruit ESP8266/32u4/ARM Boards + FeatherWing OLED

void setup(void)
{
  u8g2.begin();
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_ORANGE, OUTPUT);
}

void loop(void)
{
  u8g2.clearBuffer();					// clear the internal memory
  u8g2.setFont(u8g2_font_6x10_tr);	// choose a suitable font

  sprintf(buffer, "Counter %d !!!", i);
  u8g2.drawStr(0,10, buffer);	// write something to the internal memory
  u8g2.sendBuffer();					// transfer internal memory to the display
  delay(100);  

  if (i % 7 == 0)
  {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else
  {
    digitalWrite(LED_BUILTIN, LOW);
  }

  if (i % 3 == 0)
  {
    digitalWrite(LED_RED, HIGH);
  }
  else
  {
    digitalWrite(LED_RED, LOW);
  }

  if (i % 5 == 0)
  {
    digitalWrite(LED_GREEN, HIGH);
  }
  else
  {
    digitalWrite(LED_GREEN, LOW);
  }

  if (i % 11 == 0)
  {
    digitalWrite(LED_ORANGE, HIGH);
  }
  else
  {
    digitalWrite(LED_ORANGE, LOW);
  }

  i++;
}
