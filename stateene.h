#ifndef HEADER_STATEENE
#define HEADER_STATEENE

typedef enum EnemyState {
  ENESTATE_SYNC,
  ENESTATE_WATING,
  ENESTATE_NONEPUT
}EnemyState;

void GameControlWithEnemyState();

#endif