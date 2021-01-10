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