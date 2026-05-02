unsigned long modeDuration = 20 * 1000;
unsigned long previousModeStart = 0;  // will store last time mode was changed
int mode = 0;
int direction;
int waittime;

int DATA = 2; // Yellow (SER)
int CLCK = 8; // Blue (SHCP)  (SRCLK)
int LATCH = 3; // Orange (STCP) (RCLK)

short data = 0b00000000;
short data2 = 0b00000000;

void setup() 
{
  pinMode(DATA, OUTPUT);
  pinMode(CLCK, OUTPUT);
  pinMode(LATCH, OUTPUT);

  Serial.begin(9600);

  init_mode4();
}

void init_mode1()
{
  data = 0;
  mode = 1;
  waittime = 30;
  previousModeStart = millis();

  Serial.println("Mode 1");
}

void init_mode2()
{
  // KITT
  data = 1;
  direction = 1;
  mode = 2;
  waittime = 50;  
  previousModeStart = millis();

  Serial.println("Mode 2");
}

void init_mode3()
{
  // double KITT
  data = 1;
  data2 = 0b10000000;
  direction = 1;
  mode = 3;
  waittime = 50;  
  previousModeStart = millis();

  Serial.println("Mode 3");
}

void init_mode4()
{
  // double KITT
  data = 0b00000011;
  direction = 1;
  mode = 4;
  waittime = 50;
  previousModeStart = millis();
  
  Serial.println("Mode 4");
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
      init_mode1();
    }
    else
    {
      init_mode1();
    }
  }
}

void mode1() 
{
  // Count 0 - 255
  digitalWrite(LATCH, LOW);                 // Hold latchPin LOW for transmitting data
  shiftOut(DATA, CLCK, LSBFIRST, data);     // Transmit data
  digitalWrite(LATCH, HIGH);                //  Return the latch pin high to signal chip

  //Wait
  delay(waittime);

  data += 1;
}

void mode2()
{
  digitalWrite(LATCH, LOW);                 // Hold latchPin LOW for transmitting data
  shiftOut(DATA, CLCK, LSBFIRST, data);     // Transmit data
  digitalWrite(LATCH, HIGH);                //  Return the latch pin high to signal chip

  //Wait
  delay(waittime);

  if (direction == 1)
  {
    if (data == 0b10000000)
    {
      direction = -1;
    }
    else
    {
      data = data << 1;
    }
  }
  else
  {
    if (data == 0b00000001)
    {
      direction = 1;
    }
    else
    {
      data = data >> 1;
    }
  }
}

void mode3()
{
  digitalWrite(LATCH, LOW);                 // Hold latchPin LOW for transmitting data
  shiftOut(DATA, CLCK, LSBFIRST, data | data2);     // Transmit data
  digitalWrite(LATCH, HIGH);                //  Return the latch pin high to signal chip

  //Wait
  delay(waittime);

  if (direction == 1)
  {
    if (data == 0b10000000)
    {
      direction = -1;
    }
    else
    {
      data = data << 1;
      data2 = data2 >> 1;
    }
  }
  else
  {
    if (data == 0b00000001)
    {
      direction = 1;
    }
    else
    {
      data = data >> 1;
      data2 = data2 << 1;
    }
  }
}

void mode4()
{
  digitalWrite(LATCH, LOW);                 // Hold latchPin LOW for transmitting data
  shiftOut(DATA, CLCK, MSBFIRST, data);     // Transmit data
  digitalWrite(LATCH, HIGH);                //  Return the latch pin high to signal chip

  //Wait
  delay(waittime);

  data = data << 1;
  data = data & 0b11111111;
  if ((data == 0b10000000) || (data == 0b00000010) )
  {
     data = data | 0b00000001;
  }
}
