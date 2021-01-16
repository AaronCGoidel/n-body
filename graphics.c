#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif

#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "particle.h"

int win;
int is_paused;
int color_mode;
char *count_msg, *fps_msg;
int frame = 0, time, timebase = 0;
float fps;

/*
 * Draws a circle centered at (x, y) filled with color
 * (blue if none is given)
 */
void draw_circle(double x, double y, float* color) {
  glPushMatrix();

  glScalef(2.0, 2.0, 1.0);
  glTranslatef(-0.5, -0.5, 0);

  if (color == NULL) {
    glColor3f(0.2, 0.3, 0.5);
  } else {
    glColor3f(color[0], color[1], color[2]);
  }

  glBegin(GL_POLYGON);

  for (double i = 0; i < 2 * M_PI; i += M_PI / 12.0)
    glVertex3f(cos(i) * .001 + x, sin(i) * .001 + y, 0.0);
  glEnd();

  glPopMatrix();
}

/*
 * Draws a dot on the screen representing the given particle, colored according
 * to mass
 */
void render_particle(particle p) {
  double x = p->pos->x;
  double y = p->pos->y;
  float* color;
  double d, f_mag;
  switch (color_mode) {
    case 0:
      color = hsv_to_rgb(205, (p->mass) * 100, 50);
      break;
    case 1:
      d = distance(x - .5, y - .5);
      color = hsv_to_rgb((d)*180, 80, 50);
      break;
    case 2:
      f_mag = magnitude(p->force) / 2.0;
      if (f_mag > 255) {
        f_mag = 255;
      }
      color = hsv_to_rgb(255 - f_mag, 80, 50);
      break;
  }

  draw_circle(x, y, color);
  free(color);
}

/*
 * Renders all particles in universe to the screen
 * pass 0 to color particles by mass, 1 to color_by_dist to color particles in a
 * gradient based on their distance from the focus, 2 to color them by magnitude
 * of net force.
 */
void draw_points(particle* universe, int size) {
  glBegin(GL_POINTS);
  glColor3f(1.0, 1.0, 1.0);
  draw_circle(-100, -100, NULL);
  for (particle* p_ptr = universe; p_ptr < universe + size; p_ptr++) {
    render_particle(*p_ptr);
  }
  glEnd();
}

/*
 * Draws the given string to the screen at (x, y, z)
 */
void draw_msg(float x, float y, float z, char* msg) {
  glColor3f(1.0, 1.0, 1.0);
  glRasterPos3f(x, y, z);

  for (char* c = msg; *c != '\0'; c++) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
  }
}

/*
 * Render the simulation consisting of the given universe of particles in the
 * given color mode
 */
void draw(particle* universe, int size, int color_mode) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();

  // display pause message if paused
  if (is_paused) {
    draw_msg(0.8, 0.9, 0.0, "Paused");
  }

  // display number of particles
  draw_msg(-.95, 0.92, 0.0, count_msg);

  // display FPS
  sprintf(fps_msg, "FPS: %.2f", fps);
  draw_msg(-0.95, 0.86, 0.0, fps_msg);

  draw_points(universe, size);
  glPopMatrix();
  glutSwapBuffers();
}

/*
 * Handler for all key events on this window
 */
void handle_key_evt(unsigned char key, int x, int y) {
  switch (key) {
    case 32:  // space bar - pause sim
      is_paused = is_paused ? 0 : 1;
      break;
    case 113:  // q - quit
      glutDestroyWindow(win);
      exit(0);
      break;
    // color modes
    case 49:  // 1 - mass
      color_mode = 0;
      break;
    case 50:  // 2 - distance
      color_mode = 1;
      break;
    case 51:  // 3 - force
      color_mode = 2;
      break;
    default:
      printf("%u\n", key);
      break;
  }
}

void reshape(int width, int height) { glViewport(0, 0, width, height); }

void idle() {
  frame++;
  time = glutGet(GLUT_ELAPSED_TIME);

  // calculate fps
  if (time - timebase > 1000) {
    fps = frame * 1000.0 / (time - timebase);
    timebase = time;
    frame = 0;
  }

  glutPostRedisplay();
}

void init(int* argc, char** argv, void* display_fn, int size) {
  glutInit(argc, argv);

  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(1000, 1000);

  win = glutCreateWindow("Simulation by Aaron Goidel");

  glutDisplayFunc(display_fn);
  glutReshapeFunc(reshape);
  glutIdleFunc(idle);
  glutKeyboardFunc(handle_key_evt);

  count_msg = (char*)malloc(30 * sizeof(char));
  setlocale(LC_NUMERIC, "");
  sprintf(count_msg, "Simulating %'d particles", size);

  fps_msg = (char*)malloc(8 * sizeof(char));
}