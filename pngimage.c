#include<GL/glut.h>
#include<GL/glpng.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"object.h"
#include"pngimage.h"
#include"gamecontroller.h"

void UpdateImage(Object *obj);
void DrawImage(Object *obj);
void DeleteImage(Object *obj);
int GetImage(char *filename, int *id, pngInfo *info);

ImageList imageHead = {NULL, NULL};
ImageList *imageTail = &imageHead;

ImageCacheList cacheHead;
ImageCacheList *cacheTail = &cacheHead;

int defaultPos[] = {WIDTH / 2, HEIGHT / 2};
int defaultSize[] = {255, 255};

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
  if(list->id == 0) {
    fprintf(stderr, "%s line:%d image bind error\n", __FILE__, __LINE__);
    return 0;
  }
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
  glBindTexture(GL_TEXTURE_2D, image->id);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_CULL_FACE);

  
  glColor3d(1, 1, 1);
  if(image->Animation(image)) {
    glRotated(180, 0, 1, 0);
    glRotated(180, 1, 0, 0); 

    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(-x / 2, -y / 2);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(-x / 2, y / 2);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(x / 2, y / 2);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(x / 2, -y / 2);

    glTexCoord2f(1.0f, 1.0f); glVertex2f(x / 2, -y / 2);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(x / 2, y / 2);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-x / 2, y / 2);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-x / 2, -y / 2);
    glEnd();
  }

  glDisable(GL_CULL_FACE);
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
  if(image->state.stateTime < 300) {
    angle = (90.0 / 300.0) * image->state.stateTime - 90;
  } else if(image->state.stateTime >= 300 && image->state.stateTime < 1000) {
    angle = 0;
  } else if(image->state.stateTime >= 1000 && image->state.stateTime < 1300) {
    angle = (90.0 / 300.0) * image->state.stateTime - 300;
  } else {
    image->state.animEnd = 1;
    return 0;
  }

  glTranslatef(image->pos[0], image->pos[1], 0);
  glRotatef(angle, 0, 1.0, 0);

  return 1;
}

int ImageAnimByXaxis(Image *image) {
  int angle;
  int nowTime = glutGet(GLUT_ELAPSED_TIME);

  image->state.stateTime = nowTime - image->state.startTime;
  if(image->state.stateTime < 300) {
    angle = (90.0 / 300.0) * image->state.stateTime - 90;
  } else if(image->state.stateTime >= 300 && image->state.stateTime < 1000) {
    angle = 0;
  } else if(image->state.stateTime >= 1000 && image->state.stateTime < 1300) {
    angle = (90.0 / 300.0) * image->state.stateTime - 300;
  } else {
    image->state.animEnd = 1;
    return 0;
  }

  glTranslatef(image->pos[0], image->pos[1], 0);
  glRotatef(angle, 1.0, 0.0, 0);

  return 1;
}

int ImageAnimZoomIn(Image *image) {
  int nowTime = glutGet(GLUT_ELAPSED_TIME);
  GLdouble scale = 1.0;
  float shiftTime = 200.0;

  image->state.stateTime = nowTime - image->state.startTime;
  if(shiftTime > image->state.stateTime) {
    scale =  (0.7 / shiftTime) * image->state.stateTime + 0.3;
  }

  glTranslated(image->pos[0], image->pos[1], -10);
  glScaled(scale, scale, 0);

  return 1;
}

int CheckAllImageAnimationFinished() {
  ImageList *list;

  list = imageHead.next;
  while(list) {
    if(!list->image->state.animEnd) return 0;
    list = list->next;
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

      free(list->image);
      free(list);
      break;
    }
    before = list;
    list = list->next;
  }
}