#include "particle.h"

#include <stdlib.h>

node new_node() {
  node new = malloc(sizeof(struct node_t));
  new->children = NULL;
  new->particle = NULL;

  return new;
}
