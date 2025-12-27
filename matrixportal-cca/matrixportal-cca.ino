// Source: https://github.com/WubbGmbaa/CyclicCA-MatrixPortal

/* ----------------------------------------------------------------------
Implementation of 2D cyclic cellular automata algorithm for 32x32 HUB75
matrix on Adafruit Matrix Portal M4 using Protomatter.

http://www.bryangratz.net

See https://softologyblog.wordpress.com/2013/08/29/cyclic-cellular-automata/
and http://www.mirekw.com/ca/rullex_cycl.html

Further reading - "Cyclic Cellular Automata in Two Dimensions" by R. Fisch,
J. Gravner, D. Griffeath

Some code based on Adafruit "Protomatter Simple" example sketch

TODO:
- Replace magic number matrix size with global var
- Implement Moore neighborhoods
------------------------------------------------------------------------- */

#include <Adafruit_Protomatter.h>
#include <cmath> //for modulo


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

//CCA Rule Vars
/*
  examples:
  r2/t2/c6/nn - Squarish Spirals, Jason Rampe (softology, visions of chaos)
  r2/t5/c3/nn - Cubism, Jason Rampe
  r2/t3/c5/nn - Maps, Mirek Wojtowicz (CELLebration)
*/
int r = 3;
int t = 3;
int c = 6;

//global cell array decs
int grid[64][32];
int nextGrid[64][32];
int count = 0;
int h;

int brightness = 128; //max 255

// Matrix setup
Adafruit_Protomatter matrix(
  64,          // Width of matrix (or matrix chain) in pixels
  6,           // Bit depth, 1-6
  1, rgbPins,  // # of matrix chains, array of 6 RGB pins for each
  4, addrPins, // # of address pins (height is inferred), array of pins
  clockPin, latchPin, oePin, // Other matrix control pins
  false);      // No double-buffering here (see "doublebuffer" example)

// fill grid with random values 
void randomizeGrid()
{
  for (int x = 0; x < 64; x++)
  {
    for (int y = 0; y < 32; y++)
    {
      grid[x][y] = random(c);
    }
  }
}

// modulo function, but negatives mirrored. do not care about values less than negative range, so this suffices
int progMod(int a, int b)
{
  return (a + b) % b; 
}

// update screen with current grid values mapped to hue
void displayGrid()
{
  //write grid cells to matrix pixels
  for (int x = 0; x < matrix.width(); x++)
  {
    for(int y = 0; y < matrix.height(); y++)
    {
      matrix.drawPixel(x, y, matrix.colorHSV((((65535 * (grid[x][y]) / c) +h ) % 65535), 255, brightness)); //colorHSV first arg splits hue space evenly into c possible colors plus offset
    }
  }

  matrix.show(); // Copy data to matrix buffers
}

// main CA loop function
void cycleIterate()
{
  for (int x = 0; x < matrix.width(); x++)
  {
    for (int y = 0; y < matrix.height(); y++)
    {
      countPixels(x, y);
    }
  }

  // copy temp grid to main grid
  for (int x = 0; x < matrix.width(); x++)
  {
    for (int y = 0; y < matrix.height(); y++)
    {
      grid[x][y] = nextGrid[x][y];
    }
  }
}

// check ca rules for NN neighborhood
void countPixels(int xLoc, int yLoc)
{
  int pixCount = 0;
  
  //determine "eat" state for current test cell
  int nextVal;
  if (grid[xLoc][yLoc] == c - 1)
  {
    nextVal = 0;
  }
  else
  {
    nextVal = grid[xLoc][yLoc] + 1;
  }
  
  // test all cells in NN neighborhood for given range, iteratively count "eat" cells
  for (int n= 1 ; n <= r; n++)
  {
    for (int i = 0; i < n; i++)
    {
      if (grid[progMod((xLoc - (n - i)), 64)][progMod((yLoc + i), 32)] == nextVal)
      {
        pixCount++;
      }

      if (grid[progMod((xLoc + i), 64)][progMod((yLoc + (n-i)), 32)] == nextVal)
      {
        pixCount++;
      }

      if (grid[progMod((xLoc + (n - i)), 64)][progMod((yLoc - i), 32)] == nextVal)
      {
        pixCount++;
      }

      if (grid[progMod((xLoc - i), 64)][progMod((yLoc - (n - i)), 32)] == nextVal)
      {
        pixCount++;
      }
    }
  }

  // compare with threshold, write to temp grid
  if (pixCount >= t)
  {
    nextGrid[xLoc][yLoc] = nextVal;
  }
  else
  {
    nextGrid[xLoc][yLoc] = grid[xLoc][yLoc];
  }
}

// seed fresh grid, pick new colors, hold disp for 1s
void initGrid()
{
  randomizeGrid();
  h = random(65535);
  displayGrid();
  delay(1000);
}

void setup(void)
{
  Serial.begin(9600);
  pinMode(3, INPUT_PULLUP);
  
  // read floating pin for random seed
  randomSeed(analogRead(A4));

  // initialize matrix...
  ProtomatterStatus status = matrix.begin();
  Serial.print("Protomatter begin() status: ");
  Serial.println((int)status);
  if (status != PROTOMATTER_OK)
  {
    // DO NOT CONTINUE if matrix setup encountered an error.
    for(;;);
  }
  
  initGrid();
}

void loop(void)
{
  if (digitalRead(3) == LOW)
  { 
    // Matrix portal "DOWN" button re-seeds grid at any time
    initGrid();
  }
  
  count++;
  if (count % 400 == 0)
  {
    initGrid();
  }
  
  cycleIterate(); //Do CA
  displayGrid(); //Refresh screen
  delay(50); //200fps cap
}
