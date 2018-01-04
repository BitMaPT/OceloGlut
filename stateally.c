#include<stdio.h>
#include<stdlib.h>
#include"gamecontroller.h"
#include"stateally.h"
#include"mouse.h"
#include"string3d.h"
#include"oceloboard.h"
#include"object.h"

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
        if(InitString(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2, "NONE to PUT") == NULL) exit(1);
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
    case ALLYSTATE_REVERSE:
      if(CheckAllStoneReversed()) {
        gameState.broad = GAMESTATE_ENEMY;
        gameState.detail.EneState = ENESTATE_SYNC;
      }
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
        DeleteSelectedTypeObject(OBJECT_SELECTABLE_POINT);
      
        gameState.detail.allyState = ALLYSTATE_REVERSE;
      }
    }
  }
}