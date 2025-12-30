#include <math.h>
#include "3d.h"

Point translate_z(Point p, float dz)
{
	return (Point) { .x = p.x, .y = p.y, .z = p.z + dz };
}

Point rotate_xz(Point p, float angle)
{
	float c = cos(angle);
	float s = sin(angle);
	return (Point) {
		.x = p.x * c - p.z * s,
		.y = p.y,
		.z = p.x * s + p.z * c,
	};
}

Point rotate_xy(Point p, float angle)
{
  float c = cos(angle);
  float s = sin(angle);
  return (Point) {
    .x = p.x * c - p.y * s,
    .y = p.x * s + p.y * c,
    .z = p.z,
  };
}

Point project(Point p)
{
	return (Point) {
		.x = p.x / p.z,
		.y = p.y / p.z,
		.z = 0,
	};
}

void toscreen(Point p, uint8_t *x, uint8_t *y, uint8_t width, uint8_t height)
{
	*x = (p.x + 1.0) / 2.0 * width;
	*y = (1.0 - (p.y + 1) / 2.0) * height;
}

void calc(Point p, uint8_t *x, uint8_t *y, float dz, float angle,
          uint8_t width, uint8_t height)
{
  Point rotated = rotate_xz(p, angle);
	rotated = rotate_xy(rotated, angle);
	Point translated = translate_z(rotated, dz);
	Point projected = project(translated);
	toscreen(projected, x, y, width, height);
}
