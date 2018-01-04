#include<stdio.h>
#include<stdlib.h>
#include"object.h"
#include"oceloboard.h"
#include"ocelostone.h"
#include"gamecontroller.h"
#include"mouse.h"

void WaitingClick();
int MousePositionToSquarePosition(int x, int y, int *xx, int *yy);
void ControlGameWithAllyState();
void GameControlWithAllyRevState();
void GameControlWithEnemyState();
void GameControlWithEnemyRevState();

int RecvEnemyCanPut();
void RecvEnemyPut();

static OceloStoneColor myStoneColor;

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
      GameControlWithEnemyState();
      return;
    case GAMESTATE_REVERSE_ENEMY:
      GameControlWithEnemyRevState();
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
      if(CheckAllStoneReversed()) {
        gameState.broad = GAMESTATE_ENEMY;
        gameState.detail.EneState = ENESTATE_SYNC;
      }
      return;
  }
}

void GameControlWithEnemyState() {
  switch(gameState.detail.EneState) {
    case ENESTATE_SYNC:
      //sync
      break;
    case ENESTATE_SHOW_ENETURN:
      if(/*check animation is ended*/) {
        if(RecvEnemyCanPut()) {
          gameState.detail.EneState = ENESTATE_WATING;
        } else {
          gameState.detail.EneState = ENESTATE_NONEPUT;
        }
      }
      break;
    case ENESTATE_WATING:
      //recieve stone position of enemy puting
      RecvEnemyCanPut();
      break;
    case ENESTATE_NONEPUT:
      //check animation ended
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

void WaitingClick() {
  int x, y;

  if(GetMouseDown(GLUT_LEFT_BUTTON, &x, &y)) {
    int xx, yy;

    if(MousePositionToSquarePosition(x, y, &xx, &yy)) {
      if(oceloCanPut[y][x]){
        PutStone(xx, yy, myStoneColor);
        gameState.broad = GAMESTATE_REVERSE_ALLY;
        gameState.detail.allyRev = ALLYREV_READY;
      }
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

int RecvEnemyCanPut() {
  //get info of enemy can put stone

  return 0;
}

void RecvEnemyPut() {
  int x, y;
  OceloStoneColor enemy;

  //recv enemy put pos from server

  switch(myStoneColor) {
    case STONE_COLOR_BLACK:
      enemy = STONE_COLOR_WHITE;
      break;
    case STONE_COLOR_WHITE:
      enemy = STONE_COLOR_BLACK;
      break;
  }

  PutStone(x, y, enemy);
}
