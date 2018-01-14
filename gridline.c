#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"object.h"
#include"gridline.h"
#include"oceloboard.h"

void UpdateGridLine(Object *obj);
void DrawGridLine(Object *obj);
void DeleteGridLine(Object *obj);

GridLine* InitGridLine(int pos[2], int line, int point) {
  GridLine *grid;
  Object *obj;

  grid = (GridLine*)malloc(sizeof(GridLine));
  if(grid == NULL) {
    fprintf(stderr, "%s line:%d malloc error\n", __FILE__, __LINE__);
    return NULL;
  }
  obj = (Object*)malloc(sizeof(Object));
  if(obj == NULL) {
    fprintf(stderr, "%s line:%d malloc error\n", __FILE__, __LINE__);
    return NULL;
  }

  memcpy(grid->pos, pos, sizeof(grid->pos));
  grid->lineWidth = line;
  grid->pointSize = point;

  obj->Update = UpdateGridLine;
  obj->Draw = DrawGridLine;
  obj->Delete = DeleteGridLine;
  obj->object.grid = grid;
  obj->type = OBJECT_GRIDLINE;

  if(AddObject(obj) == 0) return NULL;

  return grid;
}

void UpdateGridLine(Object *obj) {
  //nothing to do
}

void DrawGridLine(Object *obj) {
  GridLine *grid = obj->object.grid;
  int gridLength, i, lineWidth, pointSize;

  gridLength = OCELO_SQUARE_SIZE * OCELO_WIDTH;
  glGetIntegerv(GL_LINE_WIDTH, &lineWidth);
  glGetIntegerv(GL_POINT_SIZE, &pointSize);

  glColor3d(0, 0, 0);
  glTranslatef(grid->pos[0], grid->pos[1], 0);
  glLineWidth(grid->lineWidth);

  //draw gridline
  glBegin(GL_LINES);
  for(i = 0; i < OCELO_WIDTH + 1; i++) {
    glVertex2d(i * OCELO_SQUARE_SIZE, 0);
    glVertex2d(i * OCELO_SQUARE_SIZE, gridLength);
  }

  for(i = 0; i < OCELO_HEIGHT + 1; i++) {
    glVertex2d(0, i * OCELO_SQUARE_SIZE);
    glVertex2d(gridLength, i * OCELO_SQUARE_SIZE);
  }
  glEnd();
  
  //draw points on gridline
  glPointSize(grid->pointSize);
  glBegin(GL_POINTS);
  glVertex2d(2 * OCELO_SQUARE_SIZE, 2 * OCELO_SQUARE_SIZE);
  glVertex2d(2 * OCELO_SQUARE_SIZE, 6 * OCELO_SQUARE_SIZE);
  glVertex2d(6 * OCELO_SQUARE_SIZE, 2 * OCELO_SQUARE_SIZE);
  glVertex2d(6 * OCELO_SQUARE_SIZE, 6 * OCELO_SQUARE_SIZE);
  glEnd();

  glPointSize(pointSize);
  glLineWidth(lineWidth);
}

void DeleteGridLine(Object *obj) {
  GridLine *grid = obj->object.grid;

  free(grid);
}