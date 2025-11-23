#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SDA_PIN 8
#define SCL_PIN 9

#define LED     0

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#define NUMFLAKES     10 // Number of snowflakes in the animation example

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int8_t icons[NUMFLAKES][3];
int ledstate = LOW;
int counter = 0;

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

static const unsigned char PROGMEM logo_bmp[] =
{ 
  0b00000000, 0b11000000,
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
  0b00000000, 0b00110000
};

void setup()
{
  Serial.begin(9600);
  delay(1000);

  pinMode(LED, OUTPUT);

  Wire.setPins(SDA_PIN, SCL_PIN);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(2000);

  init_stars(logo_bmp, LOGO_WIDTH, LOGO_HEIGHT);
  show_info();
}

void show_info()
{
  draw_rect();
  draw_title();
  draw_bitmap((display.width() - LOGO_WIDTH) / 2, display.height() - LOGO_HEIGHT - 4);
  delay(5000);
  erase_info();
}

#define ERASE_SQUARE_SIZE 8

void erase_info()
{
  int x, y, i, c = 0, px = -1, py = -1;
  int xx = display.width() / ERASE_SQUARE_SIZE;
  int yy = display.height() / ERASE_SQUARE_SIZE;

  int epos[xx * yy] = { 0 };
  for (i = 0; i < xx * yy; ++i)
  {
    epos[i] = i;
  }

  for (i = 0; i < 1000; i++)
  {
    x = random(0, xx * yy);
    if (x >= xx * yy)
    {
      Serial.println("Random error");
      break;
    }

    y = random(0, xx * yy);
    if (y >= xx * yy)
    {
      Serial.println("Random error");
      break;
    }

    c = epos[x];
    epos[x] = epos[y];
    epos[y] = c;
  }

  for (i = 0; i < xx * yy; ++i)
  {
    x = epos[i] % xx;
    y = epos[i] / xx;

    if (px != -1)
    {
      display.fillRect(px * ERASE_SQUARE_SIZE, py * ERASE_SQUARE_SIZE, ERASE_SQUARE_SIZE, ERASE_SQUARE_SIZE, SSD1306_BLACK);
    }
    display.fillRect(x * ERASE_SQUARE_SIZE, y * ERASE_SQUARE_SIZE, ERASE_SQUARE_SIZE, ERASE_SQUARE_SIZE, SSD1306_WHITE);
    px = x;
    py = y;
    display.display();
    delay(20);
  }
  
  display.fillRect(px * ERASE_SQUARE_SIZE, py * ERASE_SQUARE_SIZE, ERASE_SQUARE_SIZE, ERASE_SQUARE_SIZE, SSD1306_BLACK);
  display.display();

  delay(1000);
}

void loop()
{
  counter += 1;

  if (counter % 10 == 0)
  {
    ledstate = ledstate == LOW ? HIGH : LOW;
    digitalWrite(LED, ledstate);
  }

  if (counter % 1000 == 0)
  {
    show_info();
  }

  move_stars(logo_bmp, LOGO_WIDTH, LOGO_HEIGHT);
}

void draw_rect(void)
{
  display.clearDisplay();
  display.drawRect(0, 0, display.width(), display.height(), SSD1306_WHITE);

  delay(500);
}

void center_text(int16_t y, const char* text)
{
  int16_t  x1, y1;
  uint16_t w, h;

  display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);

  display.setCursor((display.width() - w) / 2, y);
  display.print(text);
  }

void draw_title(void)
{
  const char* text1 = "Coderdojo";
  const char* text2 = "Belgium";

  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.cp437(true);

  display.setCursor(0, 0);


  center_text(3, text1);
  center_text(20, text2);
  
  display.display();
  delay(500);
}

void draw_bitmap(int x, int y)
{
  display.drawBitmap(x, y, logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();

  delay(500);
}

#define XPOS   0 // Indexes into the 'icons' array in function below
#define YPOS   1
#define DELTAY 2


void init_stars(const uint8_t *bitmap, uint8_t w, uint8_t h)
{
  int f;
  for (f = 0; f < NUMFLAKES; f++)
  {
    icons[f][XPOS]   = random(LOGO_WIDTH, display.width() - LOGO_WIDTH);
    icons[f][YPOS]   = -LOGO_HEIGHT;
    icons[f][DELTAY] = random(1, 6);
  }
}

void move_stars(const uint8_t *bitmap, uint8_t w, uint8_t h)
{
  int f;
  display.clearDisplay(); // Clear the display buffer

  // Draw each snowflake:
  for (f = 0; f < NUMFLAKES; f++)
  {
    display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, SSD1306_WHITE);
  }

  display.display(); // Show the display buffer on the screen

  // Then update coordinates of each flake...
  for (f = 0; f < NUMFLAKES; f++)
  {
    icons[f][YPOS] += icons[f][DELTAY];
    // If snowflake is off the bottom of the screen...
    if (icons[f][YPOS] >= display.height())
    {
      // Reinitialize to a random position, just off the top
      icons[f][XPOS]   = random(1 - LOGO_WIDTH, display.width());
      icons[f][YPOS]   = -LOGO_HEIGHT;
      icons[f][DELTAY] = random(1, 6);
    }
  }
}
