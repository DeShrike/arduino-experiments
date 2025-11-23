/* ----------------------------------------------------------------------
"Game Of Life" For MatrixPortal S3.
------------------------------------------------------------------------- */

#include <Wire.h>                 // For I2C communication
#include <Adafruit_Protomatter.h> // For RGB matrix

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
  WIDTH, 4, 1, rgbPins, NUM_ADDR_PINS, addrPins,
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

uint8_t frame1[HEIGHT * WIDTH] = { 0 };
uint8_t frame2[HEIGHT * WIDTH] = { 0 };

#define IX(x,y) (((y) * WIDTH) + (x))

uint8_t *current_grid = frame1;
uint8_t *other_grid = frame2;

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

  init_gol();
}

void init_gol()
{
  for (uint8_t y = 0; y < HEIGHT; y++)
  {
    for (uint8_t x = 0; x < WIDTH; x++)
    {
      if (current_grid[IX(x, y)] == 0)
      {
        current_grid[IX(x, y)] = random(20) < 15 ? 0 : 1;
      }
    }
  }
}

uint8_t count_neighbours(uint8_t x, uint8_t y)
{
  uint8_t c = 0;
  int8_t xx, yy;
  for (int8_t dx = -1; dx <= 1; dx++)
  {
    for (int8_t dy = -1; dy <= 1; dy++)
    {
      if (dx == 0 && dy == 0)
      {
        continue;
      }

      xx = (x + dx + WIDTH) % WIDTH;
      yy = (y + dy + HEIGHT) % HEIGHT;

      if (other_grid[IX(xx, yy)] > 0)
      {
        c++;
      }
    } 
  }

  return c;
}

void eliminate_plusses()
{
  uint8_t cc, oc, n;
  for (uint8_t y = 0; y < HEIGHT; y++)
  {
    for (uint8_t x = 0; x < WIDTH; x++)
    {
      cc = current_grid[IX(x, y)];
      oc = other_grid[IX(x, y)];
      n = count_neighbours(x, y);

      if (cc > 0 && oc > 0 && n == 2)
      {
        if (
         current_grid[IX(x - 1, y)] > 0 &&
         current_grid[IX(x + 1, y)] > 0 &&
         other_grid[IX(x, y - 1)] > 0 &&
         other_grid[IX(x, y + 1)] > 0 )
        {
          current_grid[IX(x, y)] = 0;
        }
      }
    }
  }
}

void process()
{
  uint8_t n, c, nc;
  int changed = 0;

  uint8_t* t = current_grid;
  current_grid = other_grid;
  other_grid = t;

  for (uint8_t y = 0; y < HEIGHT; y++)
  {
    for (uint8_t x = 0; x < WIDTH; x++)
    {
      c = other_grid[IX(x, y)];
      current_grid[IX(x, y)] = 0;
      n = count_neighbours(x, y);
      nc = 0;
      if ((c > 0 ) && (n == 2 || n == 3))
      {
        nc = 1;
      }
      else if (c == 0 && n == 3)
      {
        nc = 1;
      }

      if (nc == 1)
      {
        current_grid[IX(x, y)] = (nc + c) % N_COLORS;
      }

      if ((c > 0) != (nc > 0))
      {
        changed++;
      }
    }
  }

  if (changed < 20)
  {
    init_gol();
  }
  else if (changed < 100)
  {
    eliminate_plusses();
  }
}

void loop()
{
  // Limit the animation frame rate to MAX_FPS.
  uint32_t t;
  while(((t = micros()) - prevTime) < (1000000L / MAX_FPS));
  prevTime = t;

  matrix.fillScreen(0x0);

  process();

  for (uint8_t y = 0; y < HEIGHT; y++)
  {
    for (uint8_t x = 0; x < WIDTH; x++)
    {
      uint8_t c = current_grid[IX(x, y)];
      if (c > 0)
      {
        matrix.drawPixel(x, y, colors[c]);
      }
    }
  }

  matrix.show(); // Copy data to matrix buffers
}
