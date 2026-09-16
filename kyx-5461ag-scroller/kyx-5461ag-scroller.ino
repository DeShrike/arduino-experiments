/*
  KYX-5461AG / AiP650E0 / 58BT018 
*/

#include "AiP650E.h"

#define GPIO4 4   // SDA
#define GPIO5 5   // SCL

const char* sentence = "Coderdojo Belgium";
int sentence_length;
int offset;

/*
      01
    20  02
      40
    10  04
      08    80
 */

void setup()
{
  Serial.begin(9600);

  while (!Serial);

  AiP650E.begin(GPIO5, GPIO4);     // CLK=D5, DIO=D4
  AiP650E.setBrightness(0);     // 0 -> brightest
  Serial.println("\n\nReady\n");

  AiP650E.clearAll();
  delay(1000);

  sentence_length = strlen(sentence);
  offset = -3;
}

void show1(int pos, char c)
{
    if (c == ' ') 
    {
      AiP650E.clear(pos);
    }
    else
    {
      AiP650E.setChar(pos, c);
    }
}

void show()
{
  const char* p = sentence + offset;

  for (int i = 0; i < 4; i++)
  {
    if ((offset + i) < sentence_length && (offset + i) >= 0)
    {
      show1(1 + i, *(p + i));
    }
    else
    {
      AiP650E.clear(1 + i);
    }
  }
}

void loop()
{
  show();
  delay(300);
  offset++;

  if (offset > sentence_length + 2)
  {
    offset = -3;
  }
}
