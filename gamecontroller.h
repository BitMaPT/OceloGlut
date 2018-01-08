#ifndef HEADER_GAMECONTROLLER
#define HEADER_GAMECONTROLLER

#include"ocelostone.h"
#include"stateally.h"
#include"stateallyrev.h"
#include"stateene.h"
#include"stateenerev.h"

typedef enum GameBroadState {
  GAMESTATE_ALLY,
  GAMESTATE_ENEMY,
  GAMESTATE_REVERSE_ALLY,
  GAMESTATE_REVERSE_ENEMY
}GameBroadState;

typedef union GameDetailState {
  AllyState allyState;
  AllyReverse allyRev;
  EnemyState EneState;
  EnemyReverse EneRev;
}GameDetailState;

typedef struct GameState {
  GameBroadState broad;
  GameDetailState detail;
}GameState;

void InitGame();
void ControlGameWithState();
void WaitPutStone();
void StartTurn();
void FinishStringAnim();

extern GameState gameState;
extern OceloStoneColor myStoneColor;
extern OceloStoneColor enemyStoneColor;

#endif