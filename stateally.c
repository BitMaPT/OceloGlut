#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include"gamecontroller.h"
#include"stateally.h"
#include"mouse.h"
#include"string3d.h"
#include"oceloboard.h"
#include"object.h"
#include"syncgame.h"
#include"syncheader.h"

static void RecvSyncInfo();
void CheckPutPosition();
void WaitingClick();

void GameControlWithAllyState() {
  switch(gameState.detail.allyState) {
    case ALLYSTATE_SYNC:
      RecvSyncInfo();
      return;
    case ALLYSTATE_CHECKING_PUT:
      CheckPutPosition();
      return;
    case ALLYSTATE_SHOW_YOURTURN:
      if(1/*check animation end*/) {
        gameState.detail.allyState = ALLYSTATE_WAITING;
      }
      return;
    case ALLYSTATE_WAITING:
      //check mouse click
      WaitingClick();
      return;
    case ALLYSTATE_NONEPUT:
      if(1/*end animation*/) {
        gameState.broad = GAMESTATE_ENEMY;
        gameState.detail.EneState = ENESTATE_SYNC;
        SendSignalToServer(SYNC_READY_NEXTTURN);
      }
      return;
    case ALLYSTATE_REVERSE:
      if(CheckAllStoneReversed()) {
        gameState.broad = GAMESTATE_ENEMY;
        gameState.detail.EneState = ENESTATE_SYNC;
        SendSignalToServer(SYNC_READY_NEXTTURN);
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

        if(SendPutPoint(xx, yy) == 0) {
          //error occured
          //end game
          exit(1);
        }
      
        gameState.detail.allyState = ALLYSTATE_REVERSE;
      }
    }
  }
}

void CheckPutPosition() {
  //checking puttable square
  //put puttable mark or show signal of no puttable square
  if(CheckAllSquaresToPut(myStoneColor)) {
    GenerateSelectablePutPoint();
    gameState.detail.allyState = ALLYSTATE_SHOW_YOURTURN;
  } else {
    if(CheckAllSquaresToPut(enemyStoneColor)) {
      //start animation of sign of no possible position to put
      if(InitString(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2, "NONE to PUT") == NULL) {
        exit(1);
      }
      if(SendNoPositionToPut() == 0) {
        //error occured
      }
      gameState.detail.allyState = ALLYSTATE_NONEPUT;
    } else {
      if(SendGameEnd() == 0) {
        //error occured in SendGameEnd
        
      }
      //state change
    }
  }
}

static void RecvSyncInfo() {
  int header;

  header = RecvSignalFromServer();
  if(header == 0) return;
  if(header < -1) exit(1);

  switch((SyncHeader)header) {
    case SYNC_READY_NEXTTURN:
      gameState.detail.allyState = ALLYSTATE_CHECKING_PUT;
      return;
    default:
      fprintf(stderr, "%s line:%d invalid header\n", __FILE__, __LINE__);
      exit(1);
  }
}