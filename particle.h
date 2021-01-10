/* ========================
 * Vector
 =========================*/
// A simple 2D vector with x and y values
typedef struct vec_t* vector;
struct vec_t {
  double x;
  double y;
};

vector new_vec();
void scale(vector v, double scalar);

/* ========================
 * Particle
 =========================*/

// A particle in the universe
typedef struct particle_t* particle;
struct particle_t {
  double mass;

  vector pos;
  vector vel;
  vector acc;
};

particle new_particle();