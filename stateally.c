#include<stdio.h>
#include<stdlib.h>
#include"gamecontroller.h"
#include"stateally.h"
#include"mouse.h"
#include"string3d.h"
#include"oceloboard.h"

void WaitingClick();

void GameControlWithAllyState() {
  switch(gameState.detail.allyState) {
    case ALLYSTATE_SYNC:
      //sync
      return;
    case ALLYSTATE_CHECKING_PUT:
      //checking puttable square
      //put puttable mark or show signal of no puttable square
      if(CheckAllSquaresToPut(myStoneColor)) {
        GenerateSelectablePutPoint();
        gameState.detail.allyState = ALLYSTATE_WAITING;
      } else {
        //start animation of sign of no possible position to put
        if(InitString(640 / 2, 480 / 2, "NONE to PUT") == NULL) exit(1);
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

void WaitingClick() {
  int x, y;

  if(GetMouseDown(GLUT_LEFT_BUTTON, &x, &y)) {
    int xx, yy;

    if(MousePositionToSquarePosition(x, y, &xx, &yy)) {
      if(oceloCanPut[yy][xx]){
        PutStone(xx, yy, myStoneColor);
        gameState.broad = GAMESTATE_REVERSE_ALLY;
        gameState.detail.allyRev = ALLYREV_READY;
      }
    }
  }
}