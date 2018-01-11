#ifndef HEADER_GAMECONTROLLER
#define HEADER_GAMECONTROLLER

#include"ocelostone.h"
#include"stateally.h"
#include"stateallyrev.h"
#include"stateene.h"
#include"stateenerev.h"

typedef enum GameState {
  GAMESTATE_INIT = 1,
  GAMESTATE_GET_PUTABLE,
  GAMESTATE_SEND_SIGNAL,
  GAMESTATE_WAIT_MYPUT,
  GAMESTATE_SEND_POSITION,
  GAMESTATE_WAIT_OPPUT,
  GAMESTATE_REVERSE
}GameState;

int ControlGameWithState();
int SetSocket(char **argv);

extern GameState gameState;
extern OceloStoneColor myStoneColor;
extern OceloStoneColor enemyStoneColor;
extern int isFirstPlayer;

#endif