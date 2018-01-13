#ifndef HEADER_OBJECT
#define HEADER_OBJECT

#include"ocelostone.h"
#include"putpoint.h"
#include"string3d.h"
#include"bitstring.h"

typedef enum ObjectType {
  OBJECT_OCELO_STONE,
  OBJECT_SELECTABLE_POINT,
  OBJECT_BITSTRING
}ObjectType;

typedef union ObjectBody {
  Stone *stone;
  OceloPoint *point;
  BitString *string;
}ObjectBody;

typedef struct Object {
  ObjectType type;
  ObjectBody object;
}Object;

typedef struct ObjectList ObjectList;

typedef struct ObjectList {
  Object *obj;
  ObjectList *next;
}ObjectList;

int DrawAllObject();
int UpdateAllObject();
int ObjectListInit();
int DeleteAllObject();
int DeleteObject(Object *obj);
int AddObject(Object *obj);
int DeleteSelectedTypeObject(ObjectType type);
int DeleteString(String3D *str);

#endif