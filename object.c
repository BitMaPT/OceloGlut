#include<stdio.h>
#include<stdlib.h>
#include"ocelostone.h"
#include"object.h"

int DrawObject(Object *obj);
int UpdateObject(Object *obj);
int DeleteObjectList(ObjectList *list);

static ObjectList *last;
static ObjectList head;

static ObjectList list;

int ObjectListInit() {
  head.obj = NULL;
  head.next = NULL;
  last = NULL;
  return 1;
}

int UpdateAllObject() {
  ObjectList *objList;

  objList = head.next;
  while(objList) {
    UpdateObject(objList->obj);
    objList = objList->next;
  }

  return 1;
}

int DrawAllObject() {
  ObjectList *objList;

  objList = head.next;
  while(objList) {
    DrawObject(objList->obj);
    objList = objList->next;
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
  ObjectList *list;

  list = (ObjectList*)malloc(sizeof(ObjectList));
  if(list == NULL) return 0;

  list->next = NULL;
  list->obj = obj;

  if(head.next == NULL) {
    head.next = list;
  }
  if(last != NULL) {
    last->next = list;
  }
  last = list;

  return 1;
}

int DeleteAllObject() {
  ObjectList *list;
  ObjectList *temp;

  list = head.next;
  while(list) {
    temp = list->next;
    DeleteObjectList(list);
    list = temp;  
  }

  return ObjectListInit();
}

int DeleteSelectedTypeObject(ObjectType type) {
  ObjectList *list, *before;

  list = head.next;
  before = &head;

  while(list) {
    if(list->obj->type == type) {
      before->next = list->next;
      if(list == last) {
        last = before;
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

int CheckAllReverseAnimationEnded() {
  ObjectList *list;

  list = head.next;
  while(list) {
    if(list->obj->type == OBJECT_OCELO_STONE) {
      switch(list->obj->object.stone->state.state) {
        case STONE_STATE_REVERSE_B2W: return 0;
        case STONE_STATE_REVERSE_W2B: return 0;
        default: break;
      }
    }
  }

  return 1;
}

int DeleteObject(Object *obj) {
  ObjectList *list, *before;

  list = head.next;
  before = &head;
  while(list) {
    if(list->obj == obj) {
      before->next = list->next;
      if(list == last) {
        last = before;
      }
      DeleteObjectList(list);
      list = before->next;
      continue;
    }

    before = list;
    list = list->next;
  }
}

int DeleteObjectList(ObjectList *list) {
  switch(list->obj->type) {
    case OBJECT_OCELO_STONE:
      free(list->obj->object.stone);
      break;
    case OBJECT_SELECTABLE_POINT:
      free(list->obj->object.point);
      break;
    case OBJECT_STRING:
      free(list->obj->object.string);
      break;
  }

  free(list->obj);
  free(list);
  return 1;
}