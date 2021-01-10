typedef struct vec_t* vector;
struct vec_t {
  float x;
  float y;
};

typedef struct particle_t* particle;
struct particle_t {
  vector pos;
  vector vel;
  vector acc;
};

typedef struct node_t* node;
struct node_t {
  particle particle;
  node* children;
  vector min, max;
};

node new_node();
