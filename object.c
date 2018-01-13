#include<stdio.h>
#include<stdlib.h>
#include"ocelostone.h"
#include"putpoint.h"
#include"string3d.h"
#include"object.h"

int DrawObject(Object *obj);
int UpdateObject(Object *obj);
int DeleteObjectList(ObjectList *list);


static ObjectList listHead;
static ObjectList *listTail;

static ObjectList list;

int ObjectListInit() {
  listHead.obj = NULL;
  listHead.next = NULL;
  listTail = &listHead;
  return 1;
}

int UpdateAllObject() {
  ObjectList *objList;

  objList = listHead.next;
  while(objList) {
    UpdateObject(objList->obj);
    objList = objList->next;
  }

  return 1;
}

int DrawAllObject() {
  ObjectList *objList;

  objList = listHead.next;
  while(objList) {
    glPushMatrix();
    DrawObject(objList->obj);
    objList = objList->next;
    glPopMatrix();
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
    case OBJECT_BITSTRING:
      DrawString(obj->object.string);
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
    case OBJECT_BITSTRING:
      UpdateString(obj->object.string);
      break;
    default:
      printf("(%s)Error line:%d\n", __FILE__, __LINE__);
      exit(1);
  }

  return 1;
}

int AddObject(Object *obj) {
  ObjectList *list;

  list = (ObjectList*)malloc(sizeof(ObjectList));
  if(list == NULL) return 0;

  list->next = NULL;
  list->obj = obj;

  listTail->next = list;
  listTail = list;

  return 1;
}

int DeleteAllObject() {
  ObjectList *list;
  ObjectList *temp;

  list = listHead.next;
  while(list) {
    temp = list->next;
    DeleteObjectList(list);
    list = temp;  
  }

  return ObjectListInit();
}

int DeleteSelectedTypeObject(ObjectType type) {
  ObjectList *list, *before;

  list = listHead.next;
  before = &listHead;

  while(list) {
    if(list->obj->type == type) {
      before->next = list->next;
      if(list == listTail) {
        listTail = before;
      }
      DeleteObjectList(list);
      list = before->next;
      continue;
    }

    before = list;
    list = list->next;
  }

  return 1;
}

int DeleteObject(Object *obj) {
  ObjectList *list, *before;

  list = listHead.next;
  before = &listHead;
  while(list) {
    if(list->obj == obj) {
      before->next = list->next;
      if(list == listTail) {
        listTail = before;
      }
      DeleteObjectList(list);
      return 1;
    }

    before = list;
    list = list->next;
  }

  return 0;
}

int DeleteObjectList(ObjectList *list) {
  switch(list->obj->type) {
    case OBJECT_OCELO_STONE:
      free(list->obj->object.stone);
      break;
    case OBJECT_SELECTABLE_POINT:
      free(list->obj->object.point);
      break;
    case OBJECT_BITSTRING:
      free(list->obj->object.string);
      break;
  }

  free(list->obj);
  free(list);
  return 1;
}

int DeleteString(String3D *str) {
  ObjectList *list, *before;

  list = listHead.next;
  before = &listHead;
  while(list) {
    if(list->obj->type == OBJECT_BITSTRING) {
      if(list->obj->object.string == str) {
        before->next = list->next;
        if(list == listTail) {
          listTail = before;
        }

        DeleteObjectList(list);
        return 1;
      }
    }

    before = list;
    list = list->next;
  }

  return 0;
}