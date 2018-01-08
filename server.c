#include<stdlib.h>
#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
#include"server.h"

#define SERVER_PORT 49152
#define BACKLOG_NUM 10

int NonBlockingAccept(int l_sockfd, struct sockaddr_in sa);

SocketList head = {-1, NULL};
SocketList *last = &head;

int main(int argc, char **argv) {
  int l_sockfd, maxfd, count;
  struct sockaddr_in sa;
  fd_set readfds;

  sa.sin_family = AF_INET;
  sa.sin_port = htons(SERVER_PORT);
  sa.sin_addr.s_addr = INADDR_ANY;

  count = 0;
  do {
    SocketList *list;
    NonBlockingAccept(l_sockfd, sa);
    list = head.next;
    while(list) {
      count++;
      list = list->next;
    }
  }while(count < 2);

  maxfd = (head.next->sockfd > head.next->next->sockfd) ? head.next->sockfd : head.next->next->sockfd;

  while(1) {
    int retval;
    SocketList *list;
    struct timeval tval = {0, 0};

    FD_ZERO(&readfds);
    list = head.next;
    while(list) {
      FD_SET(list->sockfd, &readfds);
    }
    retval = select(maxfd + 1, &readfds, NULL, NULL, &tval);
    if(retval < 0) {
      fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
      perror("select");
      close(l_sockfd);
      return 1;
    } else if(retval) {
      
    }

  }
}

int NonBlockingAccept(int l_sockfd, struct sockaddr_in sa) {
  SocketList *new_socket;
  struct timeval tval = {0, 0};
  fd_set readfds;
  int retval;
  char buf[1];

  FD_ZERO(&readfds);
  FD_SET(l_sockfd, &readfds);
  
  retval = select(l_sockfd + 1, &readfds, NULL, NULL, &tval);
  
  if(retval < 0) {
    fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
    perror("select");
    return 0;

    //accept is ready
  } else if (retval) {
    int new_sockfd;
    socklen_t size;
    ssize_t recv_size;

    new_socket = (SocketList*)malloc(sizeof(SocketList));
    if(new_socket == NULL) {
      fprintf(stderr, "%s line:%d malloc error\n", __FILE__, __LINE__);
      return 0;
    }

    size = sizeof(sa);
    new_sockfd = accept(l_sockfd, (struct sockaddr*)&sa, &size);
    if(new_sockfd < 0) {
      fprintf(stderr, "%s line:%d ", __FILE__, __LINE__);
      perror("accept");
      return 0;
    }

    new_socket->sockfd = new_sockfd;
    new_socket->next = NULL;

    last->next = new_socket;
    last = new_socket;
  }

  return 1;
}

