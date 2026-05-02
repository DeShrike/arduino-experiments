int waittime = 90;
const int minwaittime = 10;
const int maxwaittime = 100;
int delta = 5;

const int ledCount = 5;
int ledPins[] = { 2, 3, 4, 5, 6 };
int currentLed = 0;
int mode = 0;
int bigCount = 0;

void setup()
{
  for (int i = 0; i < ledCount; i++)
  {
    pinMode(ledPins[i], OUTPUT);
    Serial.println(ledPins[i]);
  }
  
  Serial.begin(9600);
  // Serial.println(LED_BUILTIN);

  init_mode1();
}

void init_mode1()
{
  Serial.println("Start Mode 1");
  mode = 1;
  waittime = 50;  
  delta = 5;
}

void init_mode2()
{
  Serial.println("Start Mode 2");
  mode = 2;
  waittime = 20;  
  delta = 1;
}

void init_mode3()
{
  Serial.println("Start Mode 3");
  mode = 3;
  waittime = 80;  
  delta = 1;
}

void init_mode4()
{
  Serial.println("Start Mode 4");
  mode = 4;
  waittime = 50;  
  delta = 1;
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
  else
  {
    mode = 1;
  }

  if (bigCount % 100 == 0)
  {
    bigCount = 1;
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
      //Serial.println(delta);
    }

    bigCount++;
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
    bigCount++;
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

  if (delta % 5 == 0)
  {
    bigCount++;
  }
}

void mode4()
{
  int i = random() % ledCount;
  digitalWrite(ledPins[i], HIGH);
  delay(waittime);
  digitalWrite(ledPins[i], LOW);
  delay(waittime);

  delta += 1;

  if (delta % 5 == 0)
  {
    bigCount++;
  }
}
