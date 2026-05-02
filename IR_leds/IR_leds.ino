#include <IRremote.h>

// https://www.circuitbasics.com/arduino-ir-remote-receiver-tutorial/

const int RECV_PIN = 7;
IRrecv irrecv(RECV_PIN);
decode_results results;

const int ledCount = 5;
int ledPins[] = { 2, 3, 4, 5, 6 };
int ledState[] = { 0, 0, 0, 0, 0 };
int loopLedIndex = 0;
unsigned long previousMillis = 0;  // will store last time IR code was decoded
unsigned long decodeInterval = 200;
unsigned long loopInterval = 20000; // Start blink loop if no IR code received for 20 seconds

void setup() 
{
  for (int i = 0; i < ledCount; i++)
  {
    pinMode(ledPins[i], OUTPUT);
    Serial.println(ledPins[i]);
  }

  Serial.begin(9600);
  irrecv.enableIRIn();
  irrecv.blink13(true);
  Serial.println("Starting");

  for (int i = 0; i < ledCount; i++)
  {
    digitalWrite(ledPins[i], HIGH);
    delay(100);
  }

  delay(500);

  for (int i = 0; i < ledCount; i++)
  {
    digitalWrite(ledPins[i], LOW);
    delay(100);
  }
}

void turn_on(int ix)
{
    ledState[ix] = 1;
    digitalWrite(ledPins[ix], HIGH);
}

void turn_off(int ix)
{
    ledState[ix] = 0;
    digitalWrite(ledPins[ix], LOW);
}

void toggle(int ix)
{
  if (ledState[ix] == 0)
  {
    ledState[ix] = 1;
    digitalWrite(ledPins[ix], HIGH);
  }
  else
  {
    ledState[ix] = 0;
    digitalWrite(ledPins[ix], LOW);
  }
}

void loop() 
{
  if (irrecv.decode(&results)) 
  {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= decodeInterval)
    {
      previousMillis = currentMillis;
      Serial.println(results.value, HEX);

      if (!results.isRepeat)
      {
        switch(results.value)
        {
            case 0x90AFB06B:
              toggle(0);
              break;
            case 0x18D62BB8:
              toggle(1);
              break;
            case 0x536C0227:
              toggle(2);
              break;
            case 0xBDF0DACD:
              toggle(3);
              break;
            case 0x98EE61F9: // 5
              toggle(4);
              break;
            case 0xFAD78AF6: // 6
              turn_on(0);
              turn_on(1);
              turn_off(2);
              turn_off(3);
              turn_off(4);
              break;
            case 0x8139E8B3: // 7
              turn_off(0);
              turn_off(1);
              turn_off(2);
              turn_on(3);
              turn_on(4);
              break;
            case 0xF2BD765D: // 8
             for (int i = 0; i < ledCount; i++)
              {
                toggle(i);
              }
              break;
            case 0x65C4E72D: // 9
              for (int i = 0; i < ledCount; i++)
              {
                turn_on(i);
              }
              break;
            case 0x9721C416: // 0
              for (int i = 0; i < ledCount; i++)
              {
                turn_off(i);
              }

              break;
        }
      }
    }

    irrecv.resume();
  }
  else
  {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= loopInterval)
    {
      toggle(loopLedIndex);
      delay(100);
      loopLedIndex = (loopLedIndex + 1) % ledCount;
    }
  }  
}
