#include<stdio.h>
#include<stdlib.h>
#include"gamecontroller.h"
#include"oceloboard.h"
#include"ocelostone.h"
#include"putpoint.h"

int CanPut(int x, int y, OceloStoneColor stone);
int CanPutRight(int x, int y, OceloStoneColor stone, OceloStoneColor enemy);
int CanPutLeft(int x, int y, OceloStoneColor stone, OceloStoneColor enemy);
int CanPutUp(int x, int y, OceloStoneColor stone, OceloStoneColor enemy);
int CanPutBelow(int x, int y, OceloStoneColor stone, OceloStoneColor enemy);
int CanPutUpRight(int x, int y, OceloStoneColor stone, OceloStoneColor enemy);
int CanPutUpLeft(int x, int y, OceloStoneColor stone, OceloStoneColor enemy);
int CanPutBelowRight(int x, int y, OceloStoneColor stone, OceloStoneColor enemy);
int CanPutBelowLeft(int x, int y, OceloStoneColor stone, OceloStoneColor enemy);

Stone* oceloBoard[OCELO_HEIGHT][OCELO_WIDTH];
int oceloCanPut[OCELO_HEIGHT][OCELO_WIDTH];

void PutStone(int x, int y, OceloStoneColor mine) {
  int i;
  OceloStoneColor enemy;

  if(mine == STONE_COLOR_WHITE) enemy = STONE_COLOR_BLACK;
  if(mine == STONE_COLOR_BLACK) enemy = STONE_COLOR_WHITE;

  oceloBoard[y][x] = InitOceloStone(x, y, mine);

  if(CanPutRight(x, y, mine, enemy)) {
    for(i = x + 1; i < OCELO_WIDTH; i++) {
      if(oceloBoard[y][i]->stone == mine) break;
      TriggerOfReverse(oceloBoard[y][i]);
    }
  }

  if(CanPutLeft(x, y, mine, enemy)) {
    for(i = x - 1; i >= 0; i--) {
      if(oceloBoard[y][i]->stone == mine) break;
      TriggerOfReverse(oceloBoard[y][i]);
    }
  }

  if(CanPutUp(x, y, mine, enemy)) {
    for(i = y - 1; i >= 0; i--) {
      if(oceloBoard[i][x]->stone == mine) break;
      TriggerOfReverse(oceloBoard[i][x]);
    }
  }

  if(CanPutBelow(x, y, mine, enemy)) {
    for(i = y + 1; i < OCELO_HEIGHT; i++) {
      if(oceloBoard[i][x]->stone == mine) break;
      TriggerOfReverse(oceloBoard[i][x]);
    }
  }

  if(CanPutUpRight(x, y, mine, enemy)) {
    for(i = 1; (x + i < OCELO_WIDTH) && (y - i >= 0); i++) {
      if(oceloBoard[y - i][x + i]->stone == mine) break;
      TriggerOfReverse(oceloBoard[y - i][x + i]);
    }
  }

  if(CanPutUpLeft(x, y, mine, enemy)) {
    for(i = 1; (x - i >= 0) && (y - i >= 0); i++) {
      if(oceloBoard[y - i][x - i]->stone == mine) break;
      TriggerOfReverse(oceloBoard[y - i][x - i]);
    }
  }

  if(CanPutBelowRight(x, y, mine, enemy)) {
    for(i = 1; (x + i < OCELO_WIDTH) && (y + i < OCELO_HEIGHT); i++) {
      if(oceloBoard[y + i][x + i]->stone == mine) break;
      TriggerOfReverse(oceloBoard[y + i][x + i]);
    }
  }

  if(CanPutBelowLeft(x, y, mine, enemy)) {
    for(i = 1; (x - i >= 0) && (y + i < OCELO_HEIGHT); i++) {
      if(oceloBoard[y + i][x - i]->stone == mine) break;
      TriggerOfReverse(oceloBoard[y + i][x - i]);
    }
  }
}

int GenerateSelectablePutPoint() {
  int x, y;

  for(x = 0; x < OCELO_WIDTH; x++) {
    for(y = 0; y < OCELO_HEIGHT; y++) {
      if(oceloCanPut[y][x]) {
        if(InitOceloPoint(x, y) == NULL) {
          printf("(%s)Error line:%d\n", __FILE__, __LINE__);
          exit(1);
        }
      }
    }
  }

  return 1;
}

int CheckAllSquaresToPut(OceloStoneColor stone) {
  int x, y;

  for(x = 0; x < OCELO_WIDTH; x++) {
    for(y = 0; y < OCELO_HEIGHT; y++) {
      oceloCanPut[y][x] = CanPut(x, y, stone);
    }
  }

  for(x = 0; x < OCELO_WIDTH; x++) {
    for(y = 0; y < OCELO_HEIGHT; y++) {
      if(oceloCanPut[y][x]) return 1;
    }
  }
  
  return 0;
}

int CanPut(int x, int y, OceloStoneColor mine) {
  int i;
  OceloStoneColor enemy;

  if(oceloBoard[y][x] != NULL) return 0;

  if(mine == STONE_COLOR_WHITE) enemy = STONE_COLOR_BLACK;
  if(mine == STONE_COLOR_BLACK) enemy = STONE_COLOR_WHITE;

  if(CanPutRight(x, y, mine, enemy)) return 1;
  if(CanPutLeft(x, y, mine, enemy)) return 1;
  if(CanPutUp(x, y, mine, enemy)) return 1;
  if(CanPutBelow(x, y, mine, enemy)) return 1;
  if(CanPutUpRight(x, y, mine, enemy)) return 1;
  if(CanPutUpLeft(x, y, mine, enemy)) return 1;
  if(CanPutBelowRight(x, y, mine, enemy)) return 1;
  if(CanPutBelowLeft(x, y, mine, enemy)) return 1;

  return 0;
}

int CanPutRight(int x, int y, OceloStoneColor mine, OceloStoneColor enemy) {
  int i;

  if(x == OCELO_WIDTH - 1) return 0;
  if(oceloBoard[y][x + 1] == NULL) return 0;
  if(oceloBoard[y][x + 1]->stone != enemy) return 0;

  for(i = x + 2; i < OCELO_WIDTH; i++) {
    if(oceloBoard[y][i] == NULL) return 0;
    if(oceloBoard[y][i]->stone == mine) return 1;
  }

  return 0;
}

int CanPutLeft(int x, int y, OceloStoneColor mine, OceloStoneColor enemy) {
  int i;

  if(x == 0) return 0;
  if(oceloBoard[y][x - 1] == NULL) return 0;
  if(oceloBoard[y][x - 1]->stone != enemy) return 0;

  for(i = x - 2; i >= 0; i--) {
    if(oceloBoard[y][i] == NULL) return 0;
    if(oceloBoard[y][i]->stone == mine) return 1;
  }

  return 0;
}

int CanPutUp(int x, int y, OceloStoneColor mine, OceloStoneColor enemy) {
  int i;
  
  if(y == 0) return 0;
  if(oceloBoard[y - 1][x] == NULL) return 0;
  if(oceloBoard[y - 1][x]->stone != enemy) return 0;

  for(i = y - 1; i >= 0; i--) {
    if(oceloBoard[i][x] == NULL) return 0;
    if(oceloBoard[i][x]->stone == mine) return 1;
  }

  return 0;
}

int CanPutBelow(int x, int y, OceloStoneColor mine, OceloStoneColor enemy) {
  int i;

  if(y == OCELO_HEIGHT - 1) return 0;
  if(oceloBoard[y + 1][x] == NULL) return 0;
  if(oceloBoard[y + 1][x]->stone != enemy) return 0;

  for(i = y + 1; i < OCELO_HEIGHT; i++) {
    if(oceloBoard[i][x] == NULL) return 0;
    if(oceloBoard[i][x]->stone == mine) return 1;
  }

  return 0;
}

int CanPutUpRight(int x, int y, OceloStoneColor mine, OceloStoneColor enemy) {
  int i;

  if(x == OCELO_WIDTH - 1) return 0;
  if(y == 0) return 0;
  if(oceloBoard[y - 1][x + 1] == NULL) return 0;
  if(oceloBoard[y - 1][x + 1]->stone != enemy) return 0;

  for(i = 1; (x + i < OCELO_WIDTH) && (y - i >= 0); i++) {
    if(oceloBoard[y - i][x + i] == NULL) return 0;
    if(oceloBoard[y - i][x + i]->stone == mine) return 1;
  }

  return 0;
}

int CanPutUpLeft(int x, int y, OceloStoneColor mine, OceloStoneColor enemy) {
  int i;

  if(x == 0) return 0;
  if(y == 0) return 0;
  if(oceloBoard[y - 1][x - 1] == NULL) return 0;
  if(oceloBoard[y - 1][x - 1]->stone != enemy) return 0;

  for(i = 1; (x - i >= 0) && (y - i >= 0); i++) {
    if(oceloBoard[y - i][x - i] == NULL) return 0;
    if(oceloBoard[y - i][x - i]->stone == mine) return 1;
  }

  return 0;
}

int CanPutBelowRight(int x, int y, OceloStoneColor mine, OceloStoneColor enemy) {
  int i;

  if(x == OCELO_WIDTH - 1) return 0;
  if(y == OCELO_HEIGHT - 1) return 0;
  if(oceloBoard[y + 1][x + 1] == NULL) return 0;
  if(oceloBoard[y + 1][x + 1]->stone != enemy) return 0;

  for(i = 1; (x + i < OCELO_WIDTH) && (y + i < OCELO_HEIGHT); i++) {
    if(oceloBoard[y + i][x + i] == NULL) return 0;
    if(oceloBoard[y + i][x + i]->stone == mine) return 1;
  }

  return 0;
}

int CanPutBelowLeft(int x, int y, OceloStoneColor mine, OceloStoneColor enemy) {
  int i;
  
  if(x == 0) return 0;
  if(y == OCELO_HEIGHT - 1) return 0;
  if(oceloBoard[y + 1][x - 1] == NULL) return 0;
  if(oceloBoard[y + 1][x - 1]->stone != enemy) return 0;

  for(i = 1; (x - i >= 0) && (y + i < OCELO_HEIGHT); i++) {
    if(oceloBoard[y + i][x - i] == NULL) return 0;
    if(oceloBoard[y + i][x - i]->stone == mine) return 1;
  } 

  return 0;
}

int CheckAllStoneReversed() {
  int x, y;

  for(y = 0; y < OCELO_HEIGHT; y++) {
    for(x = 0; x < OCELO_WIDTH; x++) {
      if(oceloBoard[y][x]->state.state == STONE_STATE_REVERSE_B2W) return 0;
      if(oceloBoard[y][x]->state.state == STONE_STATE_REVERSE_W2B) return 0;
    }
  }

  return 1;
}

int MousePositionToSquarePosition(int x, int y, int *xx, int *yy) {
  if(x < 40 || x >= 40 + 70 * OCELO_WIDTH)  return 0;
  if(y < 40 || y >= 40 + 70 * OCELO_HEIGHT) return 0;

  x = x - 40;
  y = y - 40;

  *xx = x / 70;
  *yy = y / 70;

  return 1;
}