#include<stdio.h>
#include<stdlib.h>
#include"ocelostone.h"
#include"oceloboard.h"
#include"object.h"
#include"mouse.h"

static int oceloResult[2];

int SetResult() {
  int x, y;
  int black, white;

  black = 0;
  white = 0;
  
  for(x = 0; x < OCELO_WIDTH; x++) {
    for(y = 0; y < OCELO_HEIGHT; y++) {
      if(oceloBoard[y][x] == NULL) continue;
      switch(oceloBoard[y][x]->stone) {
        case STONE_COLOR_BLACK:
          black++;
          break;
        case STONE_COLOR_WHITE:
          white++;
          break;
      }
    }
  }

  switch(myStoneColor) {
    case STONE_COLOR_BLACK:
      oceloResult[0] = black;
      oceloResult[1] = white;
      break;
    case STONE_COLOR_WHITE:
      oceloResult[0] = white;
      oceloResult[1] = black;
      break;
  }

  DeleteSelectedTypeObject(OBJECT_OCELO_STONE);

  return 1;
}

int ResultStonePut() {
  static int startTime = 0;
  static int mycount = 0, opcount = 0;
  int shiftTime = 100;

  if(startTime == 0) {
    startTime = glutGet(GLUT_ELAPSED_TIME);
  }

  if(oceloResult[0] == 0 && oceloResult[1] == 0) {
    int nextStateShiftTime = 1000;
    if(nextStateShiftTime < glutGet(GLUT_ELAPSED_TIME) - startTime) {
      int pos[] = {WIDTH / 2, HEIGHT / 2};
      int size[] = {255, 255};

      if(mycount > opcount) {
        if(InitImage("Stunt.png", pos, size, ImageAnimZoomIn) == NULL) return 0;
      } else if(mycount == opcount) {
        if(InitImage("Stunt.png", pos, size, ImageAnimZoomIn) == NULL) return 0;
      } else {
        if(InitImage("Stunt.png", pos, size, ImageAnimZoomIn) == NULL) return 0;
      }
      gameState = GAMESTATE_RESULT_SHOWRESULT;
    }
    return 1;
  }

  if(shiftTime < glutGet(GLUT_ELAPSED_TIME) - startTime) {
    startTime = glutGet(GLUT_ELAPSED_TIME);

    if(oceloResult[0] > 0) {
      int x, y;

      oceloResult[0]--;
      mycount++;

      x = OCELO_WIDTH - ((mycount - 1) % OCELO_WIDTH) - 1;
      y = OCELO_HEIGHT - ((mycount - 1) / OCELO_WIDTH) - 1;
      InitOceloStone(x, y, myStoneColor);
    }

    if(oceloResult[1] > 0) {
      int x, y;

      oceloResult[1]--;
      opcount++;

      x = (opcount - 1) % OCELO_WIDTH;
      y = (opcount - 1) / OCELO_WIDTH;
      InitOceloStone(x, y, enemyStoneColor);
    }
  }

  return 1;
}

int WaitClickToExit() {
  int dummy;
  if(GetMouseDown(GLUT_LEFT_BUTTON, &dummy, &dummy)) {
    exit(0);
  }
  return 1;
}