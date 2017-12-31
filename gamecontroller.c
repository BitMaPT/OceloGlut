#include<stdlib.h>
#include"ocelo.h"
#include"ocelostone.h"
#include"gamecontroller.h"

//OceloPlayer playerTurn = PLAYER_BLACK;
Stone *oceloBoard[OCELO_HEIGHT][OCELO_WIDTH];
int oceloCanPut[OCELO_HEIGHT][OCELO_WIDTH];
Object *objectList;
static GameState gameState;


void ControlGameWithState() {
  switch(gameState.broad) {
    case GAMESTATE_ALLY:
      ControlGameWithAllyState();
      return;
    case GAMESTATE_REVERSE_ALLY:
    case GAMESTATE_ENEMY:
    case GAMESTATE_REVERSE_ENEMY:
      return;
  }
}

void ControlGameWithAllyState() {
  switch(gameState.detail.allyState) {
    case ALLYSTATE_SYNC:
      //sync
      return;
    case ALLYSTATE_SHOW_YOURTURN:
      return;
    case ALLYSTATE_CHECKING_PUT:
      
      return;
    case ALLYSTATE_WAITING:
      return;
    case ALLYSTATE_NONEPUT:

      return;
  }
}

void GameControlWithAllyRevState() {
  switch(gameState.detail.allyRev) {
    case ALLYREV_READY:

      return;
    case ALLYREV_ANIMATION:

      return;
    case ALLYREV_END:

      return;
  }
}

void InitGame() {
  int x, y;

  for(y = 0; y < OCELO_HEIGHT; y++) {
    for(x = 0; x < OCELO_WIDTH; x++) {
      oceloBoard[y][x] = NULL;
    }
  }

  oceloBoard[3][3] = InitOceloStone(3, 3, STONE_COLOR_BLACK);
  oceloBoard[3][4] = InitOceloStone(3, 4, STONE_COLOR_WHITE);
  oceloBoard[4][3] = InitOceloStone(4, 3, STONE_COLOR_WHITE);
  oceloBoard[4][4] = InitOceloStone(4, 4, STONE_COLOR_BLACK);



  //playerTurn = PLAYER_BLACK;

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
