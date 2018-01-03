#include<GL/glut.h>
#include"mouse.h"

static int clicked;

typedef struct MouseButton {
  int state;
  int x;
  int y;
}MouseButton;

static MouseButton rightDown;
static MouseButton rightUp;

static MouseButton middleDown;
static MouseButton middleUp;

static MouseButton leftDown;
static MouseButton leftUp;

void Mouse(int button, int state, int x, int y) {
  MouseButton temp = {1, x, y};

  switch(button) {
    case GLUT_LEFT_BUTTON:
      switch(state) {
        case GLUT_DOWN:
          leftDown = temp;
          return;
        case GLUT_UP:
          leftUp = temp;
          return;
      }
      return;
    case GLUT_MIDDLE_BUTTON:
      switch(state) {
        case GLUT_DOWN:
          middleDown = temp;
          return;
        case GLUT_UP:
          middleUp = temp;
          return;
      }
      return;
    case GLUT_RIGHT_BUTTON:
      switch(state) {
        case GLUT_DOWN:
          rightDown = temp;
          return;
        case GLUT_UP:
          rightUp = temp;
          return;
      }
      return;
  }
}

//reset mouse status per frame
void ResetMouseState() {
  MouseButton reset = {0, 0, 0};

  leftDown = reset;
  leftUp = reset;
  middleDown = reset;
  middleUp = reset;
  rightDown = reset;
  rightUp = reset;
}

int GetMouseDown(int button, int *x, int *y) {
  switch(button) {
    case GLUT_LEFT_BUTTON:
      *x = leftDown.x;
      *y = leftDown.y;
      return leftDown.state;
    case GLUT_MIDDLE_BUTTON:
      *x = middleDown.x;
      *y = middleDown.y;
      return middleDown.state;
    case GLUT_RIGHT_BUTTON:
      *x = rightDown.x;
      *y = rightDown.y;
      return rightDown.state;
    default:
      return 0;
  }
}

int GetMouseUp(int button, int *x, int *y) {
  switch(button) {
    case GLUT_LEFT_BUTTON:
      *x = leftUp.x;
      *y = leftUp.y;
      return leftUp.state;
    case GLUT_MIDDLE_BUTTON:
      *x = middleUp.x;
      *y = middleUp.y;
      return middleUp.state;
    case GLUT_RIGHT_BUTTON:
      *x = rightUp.x;
      *y = rightUp.y;
      return rightUp.state;
    default:
      return 0;
  }
}