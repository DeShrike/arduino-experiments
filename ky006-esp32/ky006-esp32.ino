///////////////////////////////////////
// KY0-006 Passive buzzer YT Tutorial//
///////////////////////////////////////
int buzzer = 0;  // Declaration of the buzzer output pin
void setup() {
  pinMode(buzzer, OUTPUT);  // Initialize as output pin
}
void loop() {
  unsigned char i;
  while (1) {
    // The signal consists of a square wave voltage.
    // Turning the buzzer on and off will generate a tone that roughly corresponds to the frequency.
    // The frequency is defined by the length of the on and off phase.

    //Tone 1
    for (i = 0; i < 80; i++) {
      digitalWrite(buzzer, HIGH); 
      delay(1);
      digitalWrite(buzzer, LOW);
      delay(1);
    }
    //Tone 2
    for (i = 0; i < 100; i++) {
      digitalWrite(buzzer, HIGH);
      delay(2);
      digitalWrite(buzzer, LOW);
      delay(2);
    }
    //Tone 3
    for (i = 0; i < 100; i++) {
      digitalWrite(buzzer, HIGH);
      delay(3);
      digitalWrite(buzzer, LOW);
      delay(2);
    }
    //Tone 4
    for (i = 0; i < 100; i++) {
      digitalWrite(buzzer, HIGH);
      delay(2);
      digitalWrite(buzzer, LOW);
      delay(4);
    }
  }
}
