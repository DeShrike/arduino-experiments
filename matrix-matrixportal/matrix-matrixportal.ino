/* ----------------------------------------------------------------------
"Matrix" For MatrixPortal S3.
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
  WIDTH, 6, 1, rgbPins, NUM_ADDR_PINS, addrPins,
  clockPin, latchPin, oePin, true);

#define N_COLORS   8
#define COLOR_FLAVORS 6

uint16_t colors[COLOR_FLAVORS][N_COLORS];
uint32_t prevTime = 0; // Used for frames-per-second throttle
uint8_t intensities[N_COLORS] = { 255, 215, 185, 145, 105, 85, 40, 20 };

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

int8_t posy[WIDTH] = { 0 };
uint8_t len[WIDTH] = { 0 };
uint8_t speed[WIDTH] = { 0 };
uint8_t flavor[WIDTH] = { 0 };
uint8_t current_flavor = 0;
int count = 0;

void init_column(uint8_t x)
{
  len[x] = random(10, HEIGHT - 5);
  posy[x] = random(HEIGHT) - HEIGHT;
  speed[x] = random(3) + 1;
  if (current_flavor == COLOR_FLAVORS)
  {
    flavor[x] = random(COLOR_FLAVORS);
  }
  else
  {
    flavor[x] = current_flavor;
  }
}

void setup(void)
{
  //Serial.begin(9600);
  //while (!Serial) delay(10);

  randomSeed(analogRead(A0));

  ProtomatterStatus status = matrix.begin();
  // Serial.printf("Protomatter begin() status: %d\n", status);

  for (uint8_t c = 0; c < N_COLORS; c++)
  {
    colors[0][c] = matrix.color565( 0, intensities[c], 0); // Green
    colors[1][c] = matrix.color565( intensities[c], 0, 0); // Red
    colors[2][c] = matrix.color565( 0, 0, intensities[c]); // Blue
    colors[3][c] = matrix.color565( intensities[c], intensities[c], 0); // Yellow
    colors[4][c] = matrix.color565( intensities[c], 0, intensities[c]); // Purple
    colors[5][c] = matrix.color565( 0, intensities[c], intensities[c]); // Cyan
  }

  for (uint8_t x = 0; x < WIDTH; x++)
  {
	init_column(x);
  }
}

void process()
{
  for (uint8_t x = 0; x < WIDTH; x++)
  {
    posy[x] += speed[x];
    if (posy[x] - len[x] >= HEIGHT)
    {
      init_column(x);
    }
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

  uint8_t c;
  for (uint8_t x = 0; x < WIDTH; x++)
  {
    if (posy[x] >= 0 && posy[x] - len[x] < HEIGHT)
    {
      for (int y = posy[x]; y > posy[x] - len[x]; y--)
      {
        if (y >= 0 && y < HEIGHT)
        {
          c = posy[x] - y;
          if (c >= N_COLORS)
          {
            c = N_COLORS - 1;
          }

          matrix.drawPixel(x, y, colors[flavor[x]][c]);
        }
      }
    }
  }

  count++;
  if (count % 500 == 0)
  {
    current_flavor = (current_flavor + 1) % (COLOR_FLAVORS + 1);
  }

  matrix.show(); // Copy data to matrix buffers
}
