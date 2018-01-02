#include"mouse.h"

static int clicked;
static int x;
static int y;

void MouseClicked(int xx, int yy) {
  clicked = 1;

  x = xx;
  y = yy;
}

int GetMouseClickPos(int *xx, int *yy) {
  if(clicked == 0) return 0;

  *xx = x;
  *yy = y;

  return 1;
}

void UpdateMouse() {
  clicked = 0;
}