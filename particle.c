#include "particle.h"

#include <stdlib.h>

/*
 * Returns a new particle
 */
particle new_particle() {
  particle p = malloc(sizeof(struct particle_t));

  p->mass = 0;
  p->pos = new_vec();
  p->vel = new_vec();
  p->acc = NULL;
  p->force = NULL;

  return p;
}