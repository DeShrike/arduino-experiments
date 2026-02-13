#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdint.h>
#include <math.h>

// Blend two RGB565 colors with alpha (0..1)
uint16_t blendColor(uint16_t bg, uint16_t fg, float alpha);

// alpha: 0 = color1, 255 = color2
uint16_t blend_rgb565(uint16_t c1, uint16_t c2, uint8_t alpha);

// alpha: 0 = color1, 255 = color2
uint16_t blend_rgb565_fast(uint16_t c1, uint16_t c2, uint8_t alpha);

uint16_t avg_rgb565(uint16_t c1, uint16_t c2);

// alpha: 0..31
static inline uint16_t blend_rgb565_a5(uint16_t c1, uint16_t c2, uint8_t alpha);

uint16_t fast_atan2(int16_t x, int16_t y);

#endif
