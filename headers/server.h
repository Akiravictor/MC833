#ifndef SERVER_H
#define SERVER_H

#include "includes.h"


/* Structs */
/* Essa struct foi feita para facilitar o envio de mensagens padrão do servidor para o cliente, visto que strings em C é algo com potencial para dar problema. */
typedef struct messages{
  char *welcome;
  char *error_persona;
  char *prof;
  char *student;
  char *ask_code;
  char *ask_msg;
  char *ask_room;
  char *ask_hours;
  char *ask_ementa;
}messages;

/* Headers */
void sigchld_handler(int s);
void *get_in_addr(struct sockaddr *sa);

/* Função que retorna a struct com todas as mensagens padrões definidas */
messages messages_constructor();
/* Funçao que envia e recebe strings servidor-cliente */
void send_and_receive(int incoming_fd, char *string, int *buf_size, char *buffer);
#endif
