#include <stdio.h>
#include <stdlib.h>

#include "bh_tree.h"

int main() {
  node root = new_node();
  particle p1 = new_particle();

  add_to_tree(p1, root);
}