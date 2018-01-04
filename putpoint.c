#include<GL/glut.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"putpoint.h"
#include"object.h"

int PointStateContena(OceloPoint *point);

void UpdatePoint(OceloPoint *point) {
  PointStateContena(point);
}

OceloPoint *InitOceloPoint(int x, int y) {
  OceloPoint *point;
  OceloPointState state = {0, 0, 500};
  Object *obj;

  point = (OceloPoint*)malloc(sizeof(OceloPoint));
  if(point == NULL) return NULL;
  obj = (Object*)malloc(sizeof(Object));
  if(obj == NULL) return NULL;

  point->pos[0] = x;
  point->pos[1] = y;
  point->alpha = 1;
  point->state = state;

  obj->object.point = point;
  obj->type = OBJECT_SELECTABLE_POINT;

  AddObject(obj);

  return point;
}

int PointStateContena(OceloPoint *point) {
  if(point->state.startTime == 0) point->state.startTime = glutGet(GLUT_ELAPSED_TIME);

  point->state.stateTime = glutGet(GLUT_ELAPSED_TIME) - point->state.startTime;
  point->alpha = 0.5 * sin(2 * M_PI * (double)(point->state.stateTime % point->state.shiftTime) / (double)point->state.shiftTime);

  return 1;
}

void DrawPoint(OceloPoint *point) {
  int xx, yy;

  xx = 40 + point->pos[0] * 70 + 35;
  yy = 40 + point->pos[1] * 70 + 35;

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glTranslated(xx, yy, 0);
  glColor4d(1, 1, 0, point->alpha);
  glBegin(GL_QUADS);
  glVertex2d(-20, -20);
  glVertex2d(-20,  20);
  glVertex2d( 20,  20);
  glVertex2d( 20, -20);
  glEnd();

  glDisable(GL_BLEND);
}