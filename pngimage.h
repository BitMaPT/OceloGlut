#ifndef HEADER_PNGIMAGE
#define HEADER_PNGIMAGE

#include<GL/glpng.h>

typedef struct ImageCacheList ImageCacheList;
struct ImageCacheList {
  int id;
  pngInfo info;
  char *name;
  ImageCacheList *next;
};

typedef struct {
  int startTime;
  int stateTime;
  int shiftTime;
  int stateNumber;
  int stateCount;
  int animEnd;
}ImageState;

typedef struct Image Image;
struct Image{
  int id;
  int pos[2];
  int size[2];
  pngInfo info;
  ImageState state;
  int (*Animation)(Image*);
};

typedef struct ImageList ImageList;
struct ImageList {
  Image *image;
  ImageList *next;
};

int LoadImage(char *filename);
Image* InitImage(char *filename, int pos[2], int size[2], int (*Animation)(Image*));
int CheckAllImageAnimationFinished();

int ImageNoneAnim(Image *image);
int ImageAnimByYaxis(Image *image);
int ImageAnimByXaxis(Image *image);

#endif