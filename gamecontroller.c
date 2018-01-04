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
GameState gameState;
ObjectList objectList;
OceloStoneColor playerColor;

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
