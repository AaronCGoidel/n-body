#include "util.h"

// A particle in the universe
typedef struct particle_t* particle;
struct particle_t {
  double mass;

  vector pos;
  vector vel;
  vector acc;
  vector force;
};

particle new_particle();