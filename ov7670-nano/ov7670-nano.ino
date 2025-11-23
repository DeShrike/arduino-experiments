// change setup.h to switch between buffered and pixel-by-pixel processing
#include "setup.h"

bool cam_ok = false;

void setup()
{
  delay(2000);
  Serial.begin(9600);
  delay(1000);

  Serial.println(F("Camera Test"));

  // This is not necessary and has no effect for ATMEGA based Arduinos.
  // WAVGAT Nano has slower clock rate by default. We want to reset it to maximum speed
  CLKPR = 0x80; // enter clock rate change mode
  CLKPR = 0; // set prescaler to 0. WAVGAT MCU has it 3 by default.

  Serial.println(F("Init"));
  cam_ok = initializeScreenAndCamera();
}

void loop()
{
  if (cam_ok)
  {
    processFrame();
  }
  else
  {
    Serial.println(F("Camera not ok"));
    delay(1000);
  }

}
