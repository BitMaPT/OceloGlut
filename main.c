#include<stdio.h>
#include<stdlib.h>
#include<GL/glut.h>
#include<unistd.h>
#include"oceloboard.h"
#include"object.h"
#include"mouse.h"
#include"gamecontroller.h"
#include"pngimage.h"
#include"syncgame.h"

void Display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);

  DrawAllObject();

  glutSwapBuffers();
}

void Resize(int w, int h) {
  glViewport(0, 0, w, h);
  glLoadIdentity();
  glOrtho(0, w, h, 0, -300.0, 300.0);
}

void Idle() {
  static int timestd = 1000 / 60;
  int timenow;

  timenow = glutGet(GLUT_ELAPSED_TIME);

  if(timenow > timestd) {
    timestd = timenow + (1000 / 60);
    
    if(!ControlGameWithState()) {
      close(clientSockfd);
      exit(1);
    }

    UpdateAllObject();

    glutPostRedisplay();
    ResetMouseState();
  }
}

void init(int argc, char **argv) {
  glClearColor(0, 0.7, 0, 1);
  if(!LoadImage("Stunt.png")) exit(1);
  if(!LoadImage("img/comerror.png")) exit(1);
  if(!LoadImage("img/draw.png")) exit(1);
  if(!LoadImage("img/gameover.png")) exit(1);
  if(!LoadImage("img/lose.png")) exit(1);
  if(!LoadImage("img/myturn.png")) exit(1);
  if(!LoadImage("img/oplost.png")) exit(1);
  if(!LoadImage("img/opturn.png")) exit(1);
  if(!LoadImage("img/search.png")) exit(1);
  if(!LoadImage("img/serverlost.png")) exit(1);
  if(!LoadImage("img/win.png")) exit(1);
  if(!SetSocket(argc, argv)) exit(1);
}

void closeWindow() {
  //close(clientSockfd);
}

int main(int argc, char **argv) {
  if(argc < 3) {
    fprintf(stderr, "write host and port\n");
    exit(1);
  }

  glutInit(&argc, argv);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutInitWindowPosition(100, 100);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
  glutCreateWindow(argv[0]);
  glutDisplayFunc(Display);
  glutReshapeFunc(Resize);
  glutMouseFunc(Mouse);
  glutIdleFunc(Idle);
  init(argc, argv);
  atexit(closeWindow);

  glutMainLoop();

  return 0;
}