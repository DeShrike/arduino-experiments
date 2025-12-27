#ifndef _3D_H_
#define _3D_H_

#include <stdint.h>
#include "3d.h"

typedef struct {
	float x;
	float y;
	float z;
} Point;

Point translate_z(Point p, float dz);
Point rotate_xz(Point p, float angle);
Point rotate_xy(Point p, float angle);
void toscreen(Point p, uint8_t *x, uint8_t *y, uint8_t width, uint8_t height);
Point project(Point p);
void calc(Point p, uint8_t *x, uint8_t *y,
          float dz, float angle,
          uint8_t width, uint8_t height);

#endif // _3D_H_
