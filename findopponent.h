#ifndef HEADER_FINDOPPONENT
#define HEADER_FINDOPPONENT

typedef enum {
  FINDOPPONENT_INIT,
  FINDOPPONENT_SEARCH
}FindOpponent;

int FindOpponentScene(char *name, char *service);

extern FindOpponent findopponent_state;

#endif