// Source: https://github.com/ccarlson71/matrixportal_plasma

#include <Adafruit_Protomatter.h>
#include <math.h>

#define HEIGHT  32 // Matrix height (pixels) - SET TO 64 FOR 64x64 MATRIX!
#define WIDTH   64 // Matrix width (pixels)
#define MAX_FPS 45 // Maximum redraw rate, frames/second
#define SCALE_BRIGHTNESS 0.5

#define PLASMA_PALETTE_SIZE 240
#define PLASMA_COLORS 6

#define LIFE_INIT_DENSITY 30 // Express as percentage
#define LIFE_GENERATIONS 400


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

double plasma_current_value = 0.0;
double plasma_step_value = 0.3;
double plasma_randomizing_param;

Adafruit_Protomatter matrix(
  64, 6, 1, rgbPins, 4, addrPins, clockPin, latchPin, oePin, true); // true indicates double-buffering

uint16_t plasma_palette[PLASMA_PALETTE_SIZE];
uint16_t white_pixel = matrix.color565(255, 255, 255);
uint16_t black_pixel = matrix.color565(0, 0, 0);

double sin_lut[360];
double cos_lut[360];
double sqrt_lut[5120];

int reset_counter;
#define PLASMA_CYCLES_LIMIT 5000

// RANDOM SEED STUFF
// 99.9% from here: https://forum.arduino.cc/t/best-way-of-random-seed/640617/8
const byte analogPort = A1;

const int SEED_BUCKET_SIZE = 64;
int seed_buckets[SEED_BUCKET_SIZE];

long getRandomSeed(int numBits)
{
  // magic numbers tested 2016-03-28
  // try to speed it up
  // Works Well. Keep!
  //
  if (numBits > 31 or numBits < 1) numBits = 31; // limit input range

  const int baseIntervalMs = 1UL; // minumum wait time
  const byte sampleSignificant = 7;  // modulus of the input sample
  const byte sampleMultiplier = 10;   // ms per sample digit difference

  const byte hashIterations = 3;
  int intervalMs = 0;

  unsigned long reading;
  long result = 0;
  int tempBit = 0;

  Serial.print("randomizing...");
  pinMode(analogPort, INPUT_PULLUP);
  pinMode(analogPort, INPUT);
  delay(200);
  // Now there will be a slow decay of the voltage,
  // about 8 seconds
  // so pick a point on the curve
  // offset by the processed previous sample:
  //

  for (int bits = 0; bits < numBits; bits++)
  {
    Serial.print('*');

    for (int i = 0; i < hashIterations; i++)
    {
      //      Serial.print(' ');
      //      Serial.print( hashIterations - i );
      delay(baseIntervalMs + intervalMs);

      // take a sample
      reading = analogRead(analogPort);
      tempBit ^= reading & 1;

      // take the low "digits" of the reading
      // and multiply it to scale it to
      // map a new point on the decay curve:
      intervalMs = (reading % sampleSignificant) * sampleMultiplier;
    }

    result |= (long)(tempBit & 1) << bits;
  }

  Serial.println(result);
  //  Serial.println();
  return result;
}

// END RANDOM COLOR SEED STUFF

void randomize_plasma()
{
  randomize_plasma_palette();
  plasma_current_value = random(100000);
  plasma_randomizing_param = 10 + (random(750) / 50.0);
}

void randomize_seed()
{
  randomSeed(getRandomSeed(31));
}

double sin_l(double rads)
{
  int degs = int(rads * 57296 / 1000);
  int m_value = degs % 360;
  return sin_lut[m_value];
}

double cos_l(double rads)
{
  int degs = int(rads * 57296 / 1000);
  int m_value = degs % 360;
  return cos_lut[m_value];
}

double random_byte(void)
{
  return double(random(255));
}

void randomize_plasma_palette(void)
{
  // The first color is black. The rest are randomly determined
  // We should maybe enforce some distance between two colors?
  double anchor_colors[PLASMA_COLORS][3];
  for (int x = 0; x < 3; x++)
  {
    anchor_colors[0][x] = 0;
  }

  for (int row = 1; row < PLASMA_COLORS; row++)
  {
    for (int col = 0; col < 3; col++)
    {
      anchor_colors[row][col] = random_byte();
    }
  }

  int palette_steps = PLASMA_PALETTE_SIZE / PLASMA_COLORS;
  double deg_smoothing_step = 180.0 / palette_steps;

  for (int anchor_idx = 0; anchor_idx < PLASMA_COLORS; anchor_idx++)
  {
    int start_idx = anchor_idx * palette_steps;
    int stop_idx = start_idx + palette_steps;
    int to_idx = (anchor_idx + 1) % PLASMA_COLORS;

    for (int i = start_idx; i < stop_idx; i++)
    {
      int j = i % palette_steps;
      double degs = j * deg_smoothing_step;
      double rads = (degs * 71) / 4068.0;
      double f = 1.0 - ((cos(rads) + 1) / 2.0);

      plasma_palette[i] = interpolate_palette(anchor_colors[anchor_idx], anchor_colors[to_idx], f);
    }
  }
}

uint16_t interpolate_palette(double from[3], double to[3], double f)
{
  int red   = int((from[0] + ((to[0] - from[0]) * f)) * SCALE_BRIGHTNESS);
  int green = int((from[1] + ((to[1] - from[1]) * f)) * SCALE_BRIGHTNESS);
  int blue  = int((from[2] + ((to[2] - from[2]) * f)) * SCALE_BRIGHTNESS);

  return matrix.color565(red, green, blue);
}

void fast_plasma(double seed_value)
{
  // Uses lookup tables for lots of stuff
  for (int x = 0; x < WIDTH; x++)
  {
    for (int y = 0; y < HEIGHT; y++)
    {
      double dx = double(x);
      double dy = double(y) / 2.0;

      double calculated_value = sin_l((dx + seed_value) / plasma_randomizing_param) +
                                cos_l((dy + (seed_value / 2.0)) / 9.0) +
                                // sin_l(dy / 9.0) +
                                sin_l(((dx + seed_value) + (dy - seed_value)) / 25.0) +
                                sin_l(sqrt_lut[(y * 64) + x] / 8.0);
      int scaled_value = int(abs(calculated_value) * 100);
      int output_color = scaled_value % PLASMA_PALETTE_SIZE;

      matrix.drawPixel(x, y, plasma_palette[output_color]);
    }
  }
}

void run_plasma(void)
{
  reset_counter = 0;
  randomize_plasma();

  while (reset_counter < PLASMA_CYCLES_LIMIT)
  {
    matrix.fillScreen(0x0);
    fast_plasma(plasma_current_value);
    matrix.show();
    plasma_current_value += plasma_step_value;

    reset_counter += 1;
    Serial.println(PLASMA_CYCLES_LIMIT - reset_counter);
  }
}

void run_game_of_life(int reps)
{
  for (int rep = 0; rep < reps; rep++)
  {
    // Initialize
    Serial.println("Running Game of Life");
    int life_arrays[3][WIDTH * HEIGHT];
    Serial.println("Initializing random life array");
    for (int i = 0; i < (WIDTH * HEIGHT); i++) { life_arrays[0][i] = (random(100) < LIFE_INIT_DENSITY) ? 1 : 0; }
  
    int from_array = 0;
    int to_array, yyy, ym1, yp1, xm1, xp1, neighbors, new_value;
  
    Serial.println("Starting simulation");
  
    unsigned long int last_millis;
    
    for (int generation = 0; generation < LIFE_GENERATIONS; generation++)
    {
      last_millis = millis();
      int to_array = (from_array + 1) % 3;
      int live_count = 0;
      // Populate from-array
      for (int y = 0; y < HEIGHT; y++)
      {
        yyy = y * WIDTH;
        ym1 = ((y + HEIGHT - 1) % HEIGHT) * WIDTH;
        yp1 = ((y + 1) % HEIGHT) * WIDTH;
        xm1 = WIDTH - 1;
        for (int x = 0; x < WIDTH; x++)
        {
          xp1 = (x + 1) % WIDTH;
          neighbors = (
            life_arrays[from_array][xm1 + ym1] + life_arrays[from_array][xm1 + yyy] + life_arrays[from_array][xm1 + yp1] +
            life_arrays[from_array][x   + ym1] +                                      life_arrays[from_array][x   + yp1] +
            life_arrays[from_array][xp1 + ym1] + life_arrays[from_array][xp1 + yyy] + life_arrays[from_array][xp1 + yp1]
            );
          int self_value = life_arrays[from_array][x + yyy];
          new_value = ((neighbors == 3) || ((neighbors == 2) && (self_value == 1))) ? 1 : 0;
          live_count += new_value;
          life_arrays[to_array][x + yyy] = new_value;
          xm1 = x;
          matrix.drawPixel(x, y, (new_value == 1) ? white_pixel : black_pixel);    
        }
      }
          
      // Activate display
      matrix.show();
      if (live_count < 3)
      {
        break;
      }

      for (;;)
      {
        if (millis() > last_millis + 150)
        {
          break;
        }
      }
  
      Serial.println(LIFE_GENERATIONS - generation);
      from_array = to_array;
    }
  }
}

void setup(void)
{
  Serial.begin(9600);

  // Initialize matrix...
  ProtomatterStatus status = matrix.begin();
  Serial.print("Protomatter begin() status: ");
  Serial.println((int)status);
  if (status != PROTOMATTER_OK)
  {
    for (;;);
  }

  randomize_seed();

  // Initialize sine LUT
  Serial.println("Building sine LUT");
  for (int x = 0; x < 360; x++)
  {
    float x_rads = (x * 71) / 4068.0;
    sin_lut[x] = sin(x_rads);
  }

  // Initialize cosine LUT
  Serial.println("Building cosine LUT");
  for (int x = 0; x < 360; x++)
  {
    float x_rads = (x * 71) / 4068.0;
    cos_lut[x] = cos(x_rads);
  }

  // Initialize sqrt LUT
  Serial.println("Building sqrt LUT");
  for (int x = 0; x < 64; x++)
  {
    for (int y = 0; y < 32; y++)
    {
      int idx = (y * 64) + x;
      double dy = double(y) / 2.0;
      sqrt_lut[idx] = sqrt((x * x) + (dy * dy));
    }
  }
}

void loop(void)
{
  run_plasma();
  run_game_of_life(8);
}
