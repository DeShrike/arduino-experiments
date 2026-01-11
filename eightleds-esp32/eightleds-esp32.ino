
#define LED_COUNT 8 
uint8_t leds[LED_COUNT] = {0, 5, 1, 2, 3, 10, 20, 21 };
uint8_t states[LED_COUNT] = {LOW, HIGH, LOW, LOW, HIGH, LOW, HIGH, LOW };

void setup()
{
  Serial.begin(9600);

  for (uint8_t i = 0; i < LED_COUNT; i++)
  {
    pinMode(leds[i], OUTPUT);
  }
}

void loop()
{
  for (uint8_t i = 0; i < LED_COUNT; i++)
  {
    states[i] = states[i] == HIGH ? LOW : HIGH;
    digitalWrite(leds[i], states[i]);  
  }

  delay(250);
}
