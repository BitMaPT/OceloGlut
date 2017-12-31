#ifndef HEADER_OBJECT
#define HEADER_OBJECT

#include"ocelostone.h"

typedef enum ObjectType {
  OBJECT_OCELO_STONE,
  OBJECT_SELECTABLE_PANEL
}ObjectType;

typedef union ObjectBody {
  Stone *stone;

}ObjectBody;

typedef struct Object {
  ObjectType type;
  ObjectBody object;
}Object;

typedef struct ObjectList {
  Object *list;
  int lenght;
  int max;
}ObjectList;

int DrawObject(Object *obj);

#endif