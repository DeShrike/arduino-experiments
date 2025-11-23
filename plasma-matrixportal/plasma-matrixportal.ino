/* ----------------------------------------------------------------------
"Plasma" For MatrixPortal S3.
------------------------------------------------------------------------- */

#include <Wire.h>                 // For I2C communication
#include <Adafruit_Protomatter.h> // For RGB matrix
#include "Perlin.h"

#define HEIGHT  32 // Matrix height (pixels) - SET TO 64 FOR 64x64 MATRIX!
#define WIDTH   64 // Matrix width (pixels)
#define MAX_FPS 25 // Maximum redraw rate, frames/second

#if defined(_VARIANT_MATRIXPORTAL_M4_) // MatrixPortal M4
uint8_t rgbPins[]  = {7, 8, 9, 10, 11, 12};
uint8_t addrPins[] = {17, 18, 19, 20, 21};
uint8_t clockPin   = 14;
uint8_t latchPin   = 15;
uint8_t oePin      = 16;
#else // MatrixPortal ESP32-S3
uint8_t rgbPins[]  = {42, 41, 40, 38, 39, 37};
uint8_t addrPins[] = {45, 36, 48, 35, 21};
uint8_t clockPin   = 2;
uint8_t latchPin   = 47;
uint8_t oePin      = 14;
#endif

#if HEIGHT == 16
#define NUM_ADDR_PINS 3
#elif HEIGHT == 32
#define NUM_ADDR_PINS 4
#elif HEIGHT == 64
#define NUM_ADDR_PINS 5
#endif

Adafruit_Protomatter matrix(
  WIDTH, 6, 1, rgbPins, NUM_ADDR_PINS, addrPins,
  clockPin, latchPin, oePin, true);

#define N_COLORS   8

uint16_t colors[N_COLORS];
uint32_t prevTime = 0; // Used for frames-per-second throttle

void err(int x)
{
  uint8_t i;
  pinMode(LED_BUILTIN, OUTPUT);       // Using onboard LED
  for(i=1;;i++)
  {
    digitalWrite(LED_BUILTIN, i & 1); // LED on/off blink to alert user
    delay(x);
  }
}

Perlin p;

#define FEATURE_SIZE 10

void setup(void)
{
  Serial.begin(9600);
  //while (!Serial) delay(10);

  randomSeed(analogRead(A0));

  ProtomatterStatus status = matrix.begin();
  Serial.printf("Protomatter begin() status: %d\n", status);

  colors[0] = matrix.color565(32, 32, 32);  // Light Gray
  colors[1] = matrix.color565(64, 64, 64);  // Dark Gray
  colors[2] = matrix.color565(228,  3,  3); // Red
  colors[3] = matrix.color565(255,140,  0); // Orange
  colors[4] = matrix.color565(255,237,  0); // Yellow
  colors[5] = matrix.color565(  0,128, 38); // Green
  colors[6] = matrix.color565(  0, 77,255); // Blue
  colors[7] = matrix.color565(117,  7,135); // Purple
}

float dr = 0.05;
float dg = 0.05;
float db = 0.05;

float offr = 0.0;
float offg = 0.0;
float offb = 0.0;

void loop()
{
  // Limit the animation frame rate to MAX_FPS.
  uint32_t t;
  while(((t = micros()) - prevTime) < (1000000L / MAX_FPS));
  prevTime = t;

  float r = 0.0, g = 0.0, b = 0.0;
  int rr, gg, bb;

  for (uint8_t y = 0; y < HEIGHT; y++)
  {
    for (uint8_t x = 0; x < WIDTH; x++)
    {
        r = p.noise(
                (float) x / FEATURE_SIZE, 
                (float) (y + HEIGHT) / FEATURE_SIZE,
                offr);
        g = p.noise( 
                (float) (x + WIDTH) / FEATURE_SIZE, 
                (float) y / FEATURE_SIZE,
                offg);
        b = p.noise( 
                (float) x / FEATURE_SIZE, 
                (float) y / FEATURE_SIZE,
                offb);

        //rr = (int)((r + 1.0) * 64); // This is correct...
        //gg = (int)((g + 1.0) * 64);
        //bb = (int)((b + 1.0) * 64);
        rr = (int)((r + 0.0) * 127); // ... but this looks better
        gg = (int)((g + 0.0) * 127);
        bb = (int)((b + 0.0) * 127);
        matrix.drawPixel(x, y, matrix.color565(rr, gg, bb));
    }
  }

  offr += dr;
  offg += dg;
  offb += db;

  matrix.show(); // Copy data to matrix buffers
}
