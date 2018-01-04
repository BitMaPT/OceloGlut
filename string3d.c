#include<GL/glut.h>
#include<string.h>
#include"gamecontroller.h"
#include"string3d.h"
#include"object.h"

void StringAnim(String3D *str);

void UpdateString(String3D *str) {
  StringAnim(str);
}

void DrawString(String3D *str) {
  int i;

  glPushAttrib(GL_LINE_BIT);

  glTranslated((GLdouble)str->pos[0], (GLdouble)str->pos[1], 0);
  glRotated(str->angle, 0, 1.0, 0);

  for(i = 0; str->str[i]; i++) {
    glutStrokeCharacter(str->font, str->str[i]);
  }

  glPopAttrib();
}

String3D* InitString(int x, int y, char *str) {
  String3D *str3d;
  StringAnimState anim = {0, 0, 3000};
  Object *obj;
  char *dstStr;

  str3d = (String3D*)malloc(sizeof(String3D));
  if(str3d == NULL) return NULL;
  obj = (Object*)malloc(sizeof(Object));
  if(obj == NULL) return NULL;
  dstStr = (char*)malloc(sizeof(char) * (strlen(str) + 1));
  if(dstStr == NULL) return NULL;

  strcpy(dstStr, str);

  str3d->pos[0] = x;
  str3d->pos[1] = y;
  str3d->angle = 0;
  str3d->str = dstStr;
  str3d->anim = anim;


  obj->object.string = str3d;
  obj->type = OBJECT_STRING;
  AddObject(obj);

  return str3d;
}

void StringAnim(String3D *str) {
  if(str->anim.startTime == 0) str->anim.startTime = glutGet(GLUT_ELAPSED_TIME);

  str->anim.stateTime = glutGet(GLUT_ELAPSED_TIME) - str->anim.startTime;

  if(str->anim.stateTime >= 0 && str->anim.stateTime < 1000) {
    str->angle = (90.0 / 1000.0) * str->anim.stateTime - 90;
  } else if (str->anim.stateTime >= 1000 && str->anim.stateTime < 2000) {
    str->angle = 0;
  } else {
    str->angle = -(90.0 / 1000.0) * str->anim.startTime + 180;
  }

  if(str->anim.stateTime >= 3000) {
    DeleteString(str);
    FinishStringAnim();
  }
}