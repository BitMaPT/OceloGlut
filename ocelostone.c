#include<stdio.h>
#include<stdlib.h>
#include<GL/glut.h>
#include<math.h>
#include"ocelostone.h"

#define CIRCLE_VERTEX 100
#define REVERSE_STATE_TIME 500 //(ms)

void StateProgress(Stone *stone, StoneState next);
StoneStateContena InitStoneStateContena(StoneState state);
void StateAutoMaton(Stone *stone);
int DrawStone(Stone *stone);
GLdouble CalcAngle(Stone *stone);

void StoneUpdate(Stone *stone) {
  StateAutoMaton(stone);
  stone->angle = CalcAngle(stone);
}

//this function generates OceloStone and set initial setting
Stone* InitOceloStone(int x, int y, OceloStoneColor type) {
  Stone *stone;

  stone = (Stone*)malloc(sizeof(Stone));
  if(stone == NULL) return NULL;

  switch(type) {
    case STONE_COLOR_BLACK: 
      stone->stone = STONE_COLOR_BLACK;
      stone->state = InitStoneStateContena(STONE_STATE_BLACK);
      stone->angle = 0;
      break;
    case STONE_COLOR_WHITE:
      stone->stone = STONE_COLOR_WHITE;
      stone->state = InitStoneStateContena(STONE_STATE_WHITE);
      stone->angle = 180.0;
      break;
    default:
      printf("(%s)Error line:%d\n", __FILE__, __LINE__);
      exit(1);
  }

  stone->pos[0] = x;
  stone->pos[1] = y;

  return stone;
}

GLdouble CalcAngle(Stone *stone) {
  switch(stone->state.state) {
    case STONE_STATE_BLACK:
      return 0;
    case STONE_STATE_WHITE:
      return 180;
    case STONE_STATE_REVERSE_B2W:
      return ((GLdouble)180 / REVERSE_STATE_TIME) * stone->state.stateTime;
    case STONE_STATE_REVERSE_W2B:
      return ((GLdouble)180 / REVERSE_STATE_TIME) * stone->state.stateTime + 180.0;
  }
}

void StateAutoMaton(Stone *stone) {
  switch(stone->state.state) {
    case STONE_STATE_BLACK:
    case STONE_STATE_WHITE:
      return;
    case STONE_STATE_REVERSE_B2W
      StateProgress(stone, STONE_STATE_WHITE);
      return;
    case STONE_STATE_REVERSE_W2B:
      StateProgress(stone, STONE_STATE_BLACK);
      return;
  }
}

void StateProgress(Stone *stone, StoneState next) {
  if(stone->state.startTime == 0) stone->state.startTime = glutGet(GLUT_ELAPSED_TIME);

  stone->state.stateTime = glutGet(GLUT_ELAPSED_TIME) - stone->state.startTime;

  if(stone->state.stateTime > stone->state.shiftTime) {
    stone->state = InitStoneStateContena(next);
    //StateAutoMaton(stone);
  }

  return;
}

void StartReverse(Stone *stone) {
  switch(stone->stone) {
    case STONE_COLOR_BLACK:
      stone->state = InitStoneStateContena(STONE_STATE_REVERSE_B2W_FIRST);
      return;
    case STONE_COLOR_WHITE:
      stone->state = InitStoneStateContena(STONE_STATE_REVERSE_W2B_FIRST);
      return;
  }
}

StoneStateContena InitStoneStateContena(StoneState state) {
  StoneStateContena s = {state, 0, 0, 0};

  switch(state) {
    case STONE_STATE_BLACK:
    case STONE_STATE_WHITE:
      return s;
    case STONE_STATE_REVERSE_B2W:
    case STONE_STATE_REVERSE_W2B:
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

  if((stone->angle >= 0 && stone->angle < 90.0) || 
     (stone->angle >= 270.0 && stone->angle < 360.0))     glColor3d(1, 1, 1);
  if(stone->angle >= 90.0 && stone->angle < 270.0)        glColor3d(0, 0, 0);
  
  glRotated(stone->angle, 0, 1.0, 0);
  glTranslated(xx, yy, 0);
  glBegin(GL_POLYGON);
  for(i = 0; i < CIRCLE_VERTEX; i++) {
    cx = 20 * cos(2 * M_PI * ((double)i / CIRCLE_VERTEX));
    cy = 20 * sin(2 * M_PI * ((double)i / CIRCLE_VERTEX));

    glVertex2d(cx, cy);
  }
  glEnd();

  return 1;
}

void TriggerOfReverse(Stone *stone) {
  switch(stone->state.state) {
    case STONE_STATE_BLACK:
      stone->state = InitStoneStateContena(STONE_STATE_REVERSE_B2W);
      stone->stone = STONE_WHITE;
      return;
    case STONE_STATE_WHITE:
      stone->state = InitStoneStateContena(STONE_STATE_REVERSE_W2B);
      stone->stone = STONE_BLACK;
      return;
    default:
      printf("(%s)Error line:%d\n", __FILE__, __LINE__);
      exit(1);
  }
}

