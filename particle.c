#include "particle.h"

#include <stdlib.h>

node new_node() {
  node new = malloc(sizeof(struct node_t));
  new->children = NULL;
  new->particle = NULL;

  return new;
}

int has_children(node n) {
  if (n->children == NULL) return 0;
  return 1;
}

void set_bounds(node n) {
  struct vec_t min = *n->min;
  struct vec_t max = *n->max;
  float x_avg = (min.x + max.x) / 2;
  float y_avg = (min.y + max.y) / 2;

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
