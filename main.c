#include<GL/glut.h>

#define WIDTH 640
#define HEIGHT 640

void Display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);

  glutSwapBuffers();
}

void Resize(int w, int h) {
  glOrtho(0, WIDTH, HEIGHT, 0, -1.0, 1.0);
}

void Mouse(int button, int state, int x, int y) {
  switch(button) {
    case GLUT_LEFT_BUTTON:
      break;
    default:
      break;
  }
}

void Idle() {
  static int timestd = 1000.0 / 60.0;
  int timenow;

  timenow = glutGet(GLUT_ELAPSED_TIME);

  if(timenow > timestd) {
    timestd += timestd + (1000 / 60);
    glutPostRedisplay();
  }
}

void init() {
  glClearColor(0, 1, 0, 1);
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutInitWindowPosition(100, 100);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
  glutCreateWindow(argv[0]);
  glutDisplayFunc(Display);
  glutReshapeFunc(Resize);
  glutMouseFunc(Mouse);
  glutIdleFunc(Idle);
  init();

  glutMainLoop();

  return 0;
}