#include<stdio.h>
#include<stdlib.h>
#include"object.h"
#include"oceloboard.h"
#include"ocelostone.h"
#include"gamecontroller.h"

void WaitingClick();
int MousePositionToSquarePosition(int x, int y, int *xx, int *yy);
void CheckAllStoneReversed(GameBroadState broad);
void ControlGameWithAllyState();
void GameControlWithAllyRevState();
void GameControlWithEnemyState();
void GameControlWithEnemyRevState();



//OceloPlayer playerTurn = PLAYER_BLACK;
static GameState gameState;
ObjectList objectList;
OceloStoneColor playerColor;

void ControlGameWithState() {
  switch(gameState.broad) {
    case GAMESTATE_ALLY:
      ControlGameWithAllyState();
      return;
    case GAMESTATE_REVERSE_ALLY:
      GameControlWithAllyRevState();
      return;
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
      //nothing to do
      return;
    case ALLYSTATE_CHECKING_PUT:
      //checking puttable square
      //put puttable mark or show signal of no puttable square
      if(CheckAllSquaresToPut(playerColor)) {
        GenerateSelectablePutPoint();
        gameState.detail.allyState = ALLYSTATE_WAITING;
      } else {
        //start animation of sign of no possible position to put
        gameState.detail.allyState = ALLYSTATE_NONEPUT;
      }
      return;
    case ALLYSTATE_WAITING:
      //check mouse click
      WaitingClick();
      return;
    case ALLYSTATE_NONEPUT:
      //nothing to do
      return;
  }
}

void GameControlWithAllyRevState() {
  switch(gameState.detail.allyRev) {
    case ALLYREV_READY:
      //StartAnimation
      DeleteSelectedTypeObject(OBJECT_SELECTABLE_POINT);
      gameState.detail.allyRev = ALLYREV_ANIMATION;
      return;
    case ALLYREV_ANIMATION:
      //check all stones reversed
      CheckAllStoneReversed(GAMESTATE_REVERSE_ALLY);
      return;
  }
}

void GameControlWithEnemyState() {
  switch(gameState.detail.EneState) {
    case ENESTATE_SYNC:
      break;
    case ENESTATE_SHOW_ENETURN:
      break;
    case ENESTATE_WATING:
      break;
    case ENESTATE_NONEPUT:
      break;
  }
}

void GameControlWithEnemyRevState() {
  switch(gameState.detail.EneRev) {
    case ENEREV_READY:
      break;
    case ENEREV_ANIMATION:
      break;
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
}

void AnimationFinised(GameBroadState broad, GameDetailState detail) {
  gameState.broad = broad;
  gameState.detail = detail;
}

//Check reverse animation ended
void CheckAllStoneReversed(GameBroadState broad) {
  if(CheckAllReverseAnimationEnded()) {
    switch(broad) {
      case GAMESTATE_REVERSE_ALLY:
        gameState.broad = GAMESTATE_ENEMY;
        gameState.detail.EneState = ENESTATE_SYNC;
        return;
      case GAMESTATE_REVERSE_ENEMY:
        gameState.broad = GAMESTATE_ALLY;
        gameState.detail.allyState = ALLYSTATE_SYNC;
        return;
      default:
        printf("(%s)Error line:%d\n", __FILE__, __LINE__);
        exit(1);
    }
  }
}

void WaitingClick() {
  int x, y;

  if(GetMouseClickPos(&x, &y)) {
    int xx, yy;

    if(MousePositionToSquarePosition(x, y, &xx, &yy)) {
      PutStone(xx, yy, );
    }
  }
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
