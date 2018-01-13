#include<GL/glut.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>
#include"bitstring.h"
#include"object.h"

BitString* InitBitString(int x, int y, double color[3], int ctime, int patterns, ...) {
  va_list list;
  BitString *bitStr;
  Object *obj;
  StringState state = {0, 0, ctime, 0};

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
  bitStr->strPattern = (char**)malloc(sizeof(char*) * patterns);
  if(bitStr->strPattern == NULL) {
    fprintf(stderr, "%s line:%d malloc error\n", __FILE__, __LINE__);
    return NULL;
  }

  va_start(list, patterns);

  //initialize BitString struct
  {
    //store all patterns of string
    int i;
    for(i = 0; i < patterns; i++) {
      char *str;
      char *copiedStr;

      str = va_arg(list, char*);
      copiedStr = (char*)malloc(sizeof(char) * strlen(str));
      if(copiedStr == NULL) {
        fprintf(stderr, "%s line:%d malloc error\n", __FILE__, __LINE__);
        return NULL;
      }

      strcpy(copiedStr, str);
      bitStr->strPattern[i] = copiedStr;
    }  
  }
  bitStr->patternNum = patterns;
  state.startTime = glutGet(GLUT_ELAPSED_TIME);
  bitStr->state = state;
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

void UpdateBitString(Object *obj) {
  BitString *str = obj->object.string;
  int nowtime = glutGet(GLUT_ELAPSED_TIME);
  int stateTime;
  
  stateTime = nowtime - str->state.startTime;

  if(stateTime > str->state.shiftTime) {
    str->state.startTime = nowtime;
    str->state.stateNum = ((str->state.stateNum + 1) % str->patternNum);
  }
}

void DrawBitString(Object *obj) {
  BitString *bitstr = obj->object.string;
  char *str;
  int length, i;

  str = bitstr->strPattern[bitstr->patternNum];

  glRasterPos2f((GLfloat)bitstr->pos[0], (GLfloat)bitstr->pos[1] + 15);
  length = strlen(str);
  for(i = 0; i < length; i++) {
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);
  }
}

void DeleteBitString(Object *obj) {
  int i;
  BitString *bitstr;

  bitstr = obj->object.string;
  for(i = 0; i < bitstr->patternNum; i++) {
    free(bitstr->strPattern[i]);
  }
  free(bitstr->strPattern);
  free(bitstr);
}