#include "particle.h"

#include <stdlib.h>

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

/* ========================
 * Functions for particles
 =========================*/

/*
 * Returns a new particle
 */
particle new_particle() {
  particle p = malloc(sizeof(struct particle_t));

  p->mass = 0;
  p->pos = NULL;
  p->vel = NULL;
  p->acc = NULL;

  return p;
}