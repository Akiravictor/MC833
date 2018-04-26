#ifndef INCLUDE_H
#define INCLUDE_H

/* libs */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <netinet/in.h>

/* Custom libs */
#include "disciplinas.h"
#include "client.h"
#include "server.h"


/* defines */
#define ADDR "localhost"
#define PORT "54321"
#define QUEUE 10
#define MAXDATASIZE 2048
#define TRUE 1
#define FALSE 0

#endif
