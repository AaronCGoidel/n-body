#include "util.h"

#include <math.h>
#include <stdlib.h>
/*
 * Squares the given number
 */
double square(double n) { return n * n; }

/*
 * Cubes the given number
 */
double cube(double n) { return n * n * n; }

/*
 * Euclidian distance
 */
double distance(double a, double b) {
  double a2 = square(a);
  double b2 = square(b);

  return sqrt(a2 + b2);
}

/*
 * Returns a random number between min and max
 */
double rand_in_range(double min, double max) {
  double range = (max - min);
  double fact = RAND_MAX / range;
  return min + (rand() / fact);
}

/* ========================
 * Functions for vectors
 =========================*/

/*
 * Returns a new, empty vector
 */
vector new_vec() {
  vector vec = malloc(sizeof(struct vec_t));
  vec->x = 0;
  vec->y = 0;
  return vec;
}

/*
 * Scales the given vector by the given scalar
 */
void scale(vector v, double scalar) {
  v->x *= scalar;
  v->y *= scalar;
}

/*
 * Returns the magnitude of the given vector
 */
double magnitude(vector v) { return distance(v->x, v->y); }

/*
 * Converts a color in HSV to a color in RGB (as percents)
 */
float* hsv_to_rgb(float h, float s, float v) {
  s /= 100.0;
  v /= 100.0;
  float C = s * v;
  float X = C * (1 - fabs(fmod(h / 60.0, 2) - 1));
  float m = v - C;

  float* color = malloc(3 * sizeof(float));

  if (h >= 0 && h < 60) {
    color[0] = C;
    color[1] = X;
    color[2] = 0;
  } else if (h >= 120 && h < 180) {
    color[0] = 0;
    color[1] = C;
    color[2] = X;
  } else if (h >= 180 && h < 240) {
    color[0] = 0;
    color[1] = X;
    color[2] = C;
  } else if (h >= 240 && h < 300) {
    color[0] = X;
    color[1] = 0;
    color[2] = C;
  } else {
    color[0] = C;
    color[1] = 0;
    color[2] = X;
  }

  color[0] = (color[0] + m);
  color[1] = (color[1] + m);
  color[2] = (color[2] + m);

  return color;
}