#include<stdio.h>
#include<stdlib.h>
#include"object.h"

int DrawObject(Object *obj);
int UpdateObject(Object *obj);
int DeleteObjectList(ObjectList *list);


static ObjectList listHead;
static ObjectList *listTail = &listHead;

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
    objList->obj->Update(objList->obj);
    objList = objList->next;
  }

  return 1;
}

int DrawAllObject() {
  ObjectList *objList;

  objList = listHead.next;
  while(objList) {
    glPushMatrix();
    objList->obj->Draw(objList->obj);
    objList = objList->next;
    glPopMatrix();
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
    list->obj->Delete(list->obj);
    free(list);
    list = temp;  
  }

  return ObjectListInit();
}

int DeleteSelectedTypeObject(ObjectType type) {
  ObjectList *list, *before;

  list = listHead.next;
  before = &listHead;

  while(list) {
    Object *obj = list->obj;

    if(obj->type == type) {
      before->next = list->next;
      if(list == listTail) {
        listTail = before;
      }

      obj->Delete(obj);
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
      list->obj->Delete(list->obj);
      free(list->obj);
      return 1;
    }

    before = list;
    list = list->next;
  }

  return 0;
}