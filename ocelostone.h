#ifndef HEADER_OCELOSTONE
#define HEADER_OCELOSTONE

#include<GL/glut.h>
#include"oceloboard.h"

typedef enum OceloStoneColor {
  STONE_COLOR_WHITE,
  STONE_COLOR_BLACK
}OceloStoneColor;

typedef enum StoneState {
  STONE_STATE_BLACK,
  STONE_STATE_WHITE,
  STONE_STATE_REVERSE_B2W,
  STONE_STATE_REVERSE_W2B
}StoneState;

typedef struct StoneStateContena {
  StoneState state;
  int startTime;
  int stateTime;
  int shiftTime;
}StoneStateContena;

typedef struct Stone {
  OceloStoneColor stone;
  StoneStateContena state;
  GLdouble angle;
  GLint pos[2];
}Stone;

void StoneUpdate(Stone *stone);
Stone* InitOceloStone(int x, int y, OceloStoneColor type);
int DrawStone(Stone *stone);
void TriggerOfReverse(Stone *stone);

#endif