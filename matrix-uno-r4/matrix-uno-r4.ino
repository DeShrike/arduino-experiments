// Include the R4Text library
#include "R4Matrix.h"

unsigned long lastTickTime;
#define UPDATE_INTERVAL 90

R4Matrix r4matrix;

int count = 0;
int mode = 0;
int number = 0;
bool led = false;

void setup()
{
   // Initialize serial communication and delaying for setup
   Serial.begin(9600);
   delay(1500);

   pinMode(LED_BUILTIN, OUTPUT);

   r4matrix.begin();
   r4matrix.setRotated180(false);

   // Initialize time tracking variables
   lastTickTime = millis();
}

void loop()
{
   // Track the current time
   unsigned long msNow = millis();

   // Update with a fixed interval
   if (msNow - lastTickTime > UPDATE_INTERVAL)
   {
      count += 1;
      if (count > 20)
      {
         if (mode < 3)
         {
            mode += 1;
            count = 0;
            led = !led;
            digitalWrite(LED_BUILTIN, led ? HIGH : LOW);
         }
      }

      if (mode == 0)
      {
         r4matrix.fill(0);
         r4matrix.drawCharChar('4', '2');
      }
      else if (mode == 1)
      {
         r4matrix.fill(0);
         r4matrix.drawChar(2);
      }
      else if (mode == 2)
      {
         r4matrix.fill(0);
         r4matrix.drawCharChar(11, 12);
      }
      else if (mode == 3)
      {
         r4matrix.startScrollString("Codedojo Roeselare 2025 - RslOpPost");
         mode = 4;
      }
      else if (mode == 4)
      {
         bool scrolldone = r4matrix.scroll();
         if (scrolldone)
         {
            mode = 5;
         }
      }
      else if (mode == 5)
      {
         r4matrix.drawNumber(number);
         number++;
         if (number > 999)
         {
            number = 0;
            mode = 6;
         }
      }
      else if (mode == 6)
      {
         r4matrix.startScrollString("Above All:Be Cool.");
         mode = 7;
      }
      else if (mode == 7)
      {
         bool scrolldone = r4matrix.scroll();
         if (scrolldone)
         {
            mode = 0;
         }
      }

      lastTickTime = msNow;
   }
}
