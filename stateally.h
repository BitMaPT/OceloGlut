#ifndef HEADER_STATEALLY
#define HEADER_STATEALLY

typedef enum AllyState {
  ALLYSTATE_SYNC,
  ALLYSTATE_CHECKING_PUT,
  ALLYSTATE_SHOW_YOURTURN,
  ALLYSTATE_WAITING,
  ALLYSTATE_NONEPUT,
  ALLYSTATE_REVERSE
}AllyState;

void GameControlWithAllyState();

#endif