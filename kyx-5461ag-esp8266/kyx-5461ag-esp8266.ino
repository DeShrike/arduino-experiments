/*
  KYX-5461AG / AiP650E0 / 58BT018 
*/

#include "AiP650E.h"

#define GPIO4 4   // SDA
#define GPIO5 5   // SCL

/*
      01
    20  02
      40
    10  04
      08    80
 */

int number = 1234;

uint8_t animation[][4] = { 
  { 0x01, 0x00, 0x00, 0x00 },
  { 0x00, 0x01, 0x00, 0x00 },
  { 0x00, 0x00, 0x01, 0x00 },
  { 0x00, 0x00, 0x00, 0x01 },
  { 0x00, 0x00, 0x00, 0x02 },
  { 0x00, 0x00, 0x00, 0x04 },
  { 0x00, 0x00, 0x00, 0x08 },
  { 0x00, 0x00, 0x08, 0x00 },
  { 0x00, 0x08, 0x00, 0x00 },
  { 0x08, 0x00, 0x00, 0x00 },
  { 0x10, 0x00, 0x00, 0x00 },
  { 0x20, 0x00, 0x00, 0x00 },
};

#define ANIMATION_SIZE sizeof(animation) / sizeof(animation[0])

void setup()
{
  Serial.begin(9600);

  while (!Serial);
  Serial.println("\n\nReady\n");

  AiP650E.begin(GPIO5, GPIO4);     // CLK=D5, DIO=D4
  AiP650E.setBrightness(0);     // 0 -> brightest
  Serial.println(ANIMATION_SIZE);
}

void loop()
{
  displayNumber(number);
  delay(100);
  number++;
  if (number > 9999)
  {
    number = 0;
  }

  if (number % 1000 == 0)
  {
    doAnimation(10);
  }
}

void doAnimation(int count)
{
  for (int c = 0; c < count; c++)
  {
    for (int a = 0; a < ANIMATION_SIZE; a++)
    {
      AiP650E.setSegments(1, animation[a][0]); 
      AiP650E.setSegments(2, animation[a][1]); 
      AiP650E.setSegments(3, animation[a][2]); 
      AiP650E.setSegments(4, animation[a][3]); 
      delay(50);
    }
  }
}

void displayNumber(int num)
{
  int d[4];
  
  d[0] = (num / 1000) % 10;
  d[1] = (num / 100) % 10;
  d[2] = (num / 10) % 10;
  d[3] = num % 10;

  for (int i = 0; i < 4; i++)
  {
    AiP650E.setChar(i + 1, d[i]);
  }
}
