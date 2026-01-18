#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

#include "box.h"
#include "notes.h"

// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

#define SPACING1 20
#define SPACING2 19
uint8_t y = 4;
uint8_t color = 0;

int16_t brightness1 = 0;
int16_t fadeAmount1 = 10;

int16_t brightness2 = 0;
int16_t fadeAmount2 = 20;

int16_t backlight_brightness;

bool buzzer_on = false;

int lastState1 = HIGH; // the previous state from the input pin
int currentState1;     // the current reading from the input pin

int lastState2 = HIGH; // the previous state from the input pin
int currentState2;     // the current reading from the input pin

#define BACKLIGHT_BRIGHTNESS1 127
#define BACKLIGHT_BRIGHTNESS2 64

uint16_t counter;
#define DELAY 50

uint16_t colors[] = {
  ST7735_WHITE, 
  ST7735_YELLOW, 
  ST7735_RED, 
  ST7735_GREEN, 
  ST7735_BLUE, 
  ST7735_MAGENTA, 
  ST7735_CYAN, 
  ST77XX_ORANGE
};

#define COLOR_COUNT (sizeof(colors) / sizeof(colors[0]))

uint8_t draw(uint8_t y)
{
  tft.setCursor(10, y);
  tft.setTextColor(colors[color]);
  tft.setTextSize(2);
  tft.println("CoderDojo");
  y += SPACING1;
  color = (color + 1) % COLOR_COUNT;
  
  tft.setCursor(8, y);
  tft.setTextColor(colors[color]);
  tft.setTextSize(1);
  tft.println("ESP32 C3 Super Mini");
  y += SPACING2;
  color = (color + 1) % COLOR_COUNT;

  return y;
}

void setup()
{
  setup_box();

  SPI.begin(SCK, MISO, MOSI);  
  Serial.begin(9600);

  tft.initR(INITR_BLACKTAB);  // Most 1.8" AZ-Delivery displays
  tft.fillScreen(ST7735_BLACK);
  delay(1000);

  backlight_brightness = BACKLIGHT_BRIGHTNESS1;
  analogWrite(BACKLIGHT_PIN, backlight_brightness);
}

void doScreen()
{
  y = 4;
  y = draw(y);
  y = draw(y);
  y = draw(y);
  y = draw(y);
  color = (color + 1) % COLOR_COUNT;
}

void doLeds()
{
  analogWrite(LED1_PIN, brightness1);
  analogWrite(LED2_PIN, brightness2);

  brightness1 = brightness1 + fadeAmount1;
  brightness2 = brightness2 + fadeAmount2;

  if (brightness1 < 0)
  {
    fadeAmount1 = -fadeAmount1;
    brightness1 = 0;
  }
  
  if (brightness1 > 255)
  {
    fadeAmount1 = -fadeAmount1;
    brightness1 = 255;
  }

  if (brightness2 < 0)
  {
    fadeAmount2 = -fadeAmount2;
    brightness2 = 0;
  }
  
  if (brightness2 > 255)
  {
    fadeAmount2 = -fadeAmount2;
    brightness2 = 255;
  }
}

void playMelody()
{
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 8; thisNote++)
  {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(BUZZER_PIN, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(BUZZER_PIN);
  }
}

void doButtons()
{
  // read the state of button 1:
  currentState1 = digitalRead(BUTTON1_PIN);
  if (lastState1 == LOW && currentState1 == HIGH)
  {
    Serial.println("BUTTON1 state changed from LOW to HIGH");

    backlight_brightness = backlight_brightness == BACKLIGHT_BRIGHTNESS1 ? BACKLIGHT_BRIGHTNESS2 : BACKLIGHT_BRIGHTNESS1;
    analogWrite(BACKLIGHT_PIN, backlight_brightness);

    delay(100);
  }
  lastState1 = currentState1;



  // read the state of button 1:
  currentState2 = digitalRead(BUTTON2_PIN);
  if (lastState2 == LOW && currentState2 == HIGH)
  {
    Serial.println("BUTTON2 state changed from LOW to HIGH");

    playMelody();
    //buzzer_on = !buzzer_on;
    //digitalWrite(BUZZER_PIN, buzzer_on ? HIGH : LOW);

    delay(100);
  }
  lastState2 = currentState2;
}


void loop()
{
  counter++;
  if (counter % (1000 / DELAY) == 0)
  {
    doScreen();
  }

  doButtons();
  doLeds();

  delay(DELAY);
}
