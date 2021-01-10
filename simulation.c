#include <stdio.h>
#include <stdlib.h>

#include "bh_tree.h"

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