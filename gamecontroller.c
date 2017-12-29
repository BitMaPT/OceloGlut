#include"ocelo.h"
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

//OceloPlayer playerTurn = PLAYER_BLACK;

Stone oceloStone[OCELO_HEIGHT][OCELO_WIDTH];
Object *objectList;

void InitGame() {
  int x, y;

  for(y = 0; y < OCELO_HEIGHT; y++) {
    for(x = 0; x < OCELO_WIDTH; x++) {
      ocelo[y][x] = STONE_NONE;
    }
  }

  ocelo[3][3] = STONE_BLACK;
  ocelo[3][4] = STONE_WHITE;
  ocelo[4][3] = STONE_WHITE;
  ocelo[4][4] = STONE_BLACK;

  playerTurn = PLAYER_BLACK;

  CheckAllSquaresToPut(STONE_BLACK);
}

void ClickBoard(int x, int y) {
  int xx, yy;
  OceloStone stone;

  if(!MousePositionToSquarePosition(x, y, &xx, &yy)) return;
  if(!oceloCanPut[yy][xx]) return;

  if(playerTurn == PLAYER_BLACK) stone = STONE_BLACK;
  if(playerTurn == PLAYER_WHITE) stone = STONE_WHITE;

  PutStone(xx, yy, stone);
  ExchangeTurn();
  RenewStatus();
}

void ExchangeTurn() {
  switch(playerTurn) {
    case PLAYER_BLACK: 
      playerTurn = PLAYER_WHITE;
      break;
    case PLAYER_WHITE:
      playerTurn = PLAYER_BLACK;
      break;
  }
}

void RenewStatus() {
  OceloStone stone;
  
  if(playerTurn == PLAYER_BLACK) stone = STONE_BLACK;
  if(playerTurn == PLAYER_WHITE) stone = STONE_WHITE;

  CheckAllSquaresToPut(stone);
}

int MousePositionToSquarePosition(int x, int y, int *xx, int *yy) {
  
}

