#include<stdio.h>
#include<stdlib.h>

typedef enum ObjectType {
  OBJECT_OCELO_STONE,
  OBJECT_SELECTABLE_PANEL
}ObjectType;

typedef union ObjectBody {

}ObjectBody;

typedef struct Object {
  ObjectType type;
  ObjectBody object;
}Object;

int DrawObject(Object obj) {
  switch(obj.type) {
    case OBJECT_OCELO_STONE:
      //draw stone
      break;
    case OBJECT_SELECTABLE_PANEL:
      //draw selectable panel
      break;
    default:
      printf("(%s)Error line:%d\n", __FILE__, __LINE__);
      exit(1);
  }

  return 1;
}