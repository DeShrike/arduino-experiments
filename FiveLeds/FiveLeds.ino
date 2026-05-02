int waittime = 90;
const int minwaittime = 10;
const int maxwaittime = 100;
int delta = 5;

unsigned long modeDuration = 20 * 1000;
unsigned long previousModeStart = 0;  // will store last time mode was changed

const int ledCount = 5;
int ledPins[] = { 2, 3, 4, 5, 6 };
int currentLed = 0;
int mode = 0;

void setup()
{
  for (int i = 0; i < ledCount; i++)
  {
    pinMode(ledPins[i], OUTPUT);
    Serial.println(ledPins[i]);
  }
  
  Serial.begin(9600);

  init_mode1();
}

void init_mode1()
{
  Serial.println("Start Mode 1");
  mode = 1;
  waittime = 50;  
  delta = 5;
  previousModeStart = millis();
}

void init_mode2()
{
  Serial.println("Start Mode 2");
  mode = 2;
  waittime = 20;  
  delta = 1;
  previousModeStart = millis();
}

void init_mode3()
{
  Serial.println("Start Mode 3");
  mode = 3;
  waittime = 80;  
  delta = 1;
  previousModeStart = millis();
}

void init_mode4()
{
  Serial.println("Start Mode 4");
  mode = 4;
  waittime = 50;  
  delta = 1;
  previousModeStart = millis();
}

void init_mode5()
{
  Serial.println("Start Mode 5");
  mode = 5;
  waittime = 50;  
  delta = 1;
  previousModeStart = millis();
}

void init_mode6()
{
  Serial.println("Start Mode 6");
  mode = 6;
  waittime = 100;
  previousModeStart = millis();
}

void init_mode7()
{
  Serial.println("Start Mode 7");
  mode = 7;
  waittime = 100;
  delta = 0;
  previousModeStart = millis();
}

void loop()
{
  if (mode == 1)
  {
    mode1();
  }
  else if (mode == 2)
  {
    mode2();
  }
  else if (mode == 3)
  {
    mode3();
  }
  else if (mode == 4)
  {
    mode4();
  }
  else if (mode == 5)
  {
    mode5();
  }
  else if (mode == 6)
  {
    mode6();
  }
  else if (mode == 7)
  {
    mode7();
  }
  else
  {
    mode = 1;
  }

  unsigned long currentMillis = millis();
  if (currentMillis - previousModeStart >= modeDuration)
  {
    if (mode == 1)
    {
      init_mode2();
    }
    else if (mode == 2)
    {
      init_mode3();
    }
    else if (mode == 3)
    {
      init_mode4();
    }
    else if (mode == 4)
    {
      init_mode5();
    }
    else if (mode == 5)
    {
      init_mode6();
    }
    else if (mode == 6)
    {
      init_mode7();
    }
    else
    {
      init_mode1();
    }
  }
}

void mode1()
{
  digitalWrite(ledPins[currentLed], HIGH);
  delay(waittime);
  digitalWrite(ledPins[currentLed], LOW);
  delay(waittime);

  currentLed += 1;
  if (currentLed >= ledCount)
  {
    currentLed = 0;

    waittime = waittime + delta;
    if (waittime > maxwaittime || waittime < minwaittime)
    {
      delta = -delta;
    }
  }
}

void mode2()
{
  digitalWrite(ledPins[currentLed], HIGH);
  delay(waittime);
  digitalWrite(ledPins[currentLed], LOW);
  delay(waittime);
  currentLed += delta;
  if (currentLed < 0 || currentLed >= ledCount)
  {
    delta = -delta; 
    currentLed += delta;
    currentLed += delta;
  }
}

void mode3()
{
  if (delta % 3 == 0)
  {
    digitalWrite(ledPins[0], HIGH);
    digitalWrite(ledPins[4], HIGH);
  }
  else if (delta % 3 == 1)
  {
    digitalWrite(ledPins[1], HIGH);
    digitalWrite(ledPins[3], HIGH);
  }
  else
  {
    digitalWrite(ledPins[2], HIGH);
  }

  delay(waittime);

  for (int i = 0; i < ledCount; i++)
  {    
    digitalWrite(ledPins[i], LOW);
  }

  delay(waittime);
  delta += 1;
}

void mode4()
{
  int i = random() % ledCount;
  digitalWrite(ledPins[i], HIGH);
  delay(waittime);
  digitalWrite(ledPins[i], LOW);
  delay(waittime);
}

void mode5()
{
  for (int i = 0; i < ledCount; i++)
  {    
    digitalWrite(ledPins[i], HIGH);
  }

  delay(waittime / 2);

  for (int i = 0; i < ledCount; i++)
  {    
    digitalWrite(ledPins[i], LOW);
  }

  delay(waittime);
}

void mode6()
{
  digitalWrite(ledPins[0], HIGH);
  digitalWrite(ledPins[1], HIGH);
  digitalWrite(ledPins[3], LOW);
  digitalWrite(ledPins[4], LOW);

  delay(waittime);

  digitalWrite(ledPins[0], LOW);
  digitalWrite(ledPins[1], LOW);
  digitalWrite(ledPins[3], HIGH);
  digitalWrite(ledPins[4], HIGH);

  delay(waittime);
}

void mode7()
{
  digitalWrite(ledPins[0], ((delta >> 0) & 0x00001) == 1 ? HIGH : LOW);
  digitalWrite(ledPins[1], ((delta >> 1) & 0x00001) == 1 ? HIGH : LOW);
  digitalWrite(ledPins[2], ((delta >> 2) & 0x00001) == 1 ? HIGH : LOW);
  digitalWrite(ledPins[3], ((delta >> 3) & 0x00001) == 1 ? HIGH : LOW);
  digitalWrite(ledPins[4], ((delta >> 4) & 0x00001) == 1 ? HIGH : LOW);

  delay(waittime);
  delta += 1;
}
