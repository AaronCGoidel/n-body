#include <stdio.h>
#include <stdlib.h>

#include "bh_tree.h"

int NUM_PARTICLES = 5;
particle* universe;
node root;

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

int main() {
  universe = malloc(NUM_PARTICLES * sizeof(particle));

  node root = new_node();
  particle p1 = new_particle();

  add_to_tree(p1, root);
}