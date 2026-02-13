#include <stdint.h>
#include "Utils.h"

// Blend two RGB565 colors with alpha (0..1)
uint16_t blendColor(uint16_t bg, uint16_t fg, float alpha)
{
  // Extract RGB565 components
  uint8_t r_bg = (bg >> 11) & 0x1F;
  uint8_t g_bg = (bg >> 5) & 0x3F;
  uint8_t b_bg = bg & 0x1F;

  uint8_t r_fg = (fg >> 11) & 0x1F;
  uint8_t g_fg = (fg >> 5) & 0x3F;
  uint8_t b_fg = fg & 0x1F;

  // Linear blend
  uint8_t r = r_bg + alpha * (r_fg - r_bg);
  uint8_t g = g_bg + alpha * (g_fg - g_bg);
  uint8_t b = b_bg + alpha * (b_fg - b_bg);

  return (r << 11) | (g << 5) | b;
}

// alpha: 0 = color1, 255 = color2
uint16_t blend_rgb565(uint16_t c1, uint16_t c2, uint8_t alpha)
{
  uint8_t inv = 255 - alpha;

  // Extract channels
  uint8_t r1 = (c1 >> 11) & 0x1F;
  uint8_t g1 = (c1 >> 5)  & 0x3F;
  uint8_t b1 =  c1        & 0x1F;

  uint8_t r2 = (c2 >> 11) & 0x1F;
  uint8_t g2 = (c2 >> 5)  & 0x3F;
  uint8_t b2 =  c2        & 0x1F;

  // Blend
  uint8_t r = (r1 * inv + r2 * alpha) >> 8;
  uint8_t g = (g1 * inv + g2 * alpha) >> 8;
  uint8_t b = (b1 * inv + b2 * alpha) >> 8;

  // Repack
  return (r << 11) | (g << 5) | b;
}

// alpha: 0 = color1, 255 = color2
uint16_t blend_rgb565_fast(uint16_t c1, uint16_t c2, uint8_t alpha)
{
  uint32_t rb = ((c1 & 0xF81F) * (255 - alpha) +
                 (c2 & 0xF81F) * alpha) >> 8;

  uint32_t g  = ((c1 & 0x07E0) * (255 - alpha) +
                 (c2 & 0x07E0) * alpha) >> 8;

  return (rb & 0xF81F) | (g & 0x07E0);
}


uint16_t avg_rgb565(uint16_t c1, uint16_t c2)
{
  return (((c1 ^ c2) & 0x0821) >> 1) + ((c1 & c2));
}

// alpha: 0..31
static inline uint16_t blend_rgb565_a5(uint16_t c1, uint16_t c2, uint8_t alpha)
{
  uint8_t inv = 31 - alpha;

  uint32_t rb = ((c1 & 0xF81F) * inv +
                 (c2 & 0xF81F) * alpha) >> 5;

  uint32_t g  = ((c1 & 0x07E0) * inv +
                 (c2 & 0x07E0) * alpha) >> 5;

  return (rb & 0xF81F) | (g & 0x07E0);
}

uint16_t fast_atan2(int16_t x, int16_t y)
{
    // Fast XY vector to integer degree algorithm - Jan 2011 www.RomanBlack.com
    // Converts any XY values including 0 to a degree value that should be
    // within +/- 1 degree of the accurate value without needing
    // large slow trig functions like ArcTan() or ArcCos().
    // NOTE! at least one of the X or Y values must be non-zero!
    // This is the full version, for all 4 quadrants and will generate
    // the angle in integer degrees from 0-360.
    // Any values of X and Y are usable including negative values provided
    // they are between -1456 and 1456 so the 16bit multiply does not overflow.

    unsigned char negflag;
    unsigned char tempdegree;
    unsigned char comp;
    unsigned int degree;     // this will hold the result
    //signed int x;            // these hold the XY vector at the start
    //signed int y;            // (and they will be destroyed)
    unsigned int ux;
    unsigned int uy;

    // Save the sign flags then remove signs and get XY as unsigned ints
    negflag = 0;
    if (x < 0)
    {
        negflag += 0x01;    // x flag bit
        x = (0 - x);        // is now +
    }

    ux = x;                // copy to unsigned var before multiply
    if (y < 0)
    {
        negflag += 0x02;    // y flag bit
        y = (0 - y);        // is now +
    }
    
    uy = y;                // copy to unsigned var before multiply

    // 1. Calc the scaled "degrees"
    if (ux > uy)
    {
        degree = (uy * 45) / ux;   // degree result will be 0-45 range
        negflag += 0x10;    // octant flag bit
    }
    else
    {
        degree = (ux * 45) / uy;   // degree result will be 0-45 range
    }

    // 2. Compensate for the 4 degree error curve
    comp = 0;
    tempdegree = degree;    // use an unsigned char for speed!

    if (tempdegree > 22)      // if top half of range
    {
        if (tempdegree <= 44) comp++;
        if (tempdegree <= 41) comp++;
        if (tempdegree <= 37) comp++;
        if (tempdegree <= 32) comp++;  // max is 4 degrees compensated
    }
    else    // else is lower half of range
    {
        if (tempdegree >= 2) comp++;
        if (tempdegree >= 6) comp++;
        if (tempdegree >= 10) comp++;
        if (tempdegree >= 15) comp++;  // max is 4 degrees compensated
    }

    degree += comp;   // degree is now accurate to +/- 1 degree!

    // Invert degree if it was X>Y octant, makes 0-45 into 90-45
    if (negflag & 0x10) degree = (90 - degree);

    // 3. Degree is now 0-90 range for this quadrant,
    // need to invert it for whichever quadrant it was in
    if (negflag & 0x02)   // if -Y
    {
        if (negflag & 0x01)   // if -Y -X
        {
            degree = (180 + degree);
        }
        else
        {
            // else is -Y +X
            degree = (180 - degree);
        }
    }
    else    // else is +Y
    {
        if (negflag & 0x01)   // if +Y -X
        {
            degree = (360 - degree);
        }
    }

    return degree;
}
