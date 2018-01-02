#ifndef HEADER_GAMECONTROLLER
#define HEADER_GAMECONTROLLER

#include"ocelostone.h"

typedef enum GameBroadState {
  GAMESTATE_ALLY,
  GAMESTATE_ENEMY,
  GAMESTATE_REVERSE_ALLY,
  GAMESTATE_REVERSE_ENEMY
}GameBroadState;

typedef enum AllyState {
  ALLYSTATE_SYNC,
  ALLYSTATE_SHOW_YOURTURN,
  ALLYSTATE_CHECKING_PUT,
  ALLYSTATE_WAITING,
  ALLYSTATE_NONEPUT
}AllyState;

typedef enum AllyReverse {
  ALLYREV_READY,
  ALLYREV_ANIMATION
}AllyReverse;

typedef enum EnemyState {
  ENESTATE_SYNC,
  ENESTATE_SHOW_ENETURN,
  ENESTATE_WATING,
  ENESTATE_NONEPUT
}EnemyState;

typedef enum EnemyReverse {
  ENEREV_READY,
  ENEREV_ANIMATION
}EnemyReverse;

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
void WaitPutStone();
void StartTurn();
void ClickBoard();
void RenewStatus();
void ExchangeTurn();

void ControlGameWithAllyState();
void GameControlWithAllyRevState();


#endif