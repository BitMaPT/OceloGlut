#ifndef HEADER_OBJECT
#define HEADER_OBJECT

#include"ocelostone.h"
#include"putpoint.h"
#include"bitstring.h"
#include"pngimage.h"

typedef enum ObjectType {
  OBJECT_OCELO_STONE,
  OBJECT_SELECTABLE_POINT,
  OBJECT_BITSTRING,
  OBJECT_PNGIMAGE
}ObjectType;

typedef union ObjectBody {
  Stone *stone;
  OceloPoint *point;
  BitString *string;
  Image *image;
}ObjectBody;

typedef struct Object Object;
struct Object {
  ObjectType type;
  ObjectBody object;
  void(*Update)(Object*);
  void(*Draw)(Object*);
  void(*Delete)(Object*);
};

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

#endif