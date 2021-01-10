#include "bh_tree.h"

#include <stdlib.h>

const int NUM_KIDS = 4;

/*
 * Creates a new node for the Barnes Hut tree
 */
node new_node() {
  node new = malloc(sizeof(struct node_t));
  new->children = NULL;
  new->particle = NULL;
  new->mass = 0;
  new->com = NULL;

  return new;
}

/*
 * Check if the given node has any children
 * Returns 1 iff yes, 0 otherwise
 */
int has_children(node n) {
  if (n->children == NULL) return 0;
  return 1;
}

/*
 * Sets the bounds for the given node based on parents bounds
 * this divides the space for calculating when to approximate
 */
void set_bounds(node n) {
  struct vec_t min = *n->min;
  struct vec_t max = *n->max;
  double x_avg = (min.x + max.x) / 2;
  double y_avg = (min.y + max.y) / 2;

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
  double x_avg = (root->min->x + root->max->x) / 2;
  double y_avg = (root->min->y + root->max->y) / 2;

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
    if (!has_children(root)) {
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
  if (has_children(root)) {
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

  if (has_children(root)) {
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
    scale(root->com, 1 / mass);
  } else {
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
