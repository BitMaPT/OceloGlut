#ifndef HEADER_OCELOSTONE
#define HEADER_OCELOSTONE

#include<GL/glut.h>
#include"ocelo.h"

typedef enum OceloStoneColor {
  STONE_WHITE,
  STONE_BLACK
}OceloStoneColor;

typedef enum StoneState {
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

void StoneUpdate(Stone *stone);
Stone* InitOceloStone(int x, int y, OceloStone type);
int DrawStone(Stone *stone);

#endif