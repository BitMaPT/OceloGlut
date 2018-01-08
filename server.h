#ifndef HEADER_SERVER
#define HEADER_SERVER

typedef struct SocketList SocketList;

struct SocketList{
  int sockfd;
  SocketList *next;
};

#endif