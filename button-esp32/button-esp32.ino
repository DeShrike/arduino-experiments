uint8_t ledstate = LOW;

#define LED_PIN  5
#define BUTTON_PIN  0

// Variables will change:
int lastState = HIGH; // the previous state from the input pin
int currentState;     // the current reading from the input pin


void setup()
{
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop()
{
// read the state of the switch/button:
  currentState = digitalRead(BUTTON_PIN);
  if(lastState == LOW && currentState == HIGH)
  {
    Serial.println("The state changed from LOW to HIGH");
    ledstate = ledstate == HIGH ? LOW : HIGH;
    // save the last state
    digitalWrite(LED_PIN, ledstate);
    delay(100);
  }
  lastState = currentState;
  
}
