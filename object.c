#include<stdio.h>
#include<stdlib.h>
#include"ocelostone.h"
#include"object.h"

int DrawObject(Object *obj) {
  switch(obj->type) {
    case OBJECT_OCELO_STONE:
      DrawStone(obj->object.stone);
      break;
    case OBJECT_SELECTABLE_PANEL:
      //draw selectable panel
      printf("(%s)Not Implemented line:%d\n", __FILE__, __LINE__);
      exit(1);
      break;
    default:
      printf("(%s)Error line:%d\n", __FILE__, __LINE__);
      exit(1);
  }

  return 1;
}