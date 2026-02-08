# BlueBox

## Features

- ESP32-C3 SuperMini
- ST7735 TFT Display - 160 * 128 pixels - 16-bit color
- 2 leds
- 2 push buttons
- Passive buzzer

## Pins

- TFT CS = 7
- TFT DC = 8
- TFT RESET = 9
- TFT Backlight = 5
- SCK = 4
- MOSI = 6
- Button 1 = 20 / Pull-Up / Active Low
- Button 2 = 10 / Pull-Up / Active Low
- Green Led = 1
- Red Led = 0
- Buzzer = 2

## Software

**BlueBox.cpp/BlueBox.h**

The main class with all the graphics routines.

**Button.cpp/Button.h**

The button handlers.

**Menu.cpp/Menu.h**

A simple full screen menu.

**utils.cpp/utils.h**

Several utility functions.

Usage: 

```c++
#include "BlueBox.h"

BlueBox bluebox;

void setup()
{
  bluebox.init();
}

void loop()
{
  auto leftbutton = bluebox.leftButton.update();
  auto rightbutton = bluebox.rightButton.update();

  if (leftbutton == Button::Pressed)
  {
    bluebox.toggleRedLed();
  }

  bluebox.drawTextCentered(50, "Hello ESP32-C3 SuperMini", ST7735_YELLOW, ST7735_BLACK, 2);
  bluebox.flush();
}
```

See `bluebox_demo.ino` for a full demo.
 
Uses buffered output with dirty region tracking for fast updates.

Buttons support *Pressed*, *LongPressed*, *DoublePressed* and *Held* actions.

Led and backlight pins are PWM enabled.

## Graphics routines

- Text drawing, standard font only.
- Rectangles, outlined and filled.
- Cirlces, outlined and filled.
- Rounded rectangles, outlines and filled.
- Lines
- Pixels
- Arcs
- Sprites / 2-bit

## Todo

- Support for color bitmaps
- Support for buzzer

## Dependencies

- Adafruit_GFX
- Adafruit_ST7735

