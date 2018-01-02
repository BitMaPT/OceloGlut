#include<stdio.h>
#include<stdlib.h>
#include"ocelostone.h"
#include"object.h"

int DrawObject(Object *obj);
int UpdateObject(Object *obj);

static ObjectList list;

int ObjectListInit() {
  list.list = (Object**)malloc(sizeof(Object*) * 10);
  if(list.list == NULL) return 0;

  list.length = 0;
  list.max = 10;

  return 1;
}

int UpdateAllObject() {
  int i;

  for(i = 0; i < list.length; i++) {
    UpdateObject(list.list[i]);
  }

  return 1;
}

int DrawAllObject() {
  int i;

  for(i = 0; i < list.length; i++) {
    DrawObject(list.list[i]);
  }

  return 1;
}

int DrawObject(Object *obj) {
  switch(obj->type) {
    case OBJECT_OCELO_STONE:
      DrawStone(obj->object.stone);
      break;
    case OBJECT_SELECTABLE_POINT:
      DrawPoint(obj->object.point);
      break;
    default:
      printf("(%s)Error line:%d\n", __FILE__, __LINE__);
      exit(1);
  }

  return 1;
}

int UpdateObject(Object *obj) {
  switch(obj->type) {
    case OBJECT_OCELO_STONE:
      UpdateStone(obj->object.stone);
      break;
    case OBJECT_SELECTABLE_POINT:
      UpdatePoint(obj->object.point);
      break;
    default:
      printf("(%s)Error line:%d\n", __FILE__, __LINE__);
      exit(1);
  }

  return 1;
}

int AddObject(Object *obj) {
  if(list.length == list.max) {
    list.max *= 2;
    list.list = (Object**)malloc(sizeof(Object*) * list.max);
    if(list.list == NULL) return 0;
  }

  list.list[list.length] = obj;
  list.length++;

  return 1;
}

int DeleteAllObject() {
  int i;

  for(i = 0; i < list.length; i++) {
    free(list.list[i]);
  }
  free(list.list);

  return ObjectListInit();
}

int DeleteSelectedTypeObject(ObjectType type) {
  int i, j;
  int *deleteList;
  ObjectList oldList;
  ObjectList newList;

  newList.list = (Object**)malloc(sizeof(Object*) * oldList.max);
  newList.max = oldList.max;
  if(newList.list == NULL) return 0;

  oldList = list;

  for(i = 0; i < list.length; i++) {
    if(oldList.list[i]->type == type) {
      switch(type) {
        case OBJECT_OCELO_STONE:
          free(oldList.list[i]->object.stone);
          break;
        case OBJECT_SELECTABLE_POINT:
          free(oldList.list[i]->object.point);
          break;
      }
      continue;
    }

    newList.list[j++] = oldList.list[i];
  }

  newList.length = oldList.length - j;
  free(oldList.list);
  list = newList;
  
  return 1;
}