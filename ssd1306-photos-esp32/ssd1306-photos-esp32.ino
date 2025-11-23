#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "flower1.h"
#include "flower2.h"
#include "cat1.h"
#include "cat2.h"
#include "car.h"
#include "jane.h"
#include "butterfly.h"
#include "rsloppost.h"

#define SDA_PIN 8
#define SCL_PIN 9

#define LED     0

#if defined(ESP8266)
// 
#elif defined(ESP32)
// 
#else
// 
#endif

int count;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int ledstate = LOW;
int counter = 0;

void setup()
{
  Serial.begin(9600);
  delay(1000);

  pinMode(LED, OUTPUT);

  Wire.setPins(SDA_PIN, SCL_PIN);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(1000);

  // Clear the buffer
  display.clearDisplay();

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(1000);
}

void loop()
{
  counter += 1;

  if (counter % 10 == 0)
  {
    ledstate = ledstate == LOW ? HIGH : LOW;
    digitalWrite(LED, ledstate);
  }

   if (counter == 100)
   {
      display.clearDisplay();
      int x = (display.width() - BUTTERFLY_WIDTH) / 2;
      display.drawBitmap(x, 0, butterfly_bmp, BUTTERFLY_WIDTH, BUTTERFLY_HEIGHT, 1);
      display.display();
   }

   if (counter == 175)
   {
      display.clearDisplay();
      display.drawBitmap(0, 0, butterfly_bmp, BUTTERFLY_WIDTH, BUTTERFLY_HEIGHT, 1);
      display.drawBitmap(64, 0, butterfly_bmp, BUTTERFLY_WIDTH, BUTTERFLY_HEIGHT, 1);
      display.display();
   }

   if (counter == 225)
   {
      display.clearDisplay();
      display.drawBitmap(0, 0, flower1_bmp, FLOWER1_WIDTH, FLOWER1_HEIGHT, 1);
      display.display();
   }

   if (counter == 250 || counter == 300 || counter == 350)
   {
      display.clearDisplay();
      display.drawBitmap(0, 0, flower1_bmp, FLOWER1_WIDTH, FLOWER1_HEIGHT, 1);
      display.drawBitmap(64, 0, flower2_bmp, FLOWER2_WIDTH, FLOWER2_HEIGHT, 1);
      display.display();
   }

   if (counter == 275 || counter == 325 || counter == 375)
   {
      display.clearDisplay();
      display.drawBitmap(64, 0, flower1_bmp, FLOWER1_WIDTH, FLOWER1_HEIGHT, 1);
      display.drawBitmap(0, 0, flower2_bmp, FLOWER2_WIDTH, FLOWER2_HEIGHT, 1);
      display.display();
   }

   if (counter == 400)
   {
      display.clearDisplay();
      int x = (display.width() - CAT2_WIDTH) / 2;
      display.drawBitmap(x, 0, cat2_bmp, CAT2_WIDTH, CAT2_HEIGHT, 1);
      display.display();
   }

   if (counter == 500)
   {
      display.clearDisplay();
      display.drawBitmap(0, 0, cat1_bmp, CAT1_WIDTH, CAT1_HEIGHT, 1);
      display.display();
   }

   if (counter == 600)
   {
      display.clearDisplay();
      display.drawBitmap(0, 0, rsloppost_bmp, RSLOPPOST_WIDTH, RSLOPPOST_HEIGHT, 1);
      display.display();
   }

   if (counter == 700)
   {
      display.clearDisplay();
      display.drawBitmap(0, 0, flower1_bmp, FLOWER1_WIDTH, FLOWER1_HEIGHT, 1);
      display.drawBitmap(64, 0, flower1_bmp, FLOWER1_WIDTH, FLOWER1_HEIGHT, 1);
      display.display();
   }
   
   if (counter == 750)
   {
      display.clearDisplay();
      display.drawBitmap(0, 0, flower2_bmp, FLOWER2_WIDTH, FLOWER2_HEIGHT, 1);
      display.drawBitmap(64, 0, flower2_bmp, FLOWER2_WIDTH, FLOWER2_HEIGHT, 1);
      display.display();
   }

   if (counter == 800)
   {
      display.clearDisplay();
      display.drawBitmap(0, 0, jane_bmp, JANE_WIDTH, JANE_HEIGHT, 1);
      display.display();
   }

   if (counter == 900)
   {
      display.clearDisplay();
      display.drawBitmap(0, 0, car_bmp, CAR_WIDTH, CAR_HEIGHT, 1);
      display.display();
      counter = 0;
   }

   delay(25);
}
