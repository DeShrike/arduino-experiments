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

void setup()
{
  Serial.begin(9600);

  while (!Serial);

  AiP650E.begin(GPIO5, GPIO4);     // CLK=D5, DIO=D4
  AiP650E.setBrightness(0);     // 0 -> brightest
  Serial.println("\n\nReady\n");
}

void loop()
{
  AiP650E.clearAll();
  delay(1000);

  AiP650E.setSegments(1, 0b00000001); 
  delay(500);

  AiP650E.setSegments(1, 0b00000010); 
  delay(500);

  AiP650E.setSegments(1, 0b00000100); 
  delay(500);
  
  AiP650E.setSegments(1, 0b00001000); 
  delay(500);
  
  AiP650E.setSegments(1, 0b00010000); 
  delay(500);
  
  AiP650E.setSegments(1, 0b00100000); 
  delay(500);
  
  AiP650E.setSegments(1, 0b01000000); 
  delay(500);
  
  AiP650E.setSegments(1, 0b10000000); 
  delay(500);
}
