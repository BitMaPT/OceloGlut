#include<stdio.h>
#include<stdlib.h>
#include<GL/glut.h>
#include<math.h>
#include"ocelo.h"

#define CIRCLE_VERTEX 100

typedef struct Stone {
  OceloStone stone;
  int pos[2];
}Stone;

int DrawStone(Stone stone) {
  int xx, yy;

  xx = 40 + 70 * stone.pos[0] + 35;
  yy = 40 + 70 * stone.pos[1] + 35;

  int i;
  double cx, cy;

  if(stone.stone == STONE_WHITE) glColor3d(1, 1, 1);
  if(stone.stone == STONE_BLACK) glColor3d(0, 0, 0);

  glBegin(GL_POLYGON);
  for(i = 0; i < CIRCLE_VERTEX; i++) {
    cx = 20 * cos(2 * M_PI * ((double)i / CIRCLE_VERTEX)) + xx;
    cy = 20 * sin(2 * M_PI * ((double)i / CIRCLE_VERTEX)) + yy;

    glVertex2d(cx, cy);
  }
}