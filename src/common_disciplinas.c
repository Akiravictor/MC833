#include "disciplinas.h"

/* struct lista lista_constructor(){ */
/*   struct lista obj; */
/*   obj.size = 0; */
/*   obj.disc = NULL; */
/*   obj.next = NULL; */
/*   return obj; */
/* } */
 


disciplina disciplina_constructor(int code, int sala, char *horarios, char *mensagem){
  disciplina obj;
  obj.code = code;
  obj.sala = sala;
  strcpy(obj.horarios,horarios);
  strcpy(obj.mensagem,mensagem);
  return obj;
}
