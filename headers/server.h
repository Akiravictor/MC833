#ifndef SERVER_H
#define SERVER_H

#include "includes.h"

#define PORT "54321"
#define QUEUE 10
#define MAXDATASIZE 255

/* Structs */
typedef struct messages{
  char *welcome;
  char *error_persona;
  char *prof;
  char *student;
  char *ask_code;
}messages;

/* Headers */
void sigchld_handler(int s);
void *get_in_addr(struct sockaddr *sa);
messages messages_constructor();
void send_and_receive(int incoming_fd, char *string, int *buf_size, char *buffer);
#endif
