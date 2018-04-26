#ifndef DISCIPLINAS_H
#define DISCIPLINAS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


/* structs  */

typedef struct disciplina{
  char code[10];
  char sala[10];
  char horarios[50];
  char ementa[255];
  char mensagem[255];
  char titulo[100];
  int deleted;
  struct disciplina *next;
} disciplina;



typedef struct lista{
  disciplina *head;
  disciplina *tail;
  disciplina *iterator;
  int real_size;
  int size;
} lista;


disciplina disciplina_constructor(char *code, char *sala, char *horarios, char *ementa, char *mensagem, char *titulo);
lista lista_constructor();
void add_disciplina(lista *l, char *code, char *sala, char *horarios, char *ementa, char *mensagem, char *titulo);
void print_disc(disciplina d);
void print_list(lista l);
void change_message(lista *l, char *code, char *mensagem);
void delete_disc(lista *l, char *code);

char *d_disc(lista *l, char *code);
char *p_disc(disciplina d);
char *p_list(lista l);
char *c_message(lista *l, char *code, char *mensagem);
char *a_disciplina(lista *l, char *code, char *sala, char *horarios, char *ementa, char *mensagem, char *titulo);
#endif
