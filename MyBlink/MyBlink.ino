int waittime = 250;
const int minwaittime = 20;
const int maxwaittime = 500;
int delta = 10;

int ledCount = 3
int ledPins[] = { 4, 6, LED_BUILTIN };
int currentLed = 0;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.

  for (int i = 0; i < ledCount; i++)
  {
    pinMode(ledPins[i], OUTPUT);
    Serial.println(ledPins[i]);
  }
  
  Serial.begin(9600);
  Serial.println(LED_BUILTIN);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(ledPins[currentLed], HIGH);
  delay(waittime);
  digitalWrite(ledPins[currentLed], LOW);
  delay(waittime);

  currentLed += 1;
  if (currentLed >= ledCount)
  {
    currentLed = 0;
  }

  if (waittime > maxwaittime || waittime < minwaittime) {
    delta = -delta;
    Serial.println(delta);
  }

  waittime = waittime + delta;
}
