#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT11.h>
#include "esp_random.h"

#define SDA_PIN 8
#define SCL_PIN 9
#define DHTPIN  0

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT11 dht11(DHTPIN);
int counter = 0;
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ 0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000 };

void setup()
{
  inputString.reserve(200);
  Serial.begin(9600);

  Wire.setPins(SDA_PIN, SCL_PIN);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.display();
  delay(1000);

  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  display.clearDisplay();
}

void loop()
{
  int temperature = 0;
  int humidity = 0;
  int result = dht11.readTemperatureHumidity(temperature, humidity);

  display.clearDisplay();
  //display.drawRect(0, 0, display.width()-1, display.height()-1, SSD1306_WHITE);
  //display.fillRect(0, 0, display.width()-1, display.height()-1, SSD1306_WHITE);
  display.drawRect(0, 0, display.width()-1, 24, SSD1306_WHITE);

  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(5, 5);

  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  char temp[30];
  sprintf(temp,"%d ", temperature);
  display.print(temp);
  display.write('°');
  display.write('C');
  
  display.setTextSize(3);
  display.setCursor(2, display.height() - 30);
  counter += 1;

  uint32_t r = esp_random() % 10000000;
  display.print(r);
  Serial.println(r);

  display.display();

  while (Serial.available())
  {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n')
    {
      stringComplete = true;
    }
  }

  if (stringComplete)
  {
    Serial.println(inputString);
    inputString = "";
    stringComplete = false;
  }
  
  delay(1000);
}

void testdrawchar(void) {
  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  for(int16_t i=0; i<256; i++) {
    if(i == '\n') display.write(' ');
    else          display.write(i);
  }

  display.display();
  delay(2000);
}

void XserialEvent()
{
  Serial.println("serialEvent");
  while (Serial.available())
  {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n')
    {
      stringComplete = true;
    }
  }
}
