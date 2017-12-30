#ifndef HEADER_GAMECONTROLLER
#define HEADER_GAMECONTROLLER

#include"ocelostone.h"

typedef enum OceloPlayer {
  PLAYER_BLACK,
  PLAYER_WHITE
}OceloPlayer;

typedef enum GameState {
  GAMESTATE_INIT,
  GAMESTATE_BLACK_PLAYER,
  GAMESTATE_WHITE_PLAYER,
  GAMESTATE_REVERSE_W2B,
  GAMESTATE_REVERSE_B2W,
  GAMESTATE_ENDING,
  GAMESTATE_CALC_RESULT,
  GAMESTATE_SHOW_RESULT
}GameState;

void InitGame();
void WaitPutStone();
void StartTurn();
void ClickBoard();
int MousePositionToSquarePosition(int x, int y, int *xx, int *yy);
void RenewStatus();
void ExchangeTurn();

extern Stone* oceloBoard[OCELO_HEIGHT][OCELO_WIDTH];
extern int oceloCanPut[OCELO_HEIGHT][OCELO_WIDTH];

#endif