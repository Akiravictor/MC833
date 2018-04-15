#ifndef DISCIPLINAS_H
#define DISCIPLINAS_H

#include "includes.h"


/* structs  */

typedef struct disciplina{
  int code;
  int sala;
  char horarios[50];
  char mensagem[255];
} disciplina;

disciplina disciplina_constructor(int code, int sala, char *horarios, char *mensagem);
/* typedef struct disciplina{ */
/*   int code; */
/*   int sala; */
/*   char horarios[50]; */
/*   char mensagem[255]; */
/* } disciplina; */

/* struct lista{ */
/*   disciplina *disc; */
/*   disciplina *next; */
/*   int size; */
/* }; */

/* disciplina disciplina_constructor(int code, int sala); */
/* struct lista lista_constructor(); */

  

#endif
