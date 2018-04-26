#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "disciplinas.h"


/* Structs */
typedef struct messages{
  char *welcome;
  char *error_persona;
  char *prof;
  char *student;
  char *ask_code;
  char *ask_msg;
  char *ask_room;
  char *ask_hours;
}messages;

/* Headers */
void sigchld_handler(int s);
int setupConnectionS(int max_clients, int* client_socket);
void sendMsg(int sock, char* msg);
int recvMsgS(int sock, char* buffer);
void executeMenu(int whoIsConnected, char* buffer, lista *l, int sockfd);
messages messages_constructor();

#endif
