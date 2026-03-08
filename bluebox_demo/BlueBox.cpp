#include <SPI.h>
#include <math.h>
#include "BlueBox.h"
#include "Utils.h"

const uint16_t bluebox_colors[] = {
   BLUEBOX_LIME, BLUEBOX_GREEN, BLUEBOX_EMERALD, BLUEBOX_TEAL, BLUEBOX_CYAN, BLUEBOX_COBALT, BLUEBOX_INDIGO, BLUEBOX_VIOLET, BLUEBOX_PINK, BLUEBOX_MAGENTA, BLUEBOX_CRIMSON, BLUEBOX_RED, BLUEBOX_ORANGE, BLUEBOX_AMBER, BLUEBOX_YELLOW, BLUEBOX_BROWN, BLUEBOX_OLIVE, BLUEBOX_STEEL, BLUEBOX_MAUVE, BLUEBOX_TAUPE, 
};
const char* bluebox_color_names[] = {
   "Lime", "Green", "Emerald", "Teal", "Cyan", "Cobalt", "Indigo", "Violet", "Pink", "Magenta", "Crimson", "Red", "Orange", "Amber", "Yellow", "Brown", "Olive", "Steel", "Mauve", "Taupe", 
};

const uint8_t BLUEBOX_COLOR_COUNT = (sizeof(bluebox_colors) / sizeof(bluebox_colors[0]));

BlueBox::BlueBox() : tft(TFT_CS, TFT_DC, TFT_RST)
{}

void BlueBox::markDirty(int x1, int y1, int x2, int y2)
{
  if (x1 < dirtyX1) dirtyX1 = max(x1, 0);
  if (y1 < dirtyY1) dirtyY1 = max(y1, 0);
  if (x2 > dirtyX2) dirtyX2 = min(x2, WIDTH - 1);
  if (y2 > dirtyY2) dirtyY2 = min(y2, HEIGHT - 1);
}

void BlueBox::init()
{
  if (initialized) return;
  SPI.begin(SCK, MISO, MOSI);

  // tft.setSPISpeed(20000000); // 20 MHz
  // tft.setSPISpeed(40000000); // 40 MHz (try 27 MHz if unstable)

  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);
  fill(ST7735_BLACK);
  flush();

  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(BACKLIGHT_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  backLight(255);

  beginInput();

  initialized = true;
}

void BlueBox::flush()
{
  if (!initialized) return;
  if (dirtyX2 < dirtyX1 || dirtyY2 < dirtyY1) return;

  int x0 = dirtyX1, y0 = dirtyY1;
  int x1 = dirtyX2, y1 = dirtyY2;
  int w = x1 - x0 + 1;
  int h = y1 - y0 + 1;

  tft.startWrite();
  tft.setAddrWindow(x0, y0, w, h);
  for (int y = y0; y <= y1; y++)
  {
    tft.writePixels(&buffer[y * WIDTH + x0], w);
  }

  tft.endWrite();

  dirtyX1 = WIDTH; dirtyY1 = HEIGHT;
  dirtyX2 = 0; dirtyY2 = 0;
}

void BlueBox::drawPixel(int x, int y, uint16_t color)
{
  if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return;
  buffer[y * WIDTH + x] = color;
  markDirty(x, y, x, y);
}

void BlueBox::drawPixelUnchecked(int x, int y, uint16_t color)
{
  buffer[y * WIDTH + x] = color;
  markDirty(x, y, x, y);
}

void BlueBox::fillRect(int x, int y, int w, int h, uint16_t color)
{
  if (x < 0)
  {
    w += x;
    x = 0;
  }

  if (y < 0)
  {
    h += y;
    y = 0;
  }

  if (x + w > WIDTH) w = WIDTH - x;
  if (y + h > HEIGHT) h = HEIGHT - y;
  if (w <= 0 || h <= 0) return;

  for (int row = y; row < y + h; row++)
  {
    for (int col = x; col < x + w; col++)
    {
      buffer[row * WIDTH + col] = color;
    }
  }

  markDirty(x, y, x + w - 1, y + h - 1);
}

void BlueBox::clearRect(int x, int y, int w, int h)
{
  fillRect(x, y, w, h, ST7735_BLACK);
}

void BlueBox::fill(uint16_t color)
{
  for (int i = 0; i < WIDTH * HEIGHT; i++)
  {
    buffer[i] = color;
  }

  markDirty(0, 0, WIDTH - 1, HEIGHT - 1);
}

void BlueBox::measureText(const char* text, uint8_t fontSize, int* width, int* height)
{
    *height = 8 * fontSize;
    *width = 0;

    while (*text)
    {
        *width += 6 * fontSize;
        text++;
    }
}

void BlueBox::drawTextCentered(int y, const char* text, uint16_t fgcolor, uint16_t bgcolor, uint8_t fontSize)
{
  int width, height;
  measureText(text, fontSize, &width, &height);
  drawText((WIDTH - width) / 2, y, text, fgcolor, bgcolor, fontSize);
}

void BlueBox::drawText(int x, int y, const char* text, uint16_t fgcolor, uint16_t bgcolor, uint8_t fontSize)
{
    //const uint8_t *font = font; // Default font is 5x7
    uint16_t cursor_x = x;
    uint16_t cursor_y = y;

    while (*text)
    {
        char c = *text++;
        if (c < 32 || c > 126) c = '?'; // Replace unsupported chars

        for (uint8_t i = 0; i < 5; i++)
        { // 5 columns per char
            uint8_t line = pgm_read_byte(&font[c * 5 + i]);
            for (uint8_t j = 0; j < 8; j++)
            { // 8 rows per char
                uint16_t color = (line & 0x1) ? fgcolor : bgcolor;
                for (uint8_t fx = 0; fx < fontSize; fx++)
                {
                    for (uint8_t fy = 0; fy < fontSize; fy++)
                    {
                        int px = cursor_x + i * fontSize + fx;
                        int py = cursor_y + j * fontSize + fy;
                        if (px >= 0 && px < WIDTH && py >= 0 && py < HEIGHT)
                        {
                            buffer[py * WIDTH + px] = color;
                        }
                    }
                }

                line >>= 1;
            }
        }

        // Space between characters
        for (uint8_t fx = 0; fx < fontSize; fx++)
        {
            for (uint8_t fy = 0; fy < 8 * fontSize; fy++)
            {
                int px = cursor_x + 5 * fontSize + fx;
                int py = cursor_y + fy;
                if (px >= 0 && px < WIDTH && py >= 0 && py < HEIGHT)
                {
                    buffer[py * WIDTH + px] = bgcolor;
                }
            }
        }

        cursor_x += (6 * fontSize); // 5 pixels + 1 space
    }

    markDirty(x, y, cursor_x, y + fontSize * 8);
}

void BlueBox::drawTextMulti(int x, int y, const char* text, uint16_t fgcolor, uint16_t bgcolor, uint8_t fontSize)
{
    int startX = x;
    while (*text)
    {
        if (*text == '\n')
        {
            x = startX;
            y += 8 * fontSize;
        }
        else
        {
            char buf[2] = {*text, 0};
            drawText(x, y, buf, fgcolor, bgcolor, fontSize);
            x += 6 * fontSize;
        }

        text++;
    }
}

// DRAW SPRITE (1-bit)
void BlueBox::drawSprite(int x, int y, const uint8_t* sprite, int w, int h, uint16_t color)
{
    for (int sy = 0; sy < h; sy++)
    {
        for (int sx = 0; sx < w; sx++)
        {
            int byteIndex = (sy * ((w + 7) / 8)) + (sx / 8);
            bool pixelOn = sprite[byteIndex] & (0x80 >> (sx % 8));
            if (pixelOn) drawPixel(x + sx, y + sy, color);
        }
    }

    markDirty(x, y, x + w - 1, y + h - 1);
}

void BlueBox::drawRGBBitmap(int16_t x, int16_t y,
                            const uint16_t bitmap[],
                            int16_t w, int16_t h)
{
	for (int16_t yy = 0; yy < h; yy++)
    {
	 	const uint16_t* src = &bitmap[yy * w];
		uint16_t* dest = &buffer[(y + yy) * WIDTH + x];
		memcpy(dest, src, w * sizeof(uint16_t));
    }

    markDirty(x, y, x + w - 1, y + h - 1);
}

void BlueBox::scrollHorizontally(int dx)
{
    if (dx == 0) return;

    if (dx > 0)
    {
        for (int y = 0; y < HEIGHT; y++)
        {
            for (int x = WIDTH - 1; x >= dx; x--)
            {
                buffer[y * WIDTH + x] = buffer[y * WIDTH + x - dx];
            }

            for (int x = 0; x < dx; x++)
            {
                buffer[y * WIDTH + x] = ST7735_BLACK;
            }
        }
    }
    else
    {
        dx = -dx;
        for (int y = 0; y < HEIGHT; y++)
        {
            for (int x = 0; x < WIDTH - dx; x++)
            {
                buffer[y * WIDTH + x] = buffer[y * WIDTH + x + dx];
            }

            for (int x = WIDTH - dx; x < WIDTH; x++)
            {
                buffer[y * WIDTH + x] = ST7735_BLACK;
            }
        }
    }

    markDirty(0, 0, WIDTH - 1, HEIGHT - 1);
}

void BlueBox::drawRoundedCorner(uint16_t x0, uint16_t y0, uint16_t r, uint8_t corner, uint16_t color)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    auto setPixel = [&](int16_t x, int16_t y)
    {
        if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
            buffer[y * WIDTH + x] = color;
    };

    while (x < y)
    {
        if (f >= 0)
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }

        x++;
        ddF_x += 2;
        f += ddF_x;

        if (corner & 0x4)
        {
            setPixel(x0 + x, y0 + y);
            setPixel(x0 + y, y0 + x);
        }
        else if (corner & 0x2)
        {
            setPixel(x0 + x, y0 - y);
            setPixel(x0 + y, y0 - x);
        }
        else if (corner & 0x8)
        {
            setPixel(x0 - y, y0 + x);
            setPixel(x0 - x, y0 + y);
        }
        else if (corner & 0x1)
        {
            setPixel(x0 - y, y0 - x);
            setPixel(x0 - x, y0 - y);
        }
    }
}

void BlueBox::fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color)
{
    int16_t f  = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    while (x < y)
    {
        if (f >= 0)
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }

        x++;
        ddF_x += 2;
        f += ddF_x;

        if (cornername & 0x1)
        {
            drawVLine(x0 + x, y0 - y, y0 - y + 2 * y + 1 + delta, color);
            drawVLine(x0 + y, y0 - x, y0 - x + 2 * x + 1 + delta, color);
        }

        if (cornername & 0x2)
        {
            drawVLine(x0 - x, y0 - y, y0 - y + 2 * y + 1 + delta, color);
            drawVLine(x0 - y, y0 - x, y0 - x + 2 * x + 1 + delta, color);
        }
    }
}

void BlueBox::drawRoundedRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint16_t color, bool filled)
{
    if (filled)
    {
      // smarter version
      fillRect(x + r, y, w - 2 * r, h, color);

      // draw four corners
      fillCircleHelper(x + w - r - 1, y + r, r, 1, h - 2 * r - 1, color);
      fillCircleHelper(x + r        , y + r, r, 2, h - 2 * r - 1, color);
    }
    else
    {
      uint16_t x2 = (w - 2 * r) + (x + r);
      drawHLine(x + r, x2, y, color); // Top
      drawHLine(x + r, x2, y + h - 1, color); // Bottom

      uint16_t y2 = (h - 2 * r) + (y + r);
      drawVLine(x, y + r, y2, color); // Left
      drawVLine(x + w - 1, y + r, y2, color); // Right

      // draw four corners
      drawRoundedCorner(x + r, y + r, r, 1, color);
      drawRoundedCorner(x + w - r - 1, y + r, r, 2, color);
      drawRoundedCorner(x + w - r - 1, y + h - r - 1, r, 4, color);
      drawRoundedCorner(x + r, y + h - r - 1, r, 8, color);
    }

    markDirty(x, y, x + w - 1, y + h - 1);
}

void BlueBox::drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color, bool filled)
{
    int16_t x = 0;
    int16_t y = r;
    int16_t d = 1 - r;

    auto setPixel = [&](int16_t x, int16_t y)
    {
        if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
            buffer[y * WIDTH + x] = color;
    };

    while (y >= x)
    {
        if (filled)
        {
            // Draw horizontal lines between points
            for (int16_t i = x0 - x; i <= x0 + x; i++)
            {
                setPixel(i, y0 + y);
                setPixel(i, y0 - y);
            }

            for (int16_t i = x0 - y; i <= x0 + y; i++)
            {
                setPixel(i, y0 + x);
                setPixel(i, y0 - x);
            }
        }
        else
        {
            setPixel(x0 + x, y0 + y);
            setPixel(x0 - x, y0 + y);
            setPixel(x0 + x, y0 - y);
            setPixel(x0 - x, y0 - y);
            setPixel(x0 + y, y0 + x);
            setPixel(x0 - y, y0 + x);
            setPixel(x0 + y, y0 - x);
            setPixel(x0 - y, y0 - x);
        }

        x++;
        if (d < 0)
        {
            d += 2 * x + 1;
        }
        else
        {
            y--;
            d += 2 * (x - y) + 1;
        }
    }

    markDirty(x0 - r, y0 - r, x0 + r, y0 + r);
}

void BlueBox::drawHLine(uint16_t x1, uint16_t x2, uint16_t y, uint16_t color)
{
    auto setPixel = [&](int16_t x, int16_t y)
    {
        if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
            buffer[y * WIDTH + x] = color;
    };

    if (x2 < x1)
    {
        return;
    }

    if (y < 0 || y >= HEIGHT)
    {
        return;
    }

    for (uint16_t x = x1; x <= x2; ++x)
    {
        if (x < 0 || x >= WIDTH)
        {
            return;
        }

        setPixel(x, y);
    }

    markDirty(x1, y, x2, y);
}

void BlueBox::drawVLine(uint16_t x, uint16_t y1, uint16_t y2, uint16_t color)
{
    auto setPixel = [&](int16_t x, int16_t y)
    {
        if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
            buffer[y * WIDTH + x] = color;
    };

    if (y2 < y1)
    {
        return;
    }

    if (x < 0 || x >= WIDTH)
    {
        return;
    }

    for (uint16_t y = y1; y <= y2; ++y)
    {
        if (y < 0 || y >= HEIGHT)
        {
            return;
        }

        setPixel(x, y);
    }

    markDirty(x, y1, x, y2);
}

void BlueBox::drawLineLow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
    int16_t dx = x1 - x0;
    int16_t dy = y1 - y0;
    int16_t yi = 1;
    if (dy < 0)
    {
        yi = -1;
        dy = -dy;
    }

    int16_t D = (2 * dy) - dx;
    uint16_t y = y0;

    for (uint16_t x = x0; x <= x1; ++x)
    {
        buffer[y * WIDTH + x] = color;

        if (D > 0)
        {
            y = y + yi;
            D = D + (2 * (dy - dx));
        }
        else
        {
            D = D + 2 * dy;
        }
    }
}

void BlueBox::drawLineHigh(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
    int16_t dx = x1 - x0;
    int16_t dy = y1 - y0;
    int16_t xi = 1;
    if (dx < 0)
    {
        xi = -1;
        dx = -dx;
    }

    int16_t D = (2 * dx) - dy;
    uint16_t x = x0;

    for (uint16_t y = y0; y <= y1; ++y)
    {
        buffer[y * WIDTH + x] = color;

        if (D > 0)
        {
            x = x + xi;
            D = D + (2 * (dx - dy));
        }
        else
        {
            D = D + 2 * dx;
        }
    }
}

void BlueBox::drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
    if (abs(y1 - y0) < abs(x1 - x0))
    {
        if (x0 > x1)
        {
            drawLineLow(x1, y1, x0, y0, color);
        }
        else
        {
            drawLineLow(x0, y0, x1, y1, color);
        }
    }
    else
    {
        if (y0 > y1)
        {
            drawLineHigh(x1, y1, x0, y0, color);
        }
        else
        {
            drawLineHigh(x0, y0, x1, y1, color);
        }
    }

    markDirty(min(x0, x1), min(y0, y1), max(x0, x1), max(y0, y1));
}

void BlueBox::drawRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    drawHLine(x1, x2, y1, color);
    drawHLine(x1, x2, y2, color);
    drawVLine(x1, y1, y2, color);
    drawVLine(x2, y1, y2, color);
}

void BlueBox::drawThickCircle(uint16_t x, uint16_t y, uint16_t r, uint16_t lineThickness, uint16_t color)
{
    // https://github.com/lvgl/lvgl/issues/252
    r += (lineThickness / 2);
    uint16_t r2 = r - lineThickness;

    uint16_t xo[256];
    uint16_t xi[256];

    int32_t   D;    // Decision Variable
    uint32_t  CurX; // Current X Value
    uint32_t  CurY; // Current Y Value

    D = 3 - (r << 1);
    CurX = 0;
    CurY = r;

    int iterations = 0;

    // Calculate outer circle
    while (CurX <= CurY)
    {
        xo[CurX] = CurY;
        xo[CurY] = CurX;
        if (iterations < CurX)
        {
            iterations = CurX;
        }

        if (iterations < CurY)
        {
            iterations = CurY;
        }

        xi[CurX * 2] = 0;
        xi[CurY * 2] = 0;
        xi[CurX * 2 + 1] = 0;
        xi[CurY * 2 + 1] = 0;

        if (D < 0)
        {
            D += (CurX << 2) + 6;
        }
        else
        {
            D += ((CurX - CurY) << 2) + 10;
            CurY--;
        }

        CurX++;
    }

    D = 3 - (r2 << 1);
    CurX = 0;
    CurY = r2;

    // Calculate inner circle
    while (CurX <= CurY)
    {
        xi[CurX] = CurY;
        xi[CurY] = CurX;

        if (D < 0)
        {
            D += (CurX << 2) + 6;
        }
        else
        {
            D += ((CurX - CurY) << 2) + 10;
            CurY--;
        }

        CurX++;
    }

    // Draw horizontal lines
    for (int yy = 0; yy <= iterations; yy++)
    {
        if (xi[yy] != 0)
        {
            drawHLine(x + xi[yy], x + xo[yy], y - yy, color);
            drawHLine(x - xo[yy], x - xi[yy], y - yy, color);
            drawHLine(x + xi[yy], x + xo[yy], y + yy, color);
            drawHLine(x - xo[yy], x - xi[yy], y + yy, color);
        }
        else
        {
            drawHLine(x - xo[yy], x + xo[yy], y - yy, color);
            drawHLine(x - xo[yy], x + xo[yy], y + yy, color);
        }
    }
}

void BlueBox::drawThickArc(uint16_t x, uint16_t y, uint16_t r, uint16_t lineThickness, uint16_t startAngle, uint16_t endAngle, uint16_t color)
{
    startAngle = (startAngle + 90) % 360;
    endAngle = (endAngle + 90) % 360;

    if (startAngle < endAngle)
    {
        drawThickArcBase(x, y, r, lineThickness, startAngle, endAngle, color);
    }
    else
    {
        drawThickArcBase(x, y, r, lineThickness, startAngle, 360, color);
        drawThickArcBase(x, y, r, lineThickness, 0, endAngle, color);
    }
}

void BlueBox::drawThickArcBase(uint16_t x, uint16_t y, uint16_t r, uint16_t lineThickness, uint16_t startAngle, uint16_t endAngle, uint16_t color)
{
    auto setPixel = [&](int16_t x, int16_t y)
    {
        if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
            buffer[y * WIDTH + x] = color;
    };

    r += (lineThickness / 2);
    uint16_t r2 = r - lineThickness;
    int16_t deg;

    deg = fast_atan2(-r, 0);
    if ((deg >= startAngle) && (deg <= endAngle))
    {
        // Left Middle
        drawHLine(x - r + 1, x - r + 1 + lineThickness, y, color);
    }

    deg = fast_atan2(r2, 0);
    if ((deg >= startAngle) && (deg <= endAngle))
    {
        // Right Middle
        drawHLine(x + r2, x + r2 + lineThickness, y, color);
    }

    deg = fast_atan2(0, -r);
    if ((deg >= startAngle) && (deg <= endAngle))
    {
        // Top Middle
        drawVLine(x, y - r + 1, y - r + 1 + lineThickness, color);
    }

    deg = fast_atan2(0, r2);
    if ((deg >= startAngle) && (deg <= endAngle))
    {
        // Bottom middle
        drawVLine(x, y + r2, y + r2 + lineThickness, color);
    }

    int16_t minx = 1000, miny = 1000;
    int16_t maxx = 0, maxy = 0;

    uint32_t RR = r * r;
    uint32_t R2R2 = r2 * r2;
    for(int16_t yy = -r; yy < 0; yy++)
    {
        for (int16_t xx = -r; xx < 0; xx++)
        {
            uint32_t rr2 = xx * xx + yy * yy;
            if ((rr2 <= RR) && (rr2 >= R2R2))
            {
                deg = fast_atan2(xx, yy);
                if ((deg >= startAngle) && (deg <= endAngle))
                {
                    setPixel(x + xx, y + yy);
                    minx = MIN(minx, x + xx);
                    miny = MIN(miny, y + yy);
                    maxx = MAX(maxx, x + xx);
                    maxy = MAX(maxy, y + yy);
                }

                deg = fast_atan2(xx, -yy);
                if ((deg >= startAngle) && (deg <= endAngle))
                {
                    setPixel(x + xx, y - yy);
                    minx = MIN(minx, x + xx);
                    miny = MIN(miny, y - yy);
                    maxx = MAX(maxx, x + xx);
                    maxy = MAX(maxy, y - yy);
                }

                deg = fast_atan2(-xx, yy);
                if ((deg >= startAngle) && (deg <= endAngle))
                {
                    setPixel(x - xx, y + yy);
                    minx = MIN(minx, x - xx);
                    miny = MIN(miny, y + yy);
                    maxx = MAX(maxx, x - xx);
                    maxy = MAX(maxy, y + yy);
                }

                deg = fast_atan2(-xx, -yy);
                if ((deg >= startAngle) && (deg <= endAngle))
                {
                    setPixel(x - xx, y - yy);
                    minx = MIN(minx, x - xx);
                    miny = MIN(miny, y - yy);
                    maxx = MAX(maxx, x - xx);
                    maxy = MAX(maxy, y - yy);
                }
            }
        }
    }

    markDirty(minx, miny, maxx, maxy);
}

// BUTTONS
void BlueBox::beginInput()
{
    leftButton.begin();
    rightButton.begin();
}

void BlueBox::processInput()
{
    leftButton.update();
    rightButton.update();
}

// LEDS
void BlueBox::greenLed(bool flag)
{
    digitalWrite(LED1_PIN, flag ? HIGH : LOW);
    greenLedState = flag;
}

void BlueBox::redLed(bool flag)
{
    digitalWrite(LED2_PIN, flag ? HIGH : LOW);
    redLedState = flag;
}

void BlueBox::toggleRedLed()
{
    redLedState = !redLedState;
    digitalWrite(LED2_PIN, redLedState ? HIGH : LOW);
}

void BlueBox::toggleGreenLed()
{
    greenLedState = !greenLedState;
    digitalWrite(LED1_PIN, greenLedState ? HIGH : LOW);
}

// BACKLIGHT
void BlueBox::backLight(uint8_t brightness)
{
    if (brightness == 0)
    {
        digitalWrite(BACKLIGHT_PIN, LOW);
    }
    else if (brightness == 255)
    {
        digitalWrite(BACKLIGHT_PIN, HIGH);
    }
    else
    {
        analogWrite(BACKLIGHT_PIN, brightness);
    }
}
