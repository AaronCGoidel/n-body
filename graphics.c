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

  for (double i = 0; i < 2 * M_PI; i += M_PI / 12)
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
  float* color = hsv_to_rgb(205, p->mass * 100, 50);

  draw_circle(x, y, color);
}

/*
 * Renders all particles in universe to the screen
 */
void draw_points(particle* universe, int size) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();

  glBegin(GL_POINTS);
  glColor3f(1.0, 1.0, 1.0);
  draw_circle(-100, -100, NULL);
  for (int i = 0; i < size; i++) {
    render_particle(universe[i]);
  }
  glEnd();

  glPopMatrix();
  glutSwapBuffers();
}

void reshape(int width, int height) { glViewport(0, 0, width, height); }

void init(int* argc, char** argv, void* display_fn) {
  glutInit(argc, argv);

  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(800, 800);

  glutCreateWindow("Simulation by Aaron Goidel");

  glutDisplayFunc(display_fn);
  glutReshapeFunc(reshape);
  glutIdleFunc(glutPostRedisplay);
}