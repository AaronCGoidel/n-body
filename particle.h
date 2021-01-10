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
