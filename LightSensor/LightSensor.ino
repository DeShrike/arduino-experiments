#define GREEN 8
#define RED 7

// https://arduinogetstarted.com/tutorials/arduino-light-sensor

int waittime = 50;

void setup() {
  Serial.begin(9600); // Start Serial.

    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);


}

void loop() {

  int A = analogRead(A0);

  Serial.println(A);// Display serial results in serial monitor.

  if (A > 800 ) {   // Change 100 to the number depending on the light in your area.
      digitalWrite(GREEN, HIGH);



    if (A > 1000 ) {   // Change 100 to the number depending on the light in your area.
        digitalWrite(RED, HIGH);
    }
    else
    {
        digitalWrite(RED, LOW);

    }
  }
  else
  {
      digitalWrite(GREEN, LOW);
      digitalWrite(RED, LOW);

  }
  // put your main code here, to run repeatedly:
  // digitalWrite(GREEN, HIGH);
  // delay(waittime);
  // digitalWrite(RED, HIGH);
  // digitalWrite(GREEN, LOW);
  // delay(waittime);
  // digitalWrite(RED, LOW);
  // delay(waittime);

delay(100);
}
