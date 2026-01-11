
#define LED_COUNT 4 
//uint8_t leds[LED_COUNT] = {10, 0, 5 };  // 21 no PWM ?   20 no PWM ?
//uint8_t leds[LED_COUNT] = {3, 2, 5 };
uint8_t leds[LED_COUNT] = {0, 1, 5, 10 };
int16_t brightness[LED_COUNT] = { 0, 0, 0, 0 };
int16_t fadeAmount[LED_COUNT] = { 5, 10, 15, 20 };

void setup()
{
  //Serial.begin(9600);

  for (uint8_t i = 0; i < LED_COUNT; i++)
  {
    pinMode(leds[i], OUTPUT);
  }
}

void loop()
{
  for (uint8_t i = 0; i < LED_COUNT; i++)
  {
    analogWrite(leds[i], brightness[i]);

    // change the brightness for next time through the loop:
    brightness[i] = brightness[i] + fadeAmount[i];

    // reverse the direction of the fading at the ends of the fade:
    if (brightness[i] < 0)
    {
      fadeAmount[i] = -fadeAmount[i];
      brightness[i] = 0;
    }
    
    if (brightness[i] > 255)
    {
      fadeAmount[i] = -fadeAmount[i];
      brightness[i] = 255;
    }
  }

  delay(30);
}
