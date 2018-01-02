#ifndef HEADER_OBJECT
#define HEADER_OBJECT

#include"ocelostone.h"
#include"putpoint.h"

typedef enum ObjectType {
  OBJECT_OCELO_STONE,
  OBJECT_SELECTABLE_POINT
}ObjectType;

typedef union ObjectBody {
  Stone *stone;
  OceloPoint *point;
}ObjectBody;

typedef struct Object {
  ObjectType type;
  ObjectBody object;
}Object;

typedef struct ObjectList {
  Object **list;
  int length;
  int max;
}ObjectList;

int DrawAllObject();
int UpdateAllObject();
int ObjectListInit();
int DeleteAllObject();
int AddObject(Object *obj);

#endif