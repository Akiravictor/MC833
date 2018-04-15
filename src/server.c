#include "includes.h"



int main(){
  /* Inicializando a lista e nosso banco de dados com algumas disciplinas inicialiadas */
  lista l = lista_constructor();
  add_disciplina(&l,833,332,"qui 10h am","o prof edmundo é show");
  add_disciplina(&l,558,17,"ter qui 4h pm","tá O(n) pelo menos");
  add_disciplina(&l,722,5,"ter qui 7h pm","MIIIIIPPPPPSSSS");

  return 0;
}

/* Para cole isso na main para testar inserçoes, remoçoes, mudanças no texto etc */
  /* print_list(l); */
  /* add_disciplina(&l,833,332,"qui 10h am","o prof edmundo é show"); */
  /* print_list(l); */
  /* add_disciplina(&l,558,17,"ter qui 4h pm","tá O(n) pelo menos"); */
  /* print_list(l); */
  /* add_disciplina(&l,722,5,"ter qui 7h pm","MIIIIIPPPPPSSSS"); */
  /* print_list(l); */

  /* change_message(&l,722,"E O SALÁRIO Ó"); */
  /* print_list(l); */

  /* delete_disc(&l,833); */
  /* print_list(l); */
  /*  delete_disc(&l,558); */
  /* print_list(l); */
  /*  delete_disc(&l,722); */
  /* print_list(l); */
  /* add_disciplina(&l,458,17,"ter qui 4h pm","algoritmos GULOSOS!!!"); */
  /* print_list(l); */
