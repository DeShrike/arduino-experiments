#ifndef _CUBE_H_
#define _CUBE_H_

#include <stdint.h>

Point cube[] = {
  (Point) { .x = 0.25, .y = 0.25, .z = 0.25 },
  (Point) { .x = -0.25, .y = 0.25, .z = 0.25 },
  (Point) { .x = -0.25, .y = -0.25, .z = 0.25 },
  (Point) { .x = 0.25, .y = -0.25, .z = 0.25 },

  (Point) { .x = 0.25, .y = 0.25, .z = -0.25 },
  (Point) { .x = -0.25, .y = 0.25, .z = -0.25 },
  (Point) { .x = -0.25, .y = -0.25, .z = -0.25 },
  (Point) { .x = 0.25, .y = -0.25, .z = -0.25 },
};

// count, index, index, index, ..., count, index, index, ...
uint8_t cube_faces[] = {
  5, 0, 1, 2, 3, 0,
  5, 4, 5, 6, 7, 4,
  2, 0, 4,
  2, 1, 5,
  2, 2, 6,
  2, 3, 7,
};

#endif // _CUBE_H_
