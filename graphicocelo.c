#include<GL/glut.h>
#include<math.h>
#include"ocelo.h"

#define CIRCLE_VERTEX 100

void DrawBoard();
void DrawStones();
void DrawStone(int x, int y);

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

void DrawStones() {
  int x, y;

  for(y = 0; y < OCELO_HEIGHT; y++) {
    for(x = 0; x < OCELO_WIDTH; x++) {
      DrawStone(x, y);
    }
  }
}

void DrawStone(int x, int y) {
  OceloStone stone;
  int xx, yy;

  stone = ocelo[y][x];
  if(stone != STONE_NONE) {
    xx = 40 + 70 * x + 35;
    yy = 40 + 70 * y + 35;

    int i;
    double cx, cy;

    if(stone == STONE_WHITE) glColor3d(1, 1, 1);
    if(stone == STONE_BLACK) glColor3d(0, 0, 0);

    glBegin(GL_POLYGON);
    for(i = 0; i < CIRCLE_VERTEX; i++) {
      cx = 20 * cos(2 * M_PI * ((double)i / CIRCLE_VERTEX)) + xx;
      cy = 20 * sin(2 * M_PI * ((double)i / CIRCLE_VERTEX)) + yy;

      glVertex2d(cx, cy);
    }
    glEnd();
  }
}