#include "bh_tree.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

const int NUM_KIDS = 4;
const double epsilon = 1e-1;

/*
 * Creates a new node for the Barnes Hut tree
 */
node new_node() {
  node new = malloc(sizeof(struct node_t));
  new->children = NULL;
  new->particle = NULL;
  new->mass = 0;
  new->com = NULL;
  new->min = new_vec();
  new->max = new_vec();
  new->max->x = 1;
  new->min->x = 0;
  new->max->y = 1;
  new->min->y = 0;

  return new;
}

/*
 * Recursively free node and its children
 */
void free_node(node n) {
  if (n->children != NULL) {
    for (int i = 0; i < NUM_KIDS; i++) {
      free_node(n->children[i]);
    }
    free(n->children);
  }
  free(n->min);
  free(n->max);
  if (n->com != NULL) {
    free(n->com);
  }
}

/*
 * Check if the given node has any children
 * Returns 1 iff yes, 0 otherwise
 */
int has_children(node n) { return n->children != NULL; }

/*
 * Print the given tree to the console
 * pass 1 for mass to print the COM and mass of each node
 */
void disp(node root, int indent, int mass) {
  if (indent == 0) {
    printf("===============\n");
  }

  if (root == NULL) {
    printf("NULL!\n");
    return;
  }

  for (int i = 0; i < indent; i++) {
    printf("  ");
  }

  if (root->particle == NULL) {
    printf("---\n");
  } else {
    printf("%d", root->particle->num);
    if (mass) {
      printf(": %f @ (%f, %f)\n", root->mass, root->com->x, root->com->y);
    } else {
      printf("\n");
    }
  }

  if (root->children != NULL) {
    for (int i = 0; i < NUM_KIDS; i++) {
      disp(root->children[i], indent + 1, mass);
    }
  }
}

/*
 * Sets the bounds for the given node based on parents bounds
 * this divides the space for calculating when to approximate
 */
void set_bounds(node n) {
  struct vec_t min = *n->min;
  struct vec_t max = *n->max;
  double x_avg = (min.x + max.x) / 2.0;
  double y_avg = (min.y + max.y) / 2.0;

  // bounds for first child
  n->children[0]->min->x = min.x;
  n->children[0]->min->y = min.y;
  n->children[0]->max->x = x_avg;
  n->children[0]->max->y = y_avg;

  // bounds for second child
  n->children[1]->min->x = x_avg;
  n->children[1]->min->y = min.y;
  n->children[1]->max->x = max.x;
  n->children[1]->max->y = y_avg;

  // bounds for third child
  n->children[2]->min->x = min.x;
  n->children[2]->min->y = y_avg;
  n->children[2]->max->x = x_avg;
  n->children[2]->max->y = max.y;

  // bounds for fourth child
  n->children[3]->min->x = x_avg;
  n->children[3]->min->y = y_avg;
  n->children[3]->max->x = max.x;
  n->children[3]->max->y = max.y;
}

/*
 * Returns the root of the correct subtree in which to place the given node
 */
node find_subtree(particle particle, node root) {
  vector pos = particle->pos;
  int target = 3;
  double x_avg = (root->min->x + root->max->x) / 2.0;
  double y_avg = (root->min->y + root->max->y) / 2.0;

  // determine which quadrant to place node in
  if (pos->x <= x_avg && pos->y <= y_avg) {
    target = 0;
  } else if (pos->x > x_avg && pos->y < y_avg) {
    target = 1;
  } else if (pos->x < x_avg && pos->y > y_avg) {
    target = 2;
  }

  return root->children[target];
}

/*
 * Adds the given particle to the tree with the given root
 */
void add_to_tree(particle new_particle, node root) {
  if (root->particle == NULL) {
    // if the root node has no particle, set to given node
    root->particle = new_particle;
  } else {
    if (root->children == NULL) {
      // if the root has no children, we need to add children
      root->children = malloc(NUM_KIDS * sizeof(node));

      for (int i = 0; i < NUM_KIDS; i++) {
        root->children[i] = new_node();
      }
      set_bounds(root);

      // place root particle appropriately
      node root_loc = find_subtree(root->particle, root);
      add_to_tree(root->particle, root_loc);
    }
    // place the new particle into the tree
    node loc = find_subtree(new_particle, root);
    add_to_tree(new_particle, loc);
  }
}

/*
 * Recursively calculates the mass of the given node
 */
double find_mass_of_node(node root) {
  if (root->particle == NULL) {
    root->mass = 0;
    return 0;
  }
  if (root->children != NULL) {
    for (int i = 0; i < NUM_KIDS; i++) {
      root->mass += find_mass_of_node(root->children[i]);
    }
  } else {
    root->mass = root->particle->mass;
  }
  return root->mass;
}

/*
 * Calculates the center of mass for each node in the given tree
 */
vector find_com(node root) {
  vector com = new_vec();

  if (root->children != NULL) {
    // if the root has children, recurse to find their com's and update parent
    root->com = com;

    double mass = 0;
    for (int i = 0; i < NUM_KIDS; i++) {
      if (root->children[i]->particle != NULL) {
        vector child_com = find_com(root->children[i]);
        root->com->x += (child_com->x) * root->children[i]->mass;
        root->com->y += (child_com->y) * root->children[i]->mass;

        mass += root->children[i]->mass;
      }
    }
    scale(root->com, 1.0 / mass);
  } else {
    // if this is a leaf node, com is just position
    com->x = root->particle->pos->x;
    com->y = root->particle->pos->y;

    root->com = com;
  }

  return root->com;
}

/*
 * Updates masses for each node in tree with the given root
 */
void update_com(node root) {
  root->mass = find_mass_of_node(root);
  root->com = find_com(root);
}

/*
 * Updates the force attribute on a given particle
 */
void update_force_on_particle(particle particle, node root, double r,
                              double x_d, double y_d, double G) {
  vector force = particle->force;
  double force_multiple = -G * particle->mass * root->mass;
  force_multiple /= cube(r + epsilon);

  force->x += x_d * force_multiple;
  force->y += y_d * force_multiple;
}

/*
 * Applies Barnes Hut approximation to recursively calculate the forces on each
 * particle in the system
 */
void approximate_force(particle particle, node root, double G) {
  double r, x_d, y_d, c_squared;
  vector pos = particle->pos;
  vector com = root->com;

  // perform repeat steps upfront
  if (root->particle != NULL) {
    x_d = pos->x - com->x;
    y_d = pos->y - com->y;
    c_squared = square(x_d) + square(y_d);
  }

  if (root->children != NULL) {
    // apply fast inverse sqrt
    double inverse_r = inv_sqrt(c_squared);
    double theta = (root->max->x - root->min->x) * inverse_r;

    if (theta <= 0.5) {
      // if the node's COM is sufficiently far, we give up and update the net
      // force on the particle
      update_force_on_particle(particle, root, 1.0 / inverse_r, x_d, y_d, G);
    } else {
      // if we are still close enough, keep running the approximation further
      // down the tree
      for (int i = 0; i < NUM_KIDS; i++) {
        approximate_force(particle, root->children[i], G);
      }
    }
  } else if (root->particle != NULL && root->particle != particle) {
    r = sqrt(c_squared);
    update_force_on_particle(particle, root, r, x_d, y_d, G);
  }
}