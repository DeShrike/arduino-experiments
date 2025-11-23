#include <Arduino.h>

/*
On NodeMCU v3: 1138352 ms
On Arduino Nano: 1379961 ms
On Arduino UNO R4 Wifi: 1649685 ms
On ESP32 C3 Super Mini: 1086913 ms
*/

bool done = false;
unsigned long count = 0;
unsigned long number = 3;
char temp[50];
unsigned long startTime;

#if !defined(LED_BUILTIN)
#define LED_BUILTIN 0
#endif

void setup(void)
{
   pinMode(LED_BUILTIN, OUTPUT);

   Serial.begin(9600);
   Serial.println("Finding first 20000 primes");
   delay(5000);
   
   startTime = millis();
}

bool isPrime(unsigned long number)
{
   for (unsigned long i = 2; i < sqrt(number); ++i)
   {
      if (number % i == 0)
      {
         return false;
      }
   }
   
   return true;
}

void loop(void)
{
   if (done)
   {
      delay(100);
      return;
   }

   digitalWrite(LED_BUILTIN, LOW);

   if (isPrime(number))
   {
      digitalWrite(LED_BUILTIN, HIGH);
      count++;
      sprintf(temp, "Prime #%ld = %ld", count, number);
      Serial.println(temp);
      if (count == 20000)
      {
         sprintf(temp, "Done. Took %ld ms", millis() - startTime);
         Serial.println(temp);
         done = true;
         digitalWrite(LED_BUILTIN, LOW);
      }

      delay(50);
   }

   number += 2;
}
