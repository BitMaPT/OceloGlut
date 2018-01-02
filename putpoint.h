#ifndef HEADER_PUTPOINT
#define HEADER_PUTPOINT

#include<GL/glut.h>

typedef struct OceloPointState {
  int startTime;
  int stateTime;
  int shiftTime;
}OceloPointState;

typedef struct OceloPoint {
  GLdouble alpha;
  OceloPointState state;
  int pos[2];
}OceloPoint;

void UpdatePoint(OceloPoint *point);
OceloPoint *InitOceloPoint(int x, int y);
void DrawPoint(OceloPoint *point);

#endif