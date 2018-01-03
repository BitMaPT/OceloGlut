#ifndef HEADER_MOUSE
#define HEADER_MOUSE

void Mouse(int button, int state, int x, int y);
void ResetMouseState();
int GetMouseDown(int button, int *x, int *y);
int GetMouseUp(int button, int *x, int *y);

#endif