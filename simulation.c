#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bh_tree.h"
#include "graphics.h"

// SOME CONSTANTS

const int NUM_PARTICLES = 10000;  // number of particles

const double dt = 1e-3;

// squish factor 1 for complete circle and 0 for flat line
double alpha;

int color_mode;  // 0 color by mass, 1 for dist from center, 2 for force

particle* universe;
node root;

/*
 * Calculate and update net forces applied to each node in the tree
 */
void calculate_forces() {
  double G = 100.0 / NUM_PARTICLES;

  for (int i = 0; i < NUM_PARTICLES; i++) {
    universe[i]->force->x = 0;
    universe[i]->force->y = 0;
    approximate_force(universe[i], root, G);
  }
}

void bounce(particle p) {
  if (p->pos->x > 1) {
    p->pos->x = 2 * 1 - p->pos->x;
    p->vel->x = -p->vel->x;
  } else if (p->pos->x < 0) {
    p->pos->x = -p->pos->x;
    p->vel->x = -p->vel->x;
  }

  if (p->pos->y > 1) {
    p->pos->y = 2 - p->pos->y;
    p->vel->y = -p->vel->y;
  } else if (p->pos->y < 0) {
    p->pos->y = -p->pos->y;
    p->vel->y = -p->vel->y;
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

    // bounce(p);
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

  draw_points(universe, NUM_PARTICLES, color_mode);
}

/*
 * Initialize the universe of random particles
 */
void setup_universe() {
  for (int i = 0; i < NUM_PARTICLES; i++) {
    particle p = new_particle(i);
    universe[i] = p;
    p->mass = rand_in_range(0, 1);

    double r = rand_in_range(0, .25);
    double theta = rand_in_range(0, 2 * M_PI);

    p->pos->x = .5 + r * cos(theta);
    p->pos->y = .5 + alpha * r * sin(theta);

    double r_prime = distance(p->pos->x - .5, p->pos->y - .5);
    p->vel->x = -50 * r_prime * sin(theta);
    p->vel->y = 50 * r_prime * cos(theta);
  }
}

/*
 * Handle commandline args
 */
void init_sim(int argc, char* argv[]) {
  alpha = .3;
  color_mode = 2;

  for (int i = 0; i < argc; i++) {
    if (strcmp("-m", argv[i]) == 0) {
      color_mode = 0;
    } else if (strcmp("-d", argv[i]) == 0) {
      color_mode = 1;
    } else if (strcmp("-f", argv[i]) == 0) {
      color_mode = 2;
    } else if (strcmp("-a", argv[i]) == 0) {
      sscanf(argv[++i], "%lf", &alpha);
    }
  }
}

int main(int argc, char* argv[]) {
  universe = malloc(NUM_PARTICLES * sizeof(particle));

  init_sim(argc, argv);

  setup_universe();

  init(&argc, argv, display);

  glutMainLoop();

  for (int i = 0; i < NUM_PARTICLES; i++) {
    free_particle(universe[i]);
  }
  free(universe);
  return 0;
}
