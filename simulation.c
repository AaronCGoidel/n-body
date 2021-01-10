#include <stdio.h>
#include <stdlib.h>

#include "particle.h"

int NUM_KIDS = 4;

/*
 * Returns the root of the correct subtree in which to place the given node
 */
node find_subtree(particle particle, node root) {
  vector pos = particle->pos;
  int target = 3;
  float x_avg = (root->min->x + root->max->x) / 2;
  float y_avg = (root->min->y + root->max->y) / 2;

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

int main() {
  printf("foo\n");
  node root = malloc(sizeof(node));
  printf("%d\n", has_children(root));
  root->children = malloc(NUM_KIDS * sizeof(node));
  printf("%d\n", has_children(root));
  for (int i = 0; i < NUM_KIDS; i++) {
    root->children[i] = new_node();
    printf("%d\n", has_children(root->children[i]));
  }
}