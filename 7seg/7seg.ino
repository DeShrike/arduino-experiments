
byte b; // 8 bits
uint8_t b2; // 8 bits;
int a;  // 16 bits
long c; // 32 bits;
                Pgfedcba
byte getal0 = 0b00111111;
byte getal1 = 0b00000110;
byte getal3 = 0b01001111;

int[] pins = { 13,12,11,10,9,8,7,6 };

/*
1    1   1   1  1 1 1 1 
128  64  32  16 8 4 2 1

0000 0
0001 1
0010 
0011
0100
0101
0110
0111 7
1000
1001 9
1010 A
1011 B
1100 C
1101 D
1110 E
1111 F
*/
void setup() {
  Serial.begin(9600); // Start Serial.


  // put your setup code here, to run once:
  b2 = 0b11111111; 
  //b2 = 216;
  //b2 = 0xD8;
}

void toongetal(byte getal)
{
  // 0b10101010

  for (int i = 0; i < 8; i += 1)
  {
    digitalWrite(pins[i], LOW);
  }

  if ((getal & 1) == 1)
  {
    digitalWrite(pins[0], HIGH);
  }

  if ((getal & 2) == 2)
  {
    digitalWrite(pins[1], HIGH);
  }

  if ((getal & 4) == 4)
  {
    digitalWrite(pins[2], HIGH);
  }

}

void loop() {
  // put your main code here, to run repeatedly:

  toongetal(getal0);
  delay(1000);
  toongetal(getal1);
  delay(1000);
  toongetal(getal3);
  delay(1000);

  //Serial.println(b2);// Display serial results in serial monitor.
  //delay(1000);

}
