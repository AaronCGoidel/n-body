#include "particle.h"
extern const int NUM_KIDS;

// A node in the Barnes Hut tree
typedef struct node_t* node;
struct node_t {
  particle particle;
  node* children;
  vector min, max;  // bounds for close neighbors
};

/* Creates a new node for the Barnes Hut tree
 */
node new_node();
/*
 * Check if the given node has any children
 * Returns 1 iff yes, 0 otherwise
 */
int has_children(node n);
/*
 * Sets the bounds for the given node based on parents bounds
 * this divides the space for calculating when to approximate
 */
void set_bounds(node n);

/*
 * Returns the root of the correct subtree in which to place the given node
 */
node find_subtree(particle particle, node root);

/*
 * Adds the given particle to the tree with the given root
 */
void add_to_tree(particle new_particle, node root);
