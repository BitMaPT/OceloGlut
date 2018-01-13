#include<GL/glut.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"bitstring.h"
#include"object.h"

BitString* InitBitString(char *str, int x, int y, double color[3]) {
  BitString *bitStr;
  Object *obj;
  char *copiedStr;

  bitStr = (BitString*)malloc(sizeof(BitString));
  if(bitStr == NULL) {
    fprintf(stderr, "%s line:%d malloc error\n", __FILE__, __LINE__);
    return NULL;
  }
  obj = (Object*)malloc(sizeof(Object));
  if(obj == NULL) {
    fprintf(stderr, "%s line:%d malloc error\n", __FILE__, __LINE__);
    return NULL;
  }
  copiedStr = (char*)malloc(sizeof(char) * strlen(str));
  if(copiedStr == NULL) {
    fprintf(stderr, "%s line:%d malloc error\n", __FILE__, __LINE__);
    return NULL;
  }

  strcpy(copiedStr, str);
  bitStr->str = copiedStr;
  bitStr->pos[0] = x;
  bitStr->pos[1] = y;
  {
    int i;
    for(i = 0; i < 3; i++) {
      bitStr->color[i] = color[i];
    }
  }

  obj->object.string = bitStr;
  obj->type = OBJECT_BITSTRING;

  if(AddObject(obj) == 0) return NULL;

  return bitStr;
}