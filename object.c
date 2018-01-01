#include<stdio.h>
#include<stdlib.h>
#include"ocelostone.h"
#include"object.h"

int DrawObject(Object *obj);

static ObjectList list;

int ObjectListInit() {
  list.list = (Object**)malloc(sizeof(Object*) * 10);
  if(list.list == NULL) return 0;

  list.lenght = 0;
  list.max = 10;

  return 1;
}

int DrawAllObject() {
  int i;

  for(i = 0; i < list.lenght; i++) {
    DrawObject(list.list[i]);
  }
}

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

int AddObject(Object *obj) {
  if(list.lenght == list.max) {
    list.max *= 2;
    list.list = (Object**)malloc(sizeof(Object*) * list.max);
    if(list.list == NULL) return 0;
  }

  list.list[list.lenght] = obj;
  list.lenght++;

  return 1;
}

int DeleteAllObject() {
  int i;

  for(i = 0; i < list.lenght; i++) {
    free(list.list[i]);
  }
  free(list.list);

  return ObjectListInit();
}