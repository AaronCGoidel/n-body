// some helpers and utils

// math

double square(double n);
double cube(double n);
double distance(double a, double b);
double rand_in_range(double min, double max);
double inv_sqrt(double n);

float* hsv_to_rgb(float h, float s, float v);

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
double magnitude(vector v);
