#include<GL/glut.h>
#include<stdio.h>
#include<stdlib.h>
#include"title.h"
#include"object.h"
#include"mouse.h"

void WaitForFadeout();
void WaitForStartButtonClick();

TitleState titleState;

void InitTitleScene() {
  DeleteAllObject();

  //add button to object list

  titleState = TITLE_STATE_STABLE;
}

void TitleControlWithState() {
  switch(titleState) {
    case TITLE_STATE_STABLE:
      WaitForStartButtonClick();
      return;
    case TITLE_STATE_FADEOUT:
      WaitForFadeout();
      return;
  }
}

void WaitForFadeout() {
  if(/*fadeout animation is end*/) {
    //change scene to waiting matching
  }
}

void WaitForStartButtonClick() {
  int x, y;

  if(GetMouseDown(GLUT_LEFT_BUTTON, &x, &y)) {
    if(/*click button*/) {
      //Start fadeout;
    }
  }
}