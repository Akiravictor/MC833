#ifndef SERVER_H
#define SERVER_H

#include "includes.h"

#define PORT "54321"
#define QUEUE 10
#define MAXDATASIZE 255

/* Headers */
void sigchld_handler(int s);
void *get_in_addr(struct sockaddr *sa);


#endif
