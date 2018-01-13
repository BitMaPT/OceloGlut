#include<GL/glut.h>
#include<GL/glpng.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"object.h"
#include"pngimage.h"

void UpdateImage(Object *obj);
void DrawImage(Object *obj);
void DeleteImage(Object *obj);
int GetImage(char *filename, int *id, pngInfo *info);

ImageList imageHead = {NULL, NULL};
ImageList *imageTail = &imageHead;

ImageCacheList cacheHead;
ImageCacheList *cacheTail = &cacheHead;

int LoadImage(char *filename) {
  ImageCacheList *list;
  
  list = (ImageCacheList*)malloc(sizeof(ImageCacheList));
  if(list == NULL) {
    fprintf(stderr, "%s line:%d malloc error\n", __FILE__, __LINE__);
    return 0;
  }
  list->name = (char*)malloc(sizeof(char) * (strlen(filename) + 1));
  if(list == NULL) {
    fprintf(stderr, "%s line:%d malloc error\n", __FILE__, __LINE__);
    return 0;
  }

  list->id = pngBind(filename, PNG_NOMIPMAP, PNG_ALPHA, &(list->info), GL_CLAMP, GL_NEAREST, GL_NEAREST);
  strcpy(list->name, filename);
  list->next = NULL;

  cacheTail->next = list;
  cacheTail = list;

  return 1;
}

int GetImage(char *filename, int *id, pngInfo *info) {
  ImageCacheList *list;

  list = cacheHead.next;
  while(list) {
    if(strcmp(list->name, filename) == 0) {
      *id = list->id;
      *info = list->info;
      return 1;
    }
  }

  return 0;
}

Image* InitImage(char *filename, int pos[2], int size[2], int (*Animation)(Image*)) {
  Image *image;
  Object *obj;
  pngInfo info;
  ImageState state = {glutGet(GLUT_ELAPSED_TIME), 0, 1000, 0, 3, 0};
  ImageList *list;

  image = (Image*)malloc(sizeof(Image));
  if(image == NULL) {
    fprintf(stderr, "%s line:%d malloc error\n", __FILE__, __LINE__);
    return NULL;
  }
  obj = (Object*)malloc(sizeof(Object));
  if(obj == NULL) {
    fprintf(stderr, "%s line:%d malloc error\n", __FILE__, __LINE__);
    return NULL;
  }
  list = (ImageList*)malloc(sizeof(ImageList));
  if(list == NULL) {
    fprintf(stderr, "%s line:%d malloc error\n", __FILE__, __LINE__);
    return NULL;
  }

  //initialize Image
  if(!GetImage(filename, &(image->id), &(image->info))) {
    fprintf(stderr, "%s line:%d %s is not cached yet. please call LoadImage() before calling this function\n", __FILE__, __LINE__, filename);
    return NULL;
  }
  image->Animation = Animation;
  image->state = state;
  {
    int i;
    for(i = 0; i < 2; i++) {
      image->pos[i] = pos[i];
      image->size[i] = size[i];
    }
  }

  //initialize Object
  obj->object.image = image;
  obj->type = OBJECT_PNGIMAGE;
  obj->Update = UpdateImage;
  obj->Draw = DrawImage;
  obj->Delete = DeleteImage;

  AddObject(obj);

  list->image = image;
  list->next = NULL;
  imageTail->next = list;
  imageTail = list;

  return image;
}

void UpdateImage(Object *obj) {
  //nothing to do
}

void DrawImage(Object *obj) {
  Image *image = obj->object.image;
  GLfloat x, y;

  if(image->state.animEnd) return;

  x = image->size[0];
  y = image->size[1];

  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  if(image->Animation(image)) {
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-x / 2, -y / 2);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-x / 2, y / 2);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(x / 2, y / 2);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(x / 2, -y / 2);
    glEnd();
  }

  glDisable(GL_BLEND);
  glDisable(GL_TEXTURE_2D);
}

int ImageNoneAnim(Image *image) {
  glTranslatef(image->pos[0], image->pos[1], 0);
  return 1;
}

int ImageAnimByYaxis(Image *image) {
  int angle;
  int nowTime = glutGet(GLUT_ELAPSED_TIME);

  image->state.stateTime = nowTime - image->state.startTime;
  if(image->state.shiftTime < image->state.stateTime) {
    image->state.startTime = nowTime;
    image->state.stateTime = 0;
    image->state.stateNumber++;
    if(image->state.stateNumber == image->state.stateCount) {
      image->state.animEnd = 1;
    }
  }

  if(image->state.animEnd) return 0;

  switch(image->state.stateNumber) {
    case 0: 
      angle = (90.0 / image->state.shiftTime) * image->state.stateTime - 90;
      break;
    case 1:
      angle = 0;
      break;
    case 2:
      angle = (90.0 / image->state.shiftTime) * image->state.stateTime;
      break;
  }

  glTranslatef(image->pos[0], image->pos[1], 0);
  glRotatef(angle, 0, 1.0, 0);

  return 1;
}

int ImageAnimByXaxis(Image *image) {
  int angle;
  int nowTime = glutGet(GLUT_ELAPSED_TIME);

  image->state.stateTime = nowTime - image->state.startTime;
  if(image->state.shiftTime < image->state.stateTime) {
    image->state.startTime = nowTime;
    image->state.stateTime = 0;
    image->state.stateNumber++;
    if(image->state.stateNumber == image->state.stateCount) {
      image->state.animEnd = 1;
    }
  }
  
  if(image->state.animEnd) return 0;

  switch(image->state.stateNumber) {
    case 0: 
      angle = (90.0 / image->state.shiftTime) * image->state.stateTime - 90;
      break;
    case 1:
      angle = 0;
      break;
    case 2:
      angle = (90.0 / image->state.shiftTime) * image->state.stateTime;
      break;
  }

  glTranslatef(image->pos[0], image->pos[1], 0);
  glRotatef(angle, 1.0, 0.0, 0);

  return 1;
}

int CheckAllImageAnimationFinished() {
  ImageList *list;

  list = imageHead.next;
  while(list) {
    if(!list->image->state.animEnd) return 0;
  }

  return 1;
}

void DeleteImage(Object *obj) {
  Image *image = obj->object.image;
  ImageList *list, *before;

  list = imageHead.next;
  before = &imageHead;
  while(list) {
    if(list->image == image) {
      before->next = list->next;
      if(list == imageTail) {
        imageTail = before;
      }

      free(list);
      free(list->image);
      break;
    }
    before = list;
    list = list->next;
  }
}