#include<stdlib.h>
#include"stateallyrev.h"
#include"gamecontroller.h"
#include"object.h"
#include"oceloboard.h"

void GameControlWithAllyRevState() {
  switch(gameState.detail.allyRev) {
    case ALLYREV_READY:
      //StartAnimation
      
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