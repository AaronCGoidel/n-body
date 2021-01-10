#include <stdio.h>
#include <stdlib.h>

#include "bh_tree.h"

int NUM_PARTICLES = 5;
particle* universe;
node root;
const double dt = 1e-3;

/*
 * Calculate and update net forces applied to each node in the tree
 */
void calculate_forces() {
  for (int i = 0; i < NUM_PARTICLES; i++) {
    universe[i]->force->x = 0;
    universe[i]->force->y = 0;
    approximate_force(universe[i], root);
  }
}

/*
 * Applies each particle's net force to update acceleration, velocity, and
 * position
 */
void apply_forces() {
  for (int i = 0; i < NUM_PARTICLES; i++) {
    particle p = universe[i];
    scale(p->force, 1 / p->mass);
    p->acc = p->force;
    p->vel->x += p->acc->x * dt;
    p->vel->y += p->acc->y * dt;
    p->pos->x += p->vel->x * dt;
    p->pos->y += p->vel->y * dt;
  }
}

/*
 * Executes one tick in the simulation
 */
void do_tick() {
  calculate_forces();
  apply_forces();
  free_node(root);
  free(root);
}

int main() {
  universe = malloc(NUM_PARTICLES * sizeof(particle));

  node root = new_node();
  particle p1 = new_particle();

  add_to_tree(p1, root);
}