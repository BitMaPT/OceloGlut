#ifndef HEADER_STATEALLY
#define HEADER_STATEALLY

typedef enum AllyState {
  ALLYSTATE_SYNC,
  ALLYSTATE_CHECKING_PUT,
  ALLYSTATE_WAITING,
  ALLYSTATE_NONEPUT
}AllyState;

void GameControlWithAllyState();

#endif