#ifndef DISCIPLINAS_H
#define DISCIPLINAS_H

#include "includes.h"


/* structs  */

/* define um nó da lista ligada */
typedef struct disciplina{
  char code[10];
  char sala[10];
  char horarios[50];
  char mensagem[255];
  char ementa[255];
  char deleted;
  struct disciplina *next;
} disciplina;


/* Define a lista ligada com algumas metadatas */
typedef struct lista{
  disciplina *head;
  disciplina *tail;
  disciplina *iterator;
  int real_size;
  int size;
}lista;

/* Construtores da lista e de um nó */
disciplina disciplina_constructor(char *code, char *sala, char *horarios, char *mensagem, char *ementa);
lista lista_constructor();

/* Funçoes relativas a lista ligada, por nao retornarem strings só podem ser usadas localmente pelo servidor */
void add_disciplina(lista *l, char *code, char *sala, char *horarios, char *mensagem, char *ementa);
void print_disc(disciplina d);
void print_list(lista l);
void change_message(lista *l, char *code, char *mensagem);
void delete_disc(lista *l, char *code);

/* Funçoes análogas as anteriores mas retornam strings que sao enviadas do servidor ao cliente */

/* deleta disciplina */
char *d_disc(lista *l, char *code);
/* imprime uma disciplina */
char *p_disc(disciplina d);
/* imprime uma lista inteira */
char *p_list(lista l);
/* c -> change, funçoes que mudam a mensagem,ementa etc de uma disciplina dado o código */
char *c_message(lista *l, char *code, char *mensagem);
char *c_ementa(lista *l, char *code, char *ementa);
char *c_sala(lista *l, char *code, char *sala);
char *c_horario(lista *l, char *code, char *horarios);
/* adiciona uma disciplina nova */
char *a_disciplina(lista *l, char *code, char *sala, char *horarios, char *mensagem, char *ementa);
#endif
