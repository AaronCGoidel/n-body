#include <GLUT/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "bh_tree.h"
#include "graphics.h"

int NUM_PARTICLES = 10000;
particle* universe;
node root;
const double dt = 8e-4;
const double alpha = .3;

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
  root = new_node();

  for (int i = 0; i < NUM_PARTICLES; i++) {
    add_to_tree(universe[i], root);
  }

  update_com(root);
  calculate_forces();
  apply_forces();

  free_node(root);
  free(root);
}

void display() {
  do_tick();

  draw_points(universe, NUM_PARTICLES);
}

void setup_universe() {
  for (int i = 0; i < NUM_PARTICLES; i++) {
    particle p = new_particle(i);
    universe[i] = p;
    p->mass = 1;

    double r = rand_in_range(0, .25);
    double theta = rand_in_range(0, 2 * M_PI);

    p->pos->x = .5 + r * cos(theta);
    p->pos->y = .5 + alpha * r * sin(theta);

    double r_prime = distance(p->pos->x - .5, p->pos->y - .5);
    p->vel->x = -50 * r_prime * sin(theta);
    p->vel->y = 50 * r_prime * cos(theta);
  }
}

int main(int argc, char* argv[]) {
  universe = malloc(NUM_PARTICLES * sizeof(particle));

  setup_universe();

  init(&argc, argv, display);

  glutMainLoop();

  return 0;
}
