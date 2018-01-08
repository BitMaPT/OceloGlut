#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include"gamecontroller.h"
#include"stateene.h"
#include"oceloboard.h"
#include"syncgame.h"

static void RecvSyncInfo();
static void RecvReadyInfo();
void RecvEnemyCanPut();
void RecvEnemyPut();

void GameControlWithEnemyState() {
  switch(gameState.detail.EneState) {
    case ENESTATE_SYNC:
      RecvSyncInfo();
      break;
    case ENESTATE_WAITCHECK:
      RecvReadyInfo();
      break;
    case ENESTATE_WATING:
      //recieve stone position of enemy puting
      RecvEnemyPut();
      break;
    case ENESTATE_REVERSE:
      if(CheckAllStoneReversed()) {
        gameState.broad = GAMESTATE_ALLY;
        gameState.detail.allyState = ALLYSTATE_CHECKING_PUT;
        if(SendSignalToServer(SYNC_READY_NEXTTURN) == 0) {
          exit(1);
        }
      }
      break;
    case ENESTATE_NONEPUT:
      if(1/*end animation*/) {
        gameState.broad = GAMESTATE_ALLY;
        gameState.detail.allyState = ALLYSTATE_CHECKING_PUT;
        if(SendSignalToServer(SYNC_READY_NEXTTURN) == 0) {
          exit(1);
        }
      }
      break;
  }
}

void RecvEnemyPut() {
  int x, y, retval;
  //get info of enemy can put stone

  retval = RecvPutPoint(&x, &y);

  if(retval == -1) {
    //error occured
    exit(1);
  } else if(retval) {
    PutStone(x, y, enemyStoneColor);
    gameState.detail.EneState = ENESTATE_REVERSE;
  }
}

static void RecvSyncInfo() {
  int header;

  header = RecvSignalFromServer();
  if(header == 0) return;
  if(header == -1) {
    exit(1);
  }
  switch((SyncHeader)header) {
    case SYNC_READY_NEXTTURN:
      gameState.detail.EneState = ENESTATE_WAITCHECK;
      return;
    default:
      fprintf(stderr, "invalid header\n");
      exit(1);
  }
}

static void RecvReadyInfo() {
  int retval;

  retval = RecvSignalFromServer();
  if(retval == 0) return;
  if(retval == -1) exit(1);
  switch((SyncHeader)retval) {
    case SYNC_WAITPUT:
      gameState.detail.EneState = ENESTATE_WATING;
      return;
    case SYNC_NO_POSITION:
      gameState.detail.EneState = ENESTATE_NONEPUT;
      return;
    case SYNC_ENDGAME:
      gameState.broad;//end game;
      gameState.detail; 
      return;
    default:
      fprintf(stderr, "%s line:%d fatal error\n", __FILE__, __LINE__);
      exit(1);
  }
}