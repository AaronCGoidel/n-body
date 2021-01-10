#include "particle.h"
extern const int NUM_KIDS;

// A node in the Barnes Hut tree
typedef struct node_t* node;
struct node_t {
  particle particle;
  node* children;
  vector min, max;  // bounds for close neighbors
};

// Actions on individual nodes

node new_node();
int has_children(node n);
void set_bounds(node n);

// For adding nodes to the tree and tree manip

node find_subtree(particle particle, node root);
void add_to_tree(particle new_particle, node root);