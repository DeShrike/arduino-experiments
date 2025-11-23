/* ----------------------------------------------------------------------
"Snake" For MatrixPortal S3.
------------------------------------------------------------------------- */

#include <Wire.h>                 // For I2C communication
#include <Adafruit_Protomatter.h> // For RGB matrix

#define HEIGHT  32 // Matrix height (pixels) - SET TO 64 FOR 64x64 MATRIX!
#define WIDTH   64 // Matrix width (pixels)
#define MAX_FPS 45 // Maximum redraw rate, frames/second

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

#define SNAKE_MAX_LENGTH 200
#define MAX_FOOD 5
#define FOOD_WAIT 100

uint8_t currentLength;

uint8_t snakeX[SNAKE_MAX_LENGTH];
uint8_t snakeY[SNAKE_MAX_LENGTH];

uint8_t foodX[MAX_FOOD] = { 0 };
uint8_t foodY[MAX_FOOD] = { 0 };
uint8_t foodColorIndex[MAX_FOOD] = { 2, 5, 6, 4, 7 };
uint8_t snakeHeadIndex = 0;

bool foodEaten = false;
int foodWait = FOOD_WAIT;

// up, right, down, left
int8_t dirX[4] = { 0, 1, 0, -1 };
int8_t dirY[4] = { -1, 0, 1, 0 };

int8_t snakeDirection = 1;

bool occupied(uint8_t x, uint8_t y)
{
   uint8_t ix = snakeHeadIndex;
   for (uint8_t i = 0; i < currentLength; ++i)
   {
      if (snakeX[ix] == x && snakeY[ix] == y)
      {
         return true;
      }
      
      ix++;
      if (ix >= SNAKE_MAX_LENGTH)
      {
         ix = 0;
      }
   }
   
   return false;
}

void drawSnake()
{
   int16_t ix = snakeHeadIndex;
   for (uint8_t i = 0; i < currentLength; ++i)
   {
      matrix.drawPixel(snakeX[ix], snakeY[ix], colors[0]);
      ix--;
      if (ix < 0)
      {
         ix = SNAKE_MAX_LENGTH - 1;
      }
   }
}

void drawFood()
{
   for (int f = 0; f < MAX_FOOD; f++)
   {
      if (foodX[f] != 0)
      {
         matrix.drawPixel(foodX[f], foodY[f], colors[foodColorIndex[f]]);
      }
   }
}

void checkFood()
{
   uint8_t headX = snakeX[snakeHeadIndex];
   uint8_t headY = snakeY[snakeHeadIndex];
   for (int f = 0; f < MAX_FOOD; f++)
   {
      if (foodX[f] == 0)
      {
        continue;  
      }
      
      if (headX == foodX[f] && headY == foodY[f])
      {
         Serial.println("Food !!");
         foodEaten = true;
         foodWait = FOOD_WAIT;
         foodX[f] = 0;
      }
   }
}

void placeFood()
{
   int foodCount = 0;
   for (int f = 0; f < MAX_FOOD; f++)
   {
      if (foodX[f] != 0)
      {
         foodCount++;
      }
   }

   if (currentLength < SNAKE_MAX_LENGTH && foodCount < MAX_FOOD)
   {
      if (foodWait > 0)
      {
         foodWait--;
         return;
      }

      foodWait = FOOD_WAIT;

      for (int f = 0; f < MAX_FOOD; f++)
      {
         if (foodX[f] != 0)
         {
            continue;
         }
         
         do {
            foodX[f] = random(WIDTH);
            foodY[f] = random(HEIGHT);
         } while (occupied(foodX[f], foodY[f]));
         break;
         //Serial.println("Placed food");
      }
   }
}

void moveSnake()
{
   uint8_t next = random(15);
   if (next == 4) // turn left
   {
      snakeDirection--;
      if (snakeDirection < 0)
      {
         snakeDirection = 3;
      }
   }
   else if (next == 5) // turn right
   {
      snakeDirection++;
      if (snakeDirection > 3)
      {
         snakeDirection = 0;
      }
   }

   uint8_t headX = snakeX[snakeHeadIndex];
   uint8_t headY = snakeY[snakeHeadIndex];

   snakeHeadIndex++;
   if (snakeHeadIndex >= SNAKE_MAX_LENGTH)
   {
      snakeHeadIndex = 0;
   }

   if (foodEaten)
   {
      if (currentLength < SNAKE_MAX_LENGTH)
      {
        currentLength++;
      }
      else
      {
        currentLength = 1;
      }
      foodEaten = false;
      Serial.print("Snake is now length ");
      Serial.println(currentLength);
   }

   int8_t newX = (headX + dirX[snakeDirection]) % WIDTH;
   int8_t newY = (headY + dirY[snakeDirection]) % HEIGHT;

   if (newX < 0)
   {
      newX += WIDTH;
   }

   if (newY < 0)
   {
      newY += HEIGHT;
   }

   snakeX[snakeHeadIndex] = newX;
   snakeY[snakeHeadIndex] = newY;
}

void setup(void)
{
  Serial.begin(9600);
  //while (!Serial) delay(10);

  randomSeed(analogRead(A0));

  ProtomatterStatus status = matrix.begin();
  Serial.printf("Protomatter begin() status: %d\n", status);

  colors[0] = matrix.color565(64, 64, 64);  // Dark Gray
  colors[1] = matrix.color565(120, 79, 23); // Brown
  colors[2] = matrix.color565(228,  3,  3); // Red
  colors[3] = matrix.color565(255,140,  0); // Orange
  colors[4] = matrix.color565(255,237,  0); // Yellow
  colors[5] = matrix.color565(  0,128, 38); // Green
  colors[6] = matrix.color565(  0, 77,255); // Blue
  colors[7] = matrix.color565(117,  7,135); // Purple

   // Initialze snake
   snakeHeadIndex = 0;
   snakeX[snakeHeadIndex] = random(WIDTH);
   snakeY[snakeHeadIndex] = random(HEIGHT);
   currentLength = 1;
}

void loop()
{
  // Limit the animation frame rate to MAX_FPS.
  uint32_t t;
  while(((t = micros()) - prevTime) < (1000000L / MAX_FPS));
  prevTime = t;

  matrix.fillScreen(0x0);

   placeFood();
   drawSnake();
   drawFood();
   moveSnake();
   checkFood();

  matrix.show(); // Copy data to matrix buffers
}
