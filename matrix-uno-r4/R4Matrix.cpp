//#include "Arduino_LED_Matrix.h"
#include "R4Matrix.h"

R4Matrix::R4Matrix()
{
   this->rotate180 = false;
}

void R4Matrix::setRotated180(bool flag)
{
   this->rotate180 = flag;
}

void R4Matrix::begin()
{
   this->matrix.begin();
}

void R4Matrix::drawChar(char c)
{
   this->drawCharInternal(c, 3);
   this->show();
}

void R4Matrix::drawCharChar(char c1, char c2)
{
   this->drawCharInternal(c1, 0);
   this->drawCharInternal(c2, MATRIXWIDTH - FONTWIDTH);
   this->show();
}

void R4Matrix::startScrollString(char* text)
{
   this->currentOffset = MATRIXWIDTH - 1;
   this->currentText = text;
}

bool R4Matrix::scroll()
{
   this->fill(0);
   char* t = this->currentText;
   int16_t o = this->currentOffset;
   bool done = true;
   while (*t != 0)
   {
      if (o <= -FONTWIDTH)
      {
        t++;
        o += FONTWIDTH;
        continue;
      }

      if (o >= MATRIXWIDTH)
      {
        break;
      }

      done = false;
      this->drawCharInternal(*t, o);

      t++;
      o += FONTWIDTH;
   }

   this->show();
   this->currentOffset--;
   return done;
}

void R4Matrix::fill(uint8_t value)
{
   for (uint8_t x = 0; x < MATRIXWIDTH; ++x)
   {
      for (uint8_t y = 0; y < MATRIXHEIGHT; ++y)
      {
         this->frame[y][x] = value;
      }
   }
}

void R4Matrix::plot(uint8_t x, uint8_t y)
{
   if (x < 0 || x >= MATRIXWIDTH || y < 0 || y >= MATRIXHEIGHT)
   {
      return;
   }

   if (this->rotate180)
   {
      x = MATRIXWIDTH - x - 1;
      y = MATRIXHEIGHT - y - 1;
   }

   this->frame[y][x] = 1;
}

void R4Matrix::unplot(uint8_t x, uint8_t y)
{
   if (x < 0 || x >= MATRIXWIDTH || y < 0 || y >= MATRIXHEIGHT)
   {
      return;
   }

   if (this->rotate180)
   {
      x = MATRIXWIDTH - x - 1;
      y = MATRIXHEIGHT - y - 1;
   }

   this->frame[y][x] = 0;
}

void R4Matrix::show()
{
   this->matrix.renderBitmap(this->frame, MATRIXHEIGHT, MATRIXWIDTH);
}

void R4Matrix::drawNumber(int16_t number)
{
   this->fill(0);
   this->drawDigitInternal(number % 10, 8);
   this->drawDigitInternal((number / 10) % 10, 4);
   this->drawDigitInternal((number / 100) % 10, 0);
   this->show();
}

void R4Matrix::drawCharInternal(char c, int16_t offset)
{
   for (uint8_t row = 0; row < FONTHEIGHT; ++row)
   {
      char cc = font[c][row];
      for (uint8_t col = 0; col < FONTWIDTH; ++col)
      {
         if (cc & (1 << (col + (8 - FONTWIDTH))))
         {
            //char temp[100];
            //sprintf(temp, "%d  %d", row, col + offset);
            //Serial.println(temp);
            this->plot(col + offset, row);
         }
      }
   }
}

void R4Matrix::drawDigitInternal(char c, int16_t offset)
{
   for (uint8_t row = 0; row < DIGITFONTHEIGHT; ++row)
   {
      char cc = digitfont[c][row];
      for (uint8_t col = 0; col < DIGITFONTWIDTH; ++col)
      {
         if (cc & (1 << (col + (8 - DIGITFONTWIDTH))))
         {
            //char temp[100];
            //sprintf(temp, "%d  %d", row, col + offset);
            //Serial.println(temp);
            this->plot(col + offset, row + 1);
         }
      }
   }
}

void R4Matrix::rotate180Internal()
{
   for (uint8_t y = 0; y < MATRIXHEIGHT; ++y)
   {
      for (uint8_t x = 0; x < MATRIXWIDTH; ++x)
      {
         char temp = this->frame[y][x];
      }
   }
}
