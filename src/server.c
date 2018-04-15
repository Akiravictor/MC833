#include "includes.h"






int main(){
  disciplina d;
  d = disciplina_constructor(833,332,"qui 10h am","o prof edmundo é show");
  printf("%d\n%d\n%s\n%s\n",d.code,d.sala,d.horarios,d.mensagem);
  return 0;
}

