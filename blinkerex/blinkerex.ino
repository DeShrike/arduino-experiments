#include <arduino.h>

const int ledPin = LED_BUILTIN;   // the number of the LED pin
int ledState = LOW;
unsigned long previousMillis = 0;
long interval = 1000;
bool blinking = true;
long blinkCount = 0;
String inputString = "";
bool stringComplete = false;

void serialEvent() 
{
  while (Serial.available() > 0) 
  {
    // get the new byte:
    char inChar = (char)Serial.read();

    // add it to the inputString:
    if (inChar == 13 || inChar == 10)   // CRLF
    {
      // if LF, string complete
      if (inChar == 10) 
      {
        stringComplete = true;
      }
    } 
    else 
    {
      inputString += inChar;
    }
  }
}

void setup() 
{
  pinMode(ledPin, OUTPUT);

  Serial.begin(9600);
  Serial.setTimeout(50); 
  inputString.reserve(100);


  // Timers: https://maker.pro/arduino/projects/timer-interrupts-improve-your-arduino-programming-skills
  // cli();           // disable all interrupts
  // TCCR1A = 0;
  // TCCR1B = 0;
  // TCNT1  = 0;
  // OCR1A = 31250;          // compare match register 16MHz/256/2 = 31250
  // TCCR1B = (1 << WGM12);  // CTC mode
  // TCCR1B = (1 << CS12);   // // Frequency 16Mhz / 256 = 62500 
  // TIMSK1 = (1 << OCIE1A); // Local interruption OCIE1A
  // sei();                  // enable all interrupts
}

// ISR(TIMER1_OVF_vect)
// {
//   // do something here
// }

void loop() 
{
  if (stringComplete) 
  {
    if (inputString == "START BLINKING") 
    {
      blinking = true;
    }
    else if (inputString == "STOP BLINKING") 
    {
      blinking = false;
    }
    else if (inputString == "STATUS") 
    {
      char buffer[100];
      sprintf(buffer, "%sBLINKING - INTERVAL: %ld - TOTAL BLINKS: %ld", blinking ? "" : "NOT ", interval, blinkCount);
      Serial.println(buffer);
    }
    else if (inputString == "BLINK FASTER") 
    {
      int d = interval > 100 ? 100 : 10;
      interval = max(interval - d, 10);
    }
    else if (inputString == "BLINK SLOWER") 
    {
      int d = interval > 100 ? 100 : 10;
      interval = min(interval + d, 2000);
    }

    inputString = "";
    stringComplete = false;
  }

  if (blinking)
  {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) 
    {
      previousMillis = currentMillis;

      if (ledState == LOW) 
      {
        ledState = HIGH;
      } 
      else 
      {
        ledState = LOW;
      }

      blinkCount++;
      if ((blinkCount % 1000) == 0)
      {
        char buffer[50];
        sprintf(buffer, "PING %ld", currentMillis);
        Serial.println(buffer);
      }

      digitalWrite(ledPin, ledState);
    }
  }  
}
