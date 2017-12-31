#include<stdlib.h>
#include"gamecontroller.h"
#include"ocelo.h"
#include"ocelostone.h"

//int ocelo[OCELO_HEIGHT][OCELO_WIDTH];

void PutStone(int x, int y, OceloStoneColor mine) {
  int i;
  OceloStoneColor enemy;

  if(mine == STONE_COLOR_WHITE) enemy = STONE_COLOR_BLACK;
  if(mine == STONE_COLOR_BLACK) enemy = STONE_COLOR_WHITE;

  oceloBoard[y][x] = InitOceloStone(x, y, mine);

  if(CanPutRight(x, y, mine, enemy)) {
    for(i = x + 1; i < OCELO_WIDTH; i++) {
      if(oceloBoard[y][i]->stone == mine) break;
      //TODO implement
      exit(1);
    }
  }

  if(CanPutLeft(x, y, stone, enemy)) {
    for(i = x - 1; i >= 0; i--) {
      if(oceloBoard[y][i]->stone == stone) break;
      //TODO
      exit(1);
    }
  }

  if(CanPutUp(x, y, stone, enemy)) {
    for(i = y - 1; i >= 0; i--) {
      if(oceloBoard[i][x]->stone == stone) break;
      //TODO
      exit(1);
    }
  }

  if(CanPutBelow(x, y, stone, enemy)) {
    for(i = y + 1; i < OCELO_HEIGHT; i++) {
      if(oceloBoard[i][x]->stone == stone) break;
      //TODO
      exit(1);
    }
  }

  if(CanPutUpRight(x, y, stone, enemy)) {
    for(i = 1; (x + i < OCELO_WIDTH) && (y - i >= 0); i++) {
      if(oceloBoard[y - i][x + i]->stone == stone) break;
      //TODO
      exit(1);
    }
  }

  if(CanPutUpLeft(x, y, stone, enemy)) {
    for(i = 1; (x - i >= 0) && (y - i >= 0); i++) {
      if(oceloBoard[y - i][x - i]->stone == stone) break;
      //TODO
      exit(1);
    }
  }

  if(CanPutBelowRight(x, y, stone, enemy)) {
    for(i = 1; (x + i < OCELO_WIDTH) && (y + i < OCELO_HEIGHT); i++) {
      if(ocelo[y + i][x + i]->stone == stone) break;
      //TODO
      exit(1);
    }
  }

  if(CanPutBelowLeft(x, y, stone, enemy)) {
    for(i = 1; (x - i >= 0) && (y + i < OCELO_HEIGHT); i++) {
      if(ocelo[y + i][x - i]->stone == stone) break;
      //TODO
      exit(1); 
    }
  }
}

void CheckAllSquaresToPut(OceloStoneColor stone) {
  int x, y;

  for(x = 0; x < OCELO_WIDTH; x++) {
    for(y = 0; y < OCELO_HEIGHT; y++) {
      oceloCanPut[y][x] = CanPut(x, y, stone);
    }
  }
}

int CanPut(int x, int y, OceloStoneColor mine) {
  int i;
  OceloStoneColor enemy;

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

int CanPutRight(int x, int y, OceloStoneColor stone, OceloStoneColor enemy) {
  int i;

  if(x == OCELO_WIDTH - 1) return 0;
  if(ocelo[y][x + 1] != enemy) return 0;

  for(i = x + 1; i < OCELO_WIDTH; i++) {
    if(ocelo[y][i] == stone) return 1;
    if(ocelo[y][i] == OCELO_STONE_NONE) return 0;
  }

  return 0;
}

int CanPutLeft(int x, int y, OceloStoneColor stone, OceloStoneColor enemy) {
  int i;

  if(x == 0) return 0;
  if(ocelo[y][x - 1] != enemy) return 0;

  for(i = x - 1; i >= 0; i--) {
    if(ocelo[y][i] == stone) return 1;
    if(ocelo[y][i] == OCELO_STONE_NONE) return 0;
  }

  return 0;
}

int CanPutUp(int x, int y, OceloStoneColor stone, OceloStoneColor enemy) {
  int i;
  
  if(y == 0) return 0;
  if(ocelo[y - 1][x] != enemy) return 0;

  for(i = y - 1; i >= 0; i--) {
    if(ocelo[i][x] == stone) return 1;
    if(ocelo[i][x] == OCELO_STONE_NONE) return 0;
  }

  return 0;
}

int CanPutBelow(int x, int y, OceloStoneColor stone, OceloStoneColor enemy) {
  int i;

  if(y == OCELO_HEIGHT - 1) return 0;
  if(ocelo[y + 1][x] != enemy) return 0;

  for(i = y + 1; i < OCELO_HEIGHT; i++) {
    if(ocelo[i][x] == stone) return 1;
    if(ocelo[i][x] == OCELO_STONE_NONE) return 0;
  }

  return 0;
}

int CanPutUpRight(int x, int y, OceloStone stone, OceloStone enemy) {
  int i;

  if(x == OCELO_WIDTH - 1) return 0;
  if(y == 0) return 0;
  if(ocelo[y - 1][x + 1] != enemy) return 0;

  for(i = 1; (x + i < OCELO_WIDTH) && (y - i >= 0); i++) {
    if(ocelo[y - i][x + i] == stone) return 1;
    if(ocelo[y - i][x + i] == OCELO_STONE_NONE) return 0;
  }

  return 0;
}

int CanPutUpLeft(int x, int y, OceloStone stone, OceloStone enemy) {
  int i;

  if(x == 0) return 0;
  if(y == 0) return 0;
  if(ocelo[y - 1][x - 1] != enemy) return 0;

  for(i = 1; (x - i >= 0) && (y - i >= 0); i++) {
    if(ocelo[y - i][x - i] == stone) return 1;
    if(ocelo[y - i][x - i] == OCELO_STONE_NONE) return 0;
  }

  return 0;
}

int CanPutBelowRight(int x, int y, OceloStone stone, OceloStone enemy) {
  int i;

  if(x == OCELO_WIDTH - 1) return 0;
  if(y == OCELO_HEIGHT - 1) return 0;
  if(ocelo[y + 1][x + 1] != enemy) return 0;

  for(i = 1; (x + i < OCELO_WIDTH) && (y + i < OCELO_HEIGHT); i++) {
    if(ocelo[y + i][x + i] == stone) return 1;
    if(ocelo[y + i][x + i] == OCELO_STONE_NONE) return 0;
  }

  return 0;
}

int CanPutBelowLeft(int x, int y, OceloStone stone, OceloStone enemy) {
  int i;
  
  if(x == 0) return 0;
  if(y == OCELO_HEIGHT - 1) return 0;
  if(ocelo[y + 1][x - 1] != enemy) return 0;

  for(i = 1; (x - i >= 0) && (y + i < OCELO_HEIGHT); i++) {
    if(ocelo[y + i][x - i] == stone) return 1;
    if(ocelo[y + i][x - i] == OCELO_STONE_NONE) return 0;
  } 

  return 0;
}