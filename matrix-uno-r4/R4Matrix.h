#ifndef _R4MATRIX_H_
#define _R4MATRIX_H_

#include "Arduino_LED_Matrix.h"

#include "6x8_horizontal_LSB_1.h"
#define FONTWIDTH 6
#define FONTHEIGHT 8

#include "digitfont.h"

#define MATRIXWIDTH 12
#define MATRIXHEIGHT 8

class R4Matrix
{
   private:
      uint8_t frame[MATRIXHEIGHT][MATRIXWIDTH] = {
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
      };

      ArduinoLEDMatrix matrix;
      int16_t currentOffset;
      char* currentText = NULL;
      bool rotate180;
      void drawCharInternal(char c, int16_t offset);
      void drawDigitInternal(char c, int16_t offset);
      void rotate180Internal();

   public:
      R4Matrix();

      void begin();

      void setRotated180(bool flag);

      void drawChar(char c);
      void drawCharChar(char c1, char c2);
      void drawNumber(int16_t number);

      void startScrollString(char* text);
      bool scroll();

      void fill(uint8_t value);
      void clear();

      void plot(uint8_t x, uint8_t y);
      void unplot(uint8_t x, uint8_t y);
      void show();
};

#endif
