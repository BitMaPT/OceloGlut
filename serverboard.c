#include<stdlib.h>
#include<stdio.h>
#include"ocelostone.h"
#include"serverboard.h"

int CanPut(int x, int y, OceloStoneColor mine, int board[OCELO_HEIGHT][OCELO_WIDTH]);
int CanPutRight(int x, int y, OceloStoneColor mine, OceloStoneColor enemy, int board[OCELO_HEIGHT][OCELO_WIDTH]);
int CanPutLeft(int x, int y, OceloStoneColor mine, OceloStoneColor enemy, int board[OCELO_HEIGHT][OCELO_WIDTH]);
int CanPutUp(int x, int y, OceloStoneColor mine, OceloStoneColor enemy, int board[OCELO_HEIGHT][OCELO_WIDTH]);
int CanPutBelow(int x, int y, OceloStoneColor mine, OceloStoneColor enemy, int board[OCELO_HEIGHT][OCELO_WIDTH]);
int CanPutUpRight(int x, int y, OceloStoneColor mine, OceloStoneColor enemy, int board[OCELO_HEIGHT][OCELO_WIDTH]);
int CanPutUpLeft(int x, int y, OceloStoneColor mine, OceloStoneColor enemy, int board[OCELO_HEIGHT][OCELO_WIDTH]);
int CanPutBelowRight(int x, int y, OceloStoneColor mine, OceloStoneColor enemy, int board[OCELO_HEIGHT][OCELO_WIDTH]);
int CanPutBelowLeft(int x, int y, OceloStoneColor mine, OceloStoneColor enemy, int board[OCELO_HEIGHT][OCELO_WIDTH]);


int Initboard(int board[OCELO_HEIGHT][OCELO_WIDTH]) {
  int x, y;

  for(y = 0; y < OCELO_HEIGHT; y++) {
    for(x = 0; x < OCELO_WIDTH; x++) {
      board[y][x] = -1;
    }
  }

  board[3][3] = STONE_COLOR_BLACK;
  board[3][4] = STONE_COLOR_WHITE;
  board[4][3] = STONE_COLOR_WHITE;
  board[4][4] = STONE_COLOR_BLACK;

  return 1;
}

int CheckPutablePoints(int board[OCELO_HEIGHT][OCELO_WIDTH], int *list, OceloStoneColor color) {
  int x, y;

  for(y = 0; y < OCELO_HEIGHT; y++) {
    for(x = 0; x < OCELO_WIDTH; x++) {
      list[y * OCELO_HEIGHT + x] = CanPut(x, y, color, board);
    }
  }

  return 1;
}

int CanPut(int x, int y, OceloStoneColor mine, int board[OCELO_HEIGHT][OCELO_WIDTH]) {
  int i;
  OceloStoneColor enemy;

  if(board[y][x] != -1) return 0;

  if(mine == STONE_COLOR_WHITE) enemy = STONE_COLOR_BLACK;
  if(mine == STONE_COLOR_BLACK) enemy = STONE_COLOR_WHITE;

  if(CanPutRight(x, y, mine, enemy, board)) return 1;
  if(CanPutLeft(x, y, mine, enemy, board)) return 1;
  if(CanPutUp(x, y, mine, enemy, board)) return 1;
  if(CanPutBelow(x, y, mine, enemy, board)) return 1;
  if(CanPutUpRight(x, y, mine, enemy, board)) return 1;
  if(CanPutUpLeft(x, y, mine, enemy, board)) return 1;
  if(CanPutBelowRight(x, y, mine, enemy, board)) return 1;
  if(CanPutBelowLeft(x, y, mine, enemy, board)) return 1;

  return 0;
}

int CanPutRight(int x, int y, OceloStoneColor mine, OceloStoneColor enemy, int board[OCELO_HEIGHT][OCELO_WIDTH]) {
  int i;

  if(x == OCELO_WIDTH - 1) return 0;
  if(board[y][x + 1] != enemy) return 0;

  for(i = x + 2; i < OCELO_WIDTH; i++) {
    if(board[y][i] == -1) return 0;
    if(board[y][i] == mine) return 1;
  }

  return 0;
}

int CanPutLeft(int x, int y, OceloStoneColor mine, OceloStoneColor enemy, int board[OCELO_HEIGHT][OCELO_WIDTH]) {
  int i;

  if(x == 0) return 0;
  if(board[y][x - 1] != enemy) return 0;

  for(i = x - 2; i >= 0; i--) {
    if(board[y][i] == -1) return 0;
    if(board[y][i] == mine) return 1;
  }

  return 0;
}

int CanPutUp(int x, int y, OceloStoneColor mine, OceloStoneColor enemy, int board[OCELO_HEIGHT][OCELO_WIDTH]) {
  int i;
  
  if(y == 0) return 0;
  if(board[y - 1][x] != enemy) return 0;

  for(i = y - 1; i >= 0; i--) {
    if(board[i][x] == -1) return 0;
    if(board[i][x] == mine) return 1;
  }

  return 0;
}

int CanPutBelow(int x, int y, OceloStoneColor mine, OceloStoneColor enemy, int board[OCELO_HEIGHT][OCELO_WIDTH]) {
  int i;

  if(y == OCELO_HEIGHT - 1) return 0;
  if(board[y + 1][x] != enemy) return 0;

  for(i = y + 1; i < OCELO_HEIGHT; i++) {
    if(board[i][x] == -1) return 0;
    if(board[i][x] == mine) return 1;
  }

  return 0;
}

int CanPutUpRight(int x, int y, OceloStoneColor mine, OceloStoneColor enemy, int board[OCELO_HEIGHT][OCELO_WIDTH]) {
  int i;

  if(x == OCELO_WIDTH - 1) return 0;
  if(y == 0) return 0;
  if(board[y - 1][x + 1] != enemy) return 0;

  for(i = 1; (x + i < OCELO_WIDTH) && (y - i >= 0); i++) {
    if(board[y - i][x + i] == -1) return 0;
    if(board[y - i][x + i] == mine) return 1;
  }

  return 0;
}

int CanPutUpLeft(int x, int y, OceloStoneColor mine, OceloStoneColor enemy, int board[OCELO_HEIGHT][OCELO_WIDTH]) {
  int i;

  if(x == 0) return 0;
  if(y == 0) return 0;
  if(board[y - 1][x - 1] != enemy) return 0;

  for(i = 1; (x - i >= 0) && (y - i >= 0); i++) {
    if(board[y - i][x - i] == -1) return 0;
    if(board[y - i][x - i] == mine) return 1;
  }

  return 0;
}

int CanPutBelowRight(int x, int y, OceloStoneColor mine, OceloStoneColor enemy, int board[OCELO_HEIGHT][OCELO_WIDTH]) {
  int i;

  if(x == OCELO_WIDTH - 1) return 0;
  if(y == OCELO_HEIGHT - 1) return 0;
  if(board[y + 1][x + 1] != enemy) return 0;

  for(i = 1; (x + i < OCELO_WIDTH) && (y + i < OCELO_HEIGHT); i++) {
    if(board[y + i][x + i] == -1) return 0;
    if(board[y + i][x + i] == mine) return 1;
  }

  return 0;
}

int CanPutBelowLeft(int x, int y, OceloStoneColor mine, OceloStoneColor enemy, int board[OCELO_HEIGHT][OCELO_WIDTH]) {
  int i;
  
  if(x == 0) return 0;
  if(y == OCELO_HEIGHT - 1) return 0;
  if(board[y + 1][x - 1] != enemy) return 0;

  for(i = 1; (x - i >= 0) && (y + i < OCELO_HEIGHT); i++) {
    if(board[y + i][x - i] == -1) return 0;
    if(board[y + i][x - i] == mine) return 1;
  } 

  return 0;
}