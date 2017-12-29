#include<stdio.h>
#include<stdlib.h>
#include<GL/glut.h>
#include<math.h>
#include"ocelo.h"

#define CIRCLE_VERTEX 100
#define REVERSE_STATE_TIME 500 //(ms)

typedef enum StoneState {
  STONE_STATE_INIT,
  STONE_STATE_BLACK,
  STONE_STATE_WHITE,
  STONE_STATE_REVERSE_B2W_FIRST,
  STONE_STATE_REVERSE_B2W_SECOND,
  STONE_STATE_REVERSE_W2B_FIRST,
  STONE_STATE_REVERSE_W2B_SECOND
}StoneState;

typedef struct StoneStateContena {
  StoneState state;
  int startTime;
  int stateTime;
  int shiftTime;
}StoneStateContena;

typedef struct Stone {
  OceloStone stone;
  StoneStateContena state;
  GLdouble angle;
  GLint pos[2];
}Stone;

void StateProgress(Stone *stone, StoneState next);
StoneStateContena InitStateContena(StoneState state);
void StateAutoMaton(Stone *stone);
int DrawStone(Stone *stone);
GLdouble CalcAngle(Stone *stone);

void StoneUpdate(Stone *stone) {
  StateAutoMaton(stone);
  stone->angle = CalcAngle(stone);
  DrawStone(stone);
}

GLdouble CalcAngle(Stone *stone) {
  switch(stone->state.state) {
    case STONE_STATE_BLACK:
    case STONE_STATE_WHITE:
    case STONE_STATE_INIT:
      return 0;
    case STONE_STATE_REVERSE_B2W_FIRST:
    case STONE_STATE_REVERSE_W2B_FIRST:
      return (90.0 / REVERSE_STATE_TIME) * stone->state.stateTime;
    case STONE_STATE_REVERSE_B2W_SECOND:
    case STONE_STATE_REVERSE_W2B_SECOND:
      return (90.0 / REVERSE_STATE_TIME) * stone->state.stateTime + 90;
  }
}

void StateAutoMaton(Stone *stone) {
  static int startTime = 0;
  int stateTime;

  switch(stone->state.state) {
    case STONE_STATE_INIT:
      switch(stone->stone) {
        case STONE_BLACK: stone->state = InitStateContena(STONE_STATE_BLACK); return;
        case STONE_WHITE: stone->state = InitStateContena(STONE_STATE_WHITE); return;
        case STONE_NONE: printf("(%s)Error line:%d\n", __FILE__, __LINE__); exit(1);
      }
    case STONE_STATE_BLACK:
    case STONE_STATE_WHITE:
      return;
    case STONE_STATE_REVERSE_B2W_FIRST:
      StateProgress(stone, STONE_STATE_REVERSE_B2W_SECOND);
      return;
    case STONE_STATE_REVERSE_B2W_SECOND:
      StateProgress(stone, STONE_STATE_WHITE);
      return;
    case STONE_STATE_REVERSE_W2B_FIRST:
      StateProgress(stone, STONE_STATE_REVERSE_W2B_SECOND);
      return;
    case STONE_STATE_REVERSE_W2B_SECOND:
      StateProgress(stone, STONE_STATE_BLACK);
      return;
  }
}

void StateProgress(Stone *stone, StoneState next) {
  if(stone->state.startTime == 0) stone->state.startTime = glutGet(GLUT_ELAPSED_TIME);

  stone->state.stateTime = glutGet(GLUT_ELAPSED_TIME) - stone->state.startTime;

  if(stone->state.stateTime > stone->state.shiftTime) {
    stone->state = InitStateContena(next);
    StateAutoMaton(stone);
  }

  return;
}

StoneStateContena InitStateContena(StoneState state) {
  StoneStateContena s = {state, 0, 0, 0};

  switch(state) {
    case STONE_STATE_INIT:
    case STONE_STATE_BLACK:
    case STONE_STATE_WHITE:
      return s;
    case STONE_STATE_REVERSE_B2W_FIRST:
    case STONE_STATE_REVERSE_B2W_SECOND:
    case STONE_STATE_REVERSE_W2B_FIRST:
    case STONE_STATE_REVERSE_W2B_SECOND:
      s.shiftTime = REVERSE_STATE_TIME;
      return s;
  }
}

int DrawStone(Stone *stone) {
  int xx, yy;

  xx = 40 + 70 * stone->pos[0] + 35;
  yy = 40 + 70 * stone->pos[1] + 35;

  int i;
  double cx, cy;

  if(stone->stone == STONE_WHITE) glColor3d(1, 1, 1);
  if(stone->stone == STONE_BLACK) glColor3d(0, 0, 0);
  glRotated(stone->angle, 0, 1.0, 0);
  glBegin(GL_POLYGON);
  for(i = 0; i < CIRCLE_VERTEX; i++) {
    cx = 20 * cos(2 * M_PI * ((double)i / CIRCLE_VERTEX)) + xx;
    cy = 20 * sin(2 * M_PI * ((double)i / CIRCLE_VERTEX)) + yy;

    glVertex2d(cx, cy);
  }
  glEnd();

  return 1;
}

