#include "particle.h"

#include <stdlib.h>

/*
 * Returns a new particle
 */
particle new_particle() {
  particle p = malloc(sizeof(struct particle_t));

  p->mass = 0;
  p->pos = NULL;
  p->vel = NULL;
  p->acc = NULL;
  p->force = NULL;

  return p;
}