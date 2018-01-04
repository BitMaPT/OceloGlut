#ifndef HEADER_STRING3D
#define HEADER_STRING3D

#include<GL/glut.h>

typedef struct StringAnimState {
  int startTime;
  int stateTime;
  int endTime;
}StringAnimState;

typedef struct String3D {
  char *str;
  GLdouble angle;
  int pos[2];
  void* font;
  StringAnimState anim;
}String3D;

void DrawString(String3D *str);
String3D* InitString(int x, int y, char *str);

#endif