#ifndef _PYRAMID_H_
#define _PYRAMID_H_

#include <stdint.h>

Point pyramid[] = {
  (Point) { .x = 0.25, .y = 0.25, .z = 0.25 },
  (Point) { .x = -0.25, .y = 0.25, .z = 0.25 },
  (Point) { .x = -0.25, .y = -0.25, .z = 0.25 },
  (Point) { .x = 0.25, .y = -0.25, .z = 0.25 },

  (Point) { .x = 0.0, .y = 0.0, .z = -0.25 },
};

// count, index, index, index, ..., count, index, index, ...
uint8_t pyramid_faces[] = {
  5, 0, 1, 2, 3, 0,
  3, 0, 4, 2,
  3, 1, 4, 3,
};

#endif // _PYRAMID_H_
