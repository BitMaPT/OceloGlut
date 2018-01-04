#include<stdlib.h>
#include"gamecontroller.h"
#include"stateene.h"
#include"oceloboard.h"

int RecvEnemyCanPut();
void RecvEnemyPut();

void GameControlWithEnemyState() {
  switch(gameState.detail.EneState) {
    case ENESTATE_SYNC:
      //sync
      break;
    case ENESTATE_WATING:
      //recieve stone position of enemy puting
      RecvEnemyCanPut();
      break;
    case ENESTATE_NONEPUT:
      //check animation ended
      break;
  }
}

int RecvEnemyCanPut() {
  //get info of enemy can put stone

  return 0;
}

void RecvEnemyPut() {
  int x, y;
  OceloStoneColor enemy;

  //recv enemy put pos from server

  switch(myStoneColor) {
    case STONE_COLOR_BLACK:
      enemy = STONE_COLOR_WHITE;
      break;
    case STONE_COLOR_WHITE:
      enemy = STONE_COLOR_BLACK;
      break;
  }

  PutStone(x, y, enemy);
}