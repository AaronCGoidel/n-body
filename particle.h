// A simple 2D vector with x and y values
typedef struct vec_t* vector;
struct vec_t {
  float x;
  float y;
};

// A particle in the universe
typedef struct particle_t* particle;
struct particle_t {
  vector pos;
  vector vel;
  vector acc;
};

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
