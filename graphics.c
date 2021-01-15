#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "particle.h"

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
void render_particle(particle p, int color_mode) {
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
void draw_points(particle* universe, int size, int color_mode) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();

  glBegin(GL_POINTS);
  glColor3f(1.0, 1.0, 1.0);
  draw_circle(-100, -100, NULL);
  for (int i = 0; i < size; i++) {
    render_particle(universe[i], color_mode);
  }
  glEnd();

  glPopMatrix();
  glutSwapBuffers();
}

void reshape(int width, int height) { glViewport(0, 0, width, height); }

void init(int* argc, char** argv, void* display_fn) {
  glutInit(argc, argv);

  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(1000, 1000);

  glutCreateWindow("Simulation by Aaron Goidel");

  glutDisplayFunc(display_fn);
  glutReshapeFunc(reshape);
  glutIdleFunc(glutPostRedisplay);
}