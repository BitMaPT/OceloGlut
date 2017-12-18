#include<GL/glut.h>

void DrawBoard();

void DrawBoard() {
  int i;

  glClearColor(0, 0.7, 0, 1);
  
  glColor3d(0, 0, 0);
  glLineWidth(3);
  glBegin(GL_LINES);
  for(i = 0; i < 9; i++) {
    glVertex2d(40 + 70 * i, 40);
    glVertex2d(40 + 70 * i, 600);
    glVertex2d(40, 40 + 70 * i);
    glVertex2d(600, 40 + 70 * i);
  }
  glEnd();
  glLineWidth(1);

  glColor3d(0, 0, 0);
  glPointSize(10);
  glBegin(GL_POINTS);
  glVertex2d(40 + 70 * 2, 40 + 70 * 2);
  glVertex2d(40 + 70 * 2, 40 + 70 * 6);
  glVertex2d(40 + 70 * 6, 40 + 70 * 2);
  glVertex2d(40 + 70 * 6, 40 + 70 * 6);
  glEnd();

  
}