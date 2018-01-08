#include<stdio.h>
#include<stdlib.h>
#include"object.h"
#include"oceloboard.h"
#include"ocelostone.h"
#include"gamecontroller.h"
#include"mouse.h"
#include"stateally.h"

void WaitingClick();
void GameControlWithAllyRevState();
void GameControlWithEnemyState();
void GameControlWithEnemyRevState();

int RecvEnemyCanPut();
void RecvEnemyPut();

OceloStoneColor myStoneColor;
OceloStoneColor enemyStoneColor;
GameState gameState;
ObjectList objectList;

void ControlGameWithState() {
  switch(gameState.broad) {
    case GAMESTATE_ALLY:
      GameControlWithAllyState();
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

void InitGame(OceloStoneColor stoneColor) {
  
  int x, y;

  DeleteAllObject();

  for(y = 0; y < OCELO_HEIGHT; y++) {
    for(x = 0; x < OCELO_WIDTH; x++) {
      oceloBoard[y][x] = NULL;
    }
  }

  oceloBoard[3][3] = InitOceloStone(3, 3, STONE_COLOR_BLACK);
  oceloBoard[3][4] = InitOceloStone(4, 3, STONE_COLOR_WHITE);
  oceloBoard[4][3] = InitOceloStone(3, 4, STONE_COLOR_WHITE);
  oceloBoard[4][4] = InitOceloStone(4, 4, STONE_COLOR_BLACK);

  myStoneColor = stoneColor;
  switch(myStoneColor) {
    case STONE_COLOR_BLACK:
      enemyStoneColor = STONE_COLOR_WHITE;
      break;
    case STONE_COLOR_WHITE:
      enemyStoneColor = STONE_COLOR_BLACK;
      break;
  }

  gameState.broad = GAMESTATE_ALLY;
  gameState.detail.allyState = ALLYSTATE_CHECKING_PUT;
}

void AnimationFinised(GameBroadState broad, GameDetailState detail) {
  gameState.broad = broad;
  gameState.detail = detail;
}

void FinishStringAnim() {
  switch(gameState.broad) {
    case GAMESTATE_ALLY:
      gameState.broad = GAMESTATE_ENEMY;
      gameState.detail.EneState = ENESTATE_SYNC;
      break;
    case GAMESTATE_ENEMY:
      gameState.broad = GAMESTATE_ALLY;
      gameState.detail.allyState = ALLYSTATE_SYNC;
      break;
    default:
      break;
  }
}