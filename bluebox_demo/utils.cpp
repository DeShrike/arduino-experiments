#include <stdint.h>
#include "utils.h"

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
