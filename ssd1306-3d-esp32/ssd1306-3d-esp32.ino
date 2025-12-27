#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "3d.h"
#include "cube.h"
#include "pyramid.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

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
uint8_t shape = 0;

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

  // Clear the buffer
  display.clearDisplay();

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(1000);
}

void setPixel(uint8_t x, uint8_t y)
{
	display.drawPixel(x, y, SSD1306_WHITE);
}

void drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
   display.drawLine(x1, y1, x2, y2, SSD1306_WHITE);
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
  counter += 1;

  if (counter % 10 == 0)
  {
    ledstate = ledstate == LOW ? HIGH : LOW;
    digitalWrite(LED, ledstate);
  }

  if (counter % 100 == 0)
  {
    shape = (shape + 1) % 2;
  }

  display.clearDisplay();

  switch (shape) {
    case 0:
      draw_shape(cube, ARRAY_SIZE(cube), cube_faces, ARRAY_SIZE(cube_faces));
      break;
    case 1:
      draw_shape(pyramid, ARRAY_SIZE(pyramid), pyramid_faces, ARRAY_SIZE(pyramid_faces));
      break;  
  }

  display.display();

  angle += dangle;

  delay(25);
}
