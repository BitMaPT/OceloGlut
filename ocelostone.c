#include<stdio.h>
#include<stdlib.h>
#include<GL/glut.h>
#include<math.h>
#include"ocelo.h"

#define CIRCLE_VERTEX 100
#define STONE_STATE_REVERSE_FIRST_TIME 500
#define STONE_STATE_REVERSE_SECOND_TIME 500

typedef enum StoneStateType {
  STONE_STATE_STABLE,
  STONE_STATE_REVERSE_FIRST,
  STONE_STATE_REVERSE_SECOND
}StoneStateType;

typedef struct StoneState {
  StoneStateType state;
  int startTime;
  int stateTime;
}StoneState;

typedef struct Stone {
  OceloStone stone;
  StoneState state;
  int pos[2];
}Stone;

int StoneStateAutoMaton(Stone *stone) {
  static int startTime = 0;
  int stateTime = 0;

  switch(stone->state.state) {
    case STONE_STATE_STABLE:
      return 1;
    case STONE_STATE_REVERSE_FIRST:
      if(startTime == 0) startTime = glutGet(GLUT_ELAPSED_TIME);

      stateTime = glutGet(GLUT_ELAPSED_TIME) - startTime;

      if(stateTime > STONE_STATE_REVERSE_FIRST_TIME) {
        stone->state.state = STONE_STATE_REVERSE_SECOND;
        StoneStateAutoMaton(stone);
      }
      return 1;
    case STONE_STATE_REVERSE_SECOND:
      if(startTime == 0) startTime = glutGet(GLUT_ELAPSED_TIME);

      stateTime = glutGet(GLUT_ELAPSED_TIME) - startTime;

      if(stateTime > STONE_STATE_REVERSE_SECOND_TIME) {
        stone->state.state = STONE_STATE_STABLE;
        StoneStateAutoMaton(stone);
      }
      return 1;
  }
}


int DrawStone(Stone stone) {
  int xx, yy;

  xx = 40 + 70 * stone.pos[0] + 35;
  yy = 40 + 70 * stone.pos[1] + 35;

  int i;
  double cx, cy;

  if(stone.stone == STONE_WHITE) glColor3d(1, 1, 1);
  if(stone.stone == STONE_BLACK) glColor3d(0, 0, 0);

  glRotated(60, 0, 1.0, 0);
  glBegin(GL_POLYGON);
  for(i = 0; i < CIRCLE_VERTEX; i++) {
    cx = 20 * cos(2 * M_PI * ((double)i / CIRCLE_VERTEX)) + xx;
    cy = 20 * sin(2 * M_PI * ((double)i / CIRCLE_VERTEX)) + yy;

    glVertex2d(cx, cy);
  }
  glEnd();

  return 1;
}

