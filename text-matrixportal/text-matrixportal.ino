/* ----------------------------------------------------------------------
Text/Font Demo
------------------------------------------------------------------------- */

#include <Adafruit_Protomatter.h>
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/FreeSans12pt7b.h>

/* ----------------------------------------------------------------------
The RGB matrix must be wired to VERY SPECIFIC pins, different for each
microcontroller board. This first section sets that up for a number of
supported boards.
------------------------------------------------------------------------- */

#if defined(_VARIANT_MATRIXPORTAL_M4_) // MatrixPortal M4
  uint8_t rgbPins[]  = {7, 8, 9, 10, 11, 12};
  uint8_t addrPins[] = {17, 18, 19, 20, 21};
  uint8_t clockPin   = 14;
  uint8_t latchPin   = 15;
  uint8_t oePin      = 16;
#elif defined(ARDUINO_ADAFRUIT_MATRIXPORTAL_ESP32S3) // MatrixPortal ESP32-S3
  uint8_t rgbPins[]  = {42, 41, 40, 38, 39, 37};
  uint8_t addrPins[] = {45, 36, 48, 35, 21};
  uint8_t clockPin   = 2;
  uint8_t latchPin   = 47;
  uint8_t oePin      = 14;
#elif defined(_VARIANT_FEATHER_M4_) // Feather M4 + RGB Matrix FeatherWing
  uint8_t rgbPins[]  = {6, 5, 9, 11, 10, 12};
  uint8_t addrPins[] = {A5, A4, A3, A2};
  uint8_t clockPin   = 13;
  uint8_t latchPin   = 0;
  uint8_t oePin      = 1;
#elif defined(__SAMD51__) // M4 Metro Variants (Express, AirLift)
  uint8_t rgbPins[]  = {6, 5, 9, 11, 10, 12};
  uint8_t addrPins[] = {A5, A4, A3, A2};
  uint8_t clockPin   = 13;
  uint8_t latchPin   = 0;
  uint8_t oePin      = 1;
#elif defined(_SAMD21_) // Feather M0 variants
  uint8_t rgbPins[]  = {6, 7, 10, 11, 12, 13};
  uint8_t addrPins[] = {0, 1, 2, 3};
  uint8_t clockPin   = SDA;
  uint8_t latchPin   = 4;
  uint8_t oePin      = 5;
#elif defined(NRF52_SERIES) // Special nRF52840 FeatherWing pinout
  uint8_t rgbPins[]  = {6, A5, A1, A0, A4, 11};
  uint8_t addrPins[] = {10, 5, 13, 9};
  uint8_t clockPin   = 12;
  uint8_t latchPin   = PIN_SERIAL1_RX;
  uint8_t oePin      = PIN_SERIAL1_TX;
#elif USB_VID == 0x239A && USB_PID == 0x8113 // Feather ESP32-S3 No PSRAM
  // M0/M4/RP2040 Matrix FeatherWing compatible:
  uint8_t rgbPins[]  = {6, 5, 9, 11, 10, 12};
  uint8_t addrPins[] = {A5, A4, A3, A2};
  uint8_t clockPin   = 13; // Must be on same port as rgbPins
  uint8_t latchPin   = RX;
  uint8_t oePin      = TX;
#elif USB_VID == 0x239A && USB_PID == 0x80EB // Feather ESP32-S2
  // M0/M4/RP2040 Matrix FeatherWing compatible:
  uint8_t rgbPins[]  = {6, 5, 9, 11, 10, 12};
  uint8_t addrPins[] = {A5, A4, A3, A2};
  uint8_t clockPin   = 13; // Must be on same port as rgbPins
  uint8_t latchPin   = RX;
  uint8_t oePin      = TX;
#elif defined(ESP32)
  // 'Safe' pins, not overlapping any peripherals:
  // GPIO.out: 4, 12, 13, 14, 15, 21, 27, GPIO.out1: 32, 33
  // Peripheral-overlapping pins, sorted from 'most expendible':
  // 16, 17 (RX, TX)
  // 25, 26 (A0, A1)
  // 18, 5, 9 (MOSI, SCK, MISO)
  // 22, 23 (SCL, SDA)
  uint8_t rgbPins[]  = {4, 12, 13, 14, 15, 21};
  uint8_t addrPins[] = {16, 17, 25, 26};
  uint8_t clockPin   = 27; // Must be on same port as rgbPins
  uint8_t latchPin   = 32;
  uint8_t oePin      = 33;
#elif defined(ARDUINO_TEENSY40)
  uint8_t rgbPins[]  = {15, 16, 17, 20, 21, 22}; // A1-A3, A6-A8, skip SDA,SCL
  uint8_t addrPins[] = {2, 3, 4, 5};
  uint8_t clockPin   = 23; // A9
  uint8_t latchPin   = 6;
  uint8_t oePin      = 9;
#elif defined(ARDUINO_TEENSY41)
  uint8_t rgbPins[]  = {26, 27, 38, 20, 21, 22}; // A12-14, A6-A8
  uint8_t addrPins[] = {2, 3, 4, 5};
  uint8_t clockPin   = 23; // A9
  uint8_t latchPin   = 6;
  uint8_t oePin      = 9;
#elif defined(ARDUINO_ADAFRUIT_FEATHER_RP2040)
  // RP2040 support requires the Earle Philhower board support package;
  // will not compile with the Arduino Mbed OS board package.
  // The following pinout works with the Adafruit Feather RP2040 and
  // original RGB Matrix FeatherWing (M0/M4/RP2040, not nRF version).
  // Pin numbers here are GP## numbers, which may be different than
  // the pins printed on some boards' top silkscreen.
  uint8_t rgbPins[]  = {8, 7, 9, 11, 10, 12};
  uint8_t addrPins[] = {25, 24, 29, 28};
  uint8_t clockPin   = 13;
  uint8_t latchPin   = 1;
  uint8_t oePin      = 0;
#endif

Adafruit_Protomatter matrix(
  64,          // Matrix width in pixels
  4,           // Bit depth -- 6 here provides maximum color options
  1, rgbPins,  // # of matrix chains, array of 6 RGB pins for each
  4, addrPins, // # of address pins (height is inferred), array of pins
  clockPin, latchPin, oePin, // Other matrix control pins
  true);       // HERE IS THE MAGIC FOR DOUBLE-BUFFERING!


int16_t  textX1;        // Current text position (X)
int16_t  textY1;        // Current text position (Y)
int16_t  textMin1;      // Text pos. (X) when scrolled off left edge
char     str1[64];      // Buffer to hold scrolling message text
uint16_t color1;

int16_t  textX2;        // Current text position (X)
int16_t  textY2;        // Current text position (Y)
int16_t  textMin2;      // Text pos. (X) when scrolled off left edge
char     str2[64];      // Buffer to hold scrolling message text
uint16_t color2;

void setup(void)
{
  Serial.begin(9600);

  // Initialize matrix...
  ProtomatterStatus status = matrix.begin();
  Serial.print("Protomatter begin() status: ");
  Serial.println((int)status);
  if(status != PROTOMATTER_OK)
  {
    // DO NOT CONTINUE if matrix setup encountered an error.
    for(;;);
  }

  color1 = matrix.color565(228,  3,  3); // Red
  color2 = matrix.color565(255, 237,  0); // Yellow

  int16_t  x1, y1;
  uint16_t w, h;

  // Set up the scrolling message...
  sprintf(str1, "Coderdojo Belgium %dx%d", matrix.width(), matrix.height()); 
  matrix.setFont(&FreeSansBold18pt7b); // Use nice bitmap font 
  matrix.setTextWrap(false); // Allow text off edge matrix.setTextColor(0xFFFF); 

  matrix.getTextBounds(str1, 0, 0, &x1, &y1, &w, &h); // How big is it?
  textMin1 = -w; // All text is off left edge when it reaches this point
  textX1 = matrix.width(); // Start off right edge
  textY1 = matrix.height() / 2 - (y1 + h / 2); // Center text vertically

  sprintf(str2, "Coderdojo Belgium %dx%d", matrix.width(), matrix.height());
  matrix.setFont(&FreeSans12pt7b); // Use nice bitmap font
  matrix.setTextWrap(false);           // Allow text off edge

  matrix.getTextBounds(str2, 0, 0, &x1, &y1, &w, &h); // How big is it?
  textMin2 = -w; // All text is off left edge when it reaches this point
  textX2 = matrix.width(); // Start off right edge
  textY2 = matrix.height() / 2 - (y1 + h / 2); // Center text vertically
}

// LOOP - RUNS REPEATEDLY AFTER SETUP --------------------------------------

void loop(void)
{
  matrix.fillScreen(0); // Fill background black

  // Draw the big scrolly text
  matrix.setFont(&FreeSansBold18pt7b); // Use nice bitmap font
  matrix.setTextColor(color1);
  matrix.setCursor(textX1, textY1);
  matrix.print(str1);

  // Draw the big scrolly text
  matrix.setFont(&FreeSans12pt7b); // Use nice bitmap font
  matrix.setTextColor(color2);
  matrix.setCursor(textX2, textY2);
  matrix.print(str2);

  // Update text position for next frame. If text goes off the
  // left edge, reset its position to be off the right edge.
  if ((--textX1) < textMin1) textX1 = matrix.width();
  if ((--textX2) < textMin2) textX2 = matrix.width();

  matrix.show();

  delay(20); // 20 milliseconds = ~50 frames/second
}
