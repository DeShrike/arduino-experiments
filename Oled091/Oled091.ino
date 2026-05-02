// https://www.instructables.com/Tutorial-to-Interface-OLED-091inch-128x32-With-Ard/

#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>

// Arduino Nano
// GND to GND
// VCC to 3.3V
// SCK to A5
// SDA to A4

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0); 

void setup(void) 
{
   u8g2.begin();
}

void loop(void) 
{
   u8g2.clearBuffer();					// clear the internal memory
   u8g2.setFont(u8g2_font_logisoso28_tr);  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall
   u8g2.drawStr(8,29,"MYBOTIC");	// write something to the internal memory
   u8g2.sendBuffer();					// transfer internal memory to the display
   delay(3000);

   u8g2.clearBuffer();         // clear the internal memory
   u8g2.setFont(u8g2_font_logisoso28_tr);  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall
   u8g2.drawStr(31,24,"your");  // write something to the internal memory
   u8g2.sendBuffer();         // transfer internal memory to the display
   delay(800);

   u8g2.clearBuffer();         // clear the internal memory
   u8g2.setFont(u8g2_font_logisoso28_tr);  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall
   u8g2.drawStr(10,29,"robotic");  // write something to the internal memory
   u8g2.sendBuffer();         // transfer internal memory to the display
   delay(800);

   u8g2.clearBuffer();         // clear the internal memory
   u8g2.setFont(u8g2_font_logisoso28_tr);  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall
   u8g2.drawStr(4,29,"solution");  // write something to the internal memory
   u8g2.sendBuffer();         // transfer internal memory to the display
   delay(2000);

   u8g2.drawLine(0, 0, 127, 31);
   u8g2.drawLine(0, 31, 127, 0);
   u8g2.sendBuffer();
   delay(2000);

   u8g2.clearBuffer();
   u8g2.setDrawColor(2);
   u8g2.drawLine(0, 0, 127, 31);
   u8g2.drawLine(0, 31, 127, 0);
   u8g2.sendBuffer();
   delay(4000);

   u8g2.setDrawColor(2);
   u8g2.drawLine(0, 5, 127, 31);
   u8g2.drawLine(0, 31, 127, 5);
   u8g2.sendBuffer();
   delay(4000);
}
