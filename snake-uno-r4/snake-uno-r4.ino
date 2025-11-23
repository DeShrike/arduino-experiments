// Include the LED_Matrix library
#include "Arduino_LED_Matrix.h"  

// Create an instance of the ArduinoLEDMatrix class
ArduinoLEDMatrix matrix;        

#define HEIGHT 8
#define WIDTH 12

// Define the frame array for the LED matrix with pixel values
uint8_t frame[HEIGHT][WIDTH] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

unsigned long lastTickTime;
#define UPDATE_INTERVAL 150

#define SNAKE_MAX_LENGTH 15
uint8_t currentLength;

uint8_t snakeX[SNAKE_MAX_LENGTH];
uint8_t snakeY[SNAKE_MAX_LENGTH];

uint8_t snakeHeadIndex = 0;

bool foodEaten = false;
bool foodPlaced = false;
uint8_t foodX = -1;
uint8_t foodY = -1;
uint8_t foodWait = 10;

// up, right, down, left
int8_t dirX[4] = { 0, 1, 0, -1 };
int8_t dirY[4] = { -1, 0, 1, 0 };

int8_t snakeDirection = 1;

void clear()
{
   for (uint8_t x = 0; x < WIDTH; ++x)
   {
      for (uint8_t y = 0; y < HEIGHT; ++y)
      {
         frame[y][x] = 0;
      }
   }
}

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
   int8_t ix = snakeHeadIndex;
   for (uint8_t i = 0; i < currentLength; ++i)
   {
      frame[snakeY[ix]][snakeX[ix]] = 1;
      ix--;
      if (ix < 0)
      {
         ix = SNAKE_MAX_LENGTH - 1;
      }
   }
}

void drawFood()
{
   if (foodPlaced)
   {
      frame[foodY][foodX] = 1;
   }
}

void checkFood()
{
   uint8_t headX = snakeX[snakeHeadIndex];
   uint8_t headY = snakeY[snakeHeadIndex];
   if (headX == foodX && headY == foodY)
   {
      //Serial.println("Food !!");
      foodPlaced = false;
      foodEaten = true;
      foodWait = 10;
   }
}

void placeFood()
{
   if (currentLength < SNAKE_MAX_LENGTH && foodPlaced == false)
   {
      if (foodWait > 0)
      {
         foodWait--;
         return;
      }

      do {
         foodX = random(WIDTH);
         foodY = random(HEIGHT);
      } while (occupied(foodX, foodY));
      foodPlaced = true;
      //Serial.println("Placed food");
   }
}

void moveSnake()
{
   uint8_t next = random(8);
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
      currentLength++;
      foodEaten = false;
      //Serial.print("Snake is now length ");
      //Serial.println(currentLength);
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

void setup()
{
   // Initialize serial communication and delaying for setup
   Serial.begin(9600);
   delay(1500);

   // Initialize the LED matrix
   matrix.begin();

   // Initialze snake
   snakeHeadIndex = 0;
   snakeX[snakeHeadIndex] = random(WIDTH);
   snakeY[snakeHeadIndex] = random(HEIGHT);
   currentLength = 1;

   // Initialize time tracking variables
   lastTickTime = millis();
}

void loop()
{
   // Track the current time
   unsigned long msNow = millis();

   // Update the game logic with a fixed interval
   if (msNow - lastTickTime > UPDATE_INTERVAL)
   {
      placeFood();
      clear();
      drawSnake();
      drawFood();
      moveSnake();
      checkFood();

      matrix.renderBitmap(frame, HEIGHT, WIDTH);
      lastTickTime = msNow;
   }
}
