#include "particle.h"

#include <stdlib.h>

/*
 * Returns a new particle
 */
particle new_particle(int num) {
  particle p = malloc(sizeof(struct particle_t));

  p->mass = 0;
  p->pos = new_vec();
  p->vel = new_vec();
  p->acc = NULL;
  p->force = new_vec();

  p->num = num;

  return p;
}

void free_particle(particle p) {
  free(p->pos);
  free(p->vel);
  if (p->acc) {
    free(p->acc);
  }
  free(p->force);
}