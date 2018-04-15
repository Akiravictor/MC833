#ifndef DISCIPLINAS_H
#define DISCIPLINAS_H

#include "includes.h"


/* structs  */

typedef struct disciplina{
  int code;
  int sala;
  char horarios[50];
  char mensagem[255];
  int deleted;
  struct disciplina *next;
} disciplina;


typedef struct lista{
  disciplina *head;
  disciplina *tail;
  disciplina *iterator;
  int real_size;
  int size;
}lista;


disciplina disciplina_constructor(int code, int sala, char *horarios, char *mensagem);
lista lista_constructor();
void add_disciplina(lista *l, int code, int sala, char *horarios, char *mensagem);
void print_disc(disciplina d);
void print_list(lista l);
void change_message(lista *l, int code, char *mensagem);
void delete_disc(lista *l, int code);

#endif
