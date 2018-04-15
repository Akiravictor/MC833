#include "disciplinas.h"

/* private funcs headers */
void remove_head(lista *l);
void remove_tail(lista *l);

/* funcs body */


lista lista_constructor(){
  lista obj;
  obj.size = 0;
  obj.real_size = 0;
  obj.head = NULL;
  obj.tail = NULL;
  obj.iterator = NULL;
  return obj;
}
 


disciplina disciplina_constructor(int code, int sala, char *horarios, char *mensagem){
  disciplina obj;
  obj.code = code;
  obj.sala = sala;
  strcpy(obj.horarios,horarios);
  strcpy(obj.mensagem,mensagem);
  obj.deleted = 0;
  obj.next = NULL;
  return obj;
}

void add_disciplina(lista *l, int code, int sala, char *horarios, char *mensagem){
  disciplina *d = malloc(sizeof(disciplina));

  *d = disciplina_constructor(code,sala,horarios,mensagem);
  /* first node */
  if(l->real_size == 0){
    l->head = d;
    l->tail = d;
  }
  /* list is not empty */
  else{
    l->tail->next = d;
    l->tail = l->tail->next;
  }
  l->size++;
  l->real_size++;
}

void print_disc(disciplina d){
  if(!d.deleted)
    printf("\nCódigo: %d\nSala: %d\nHorário: %s\nMensagem: %s\n",d.code,d.sala,d.horarios,d.mensagem);
}

void print_list(lista l){
  if(l.size){
    l.iterator = l.head;
    printf("Disciplines registered:\n");
    for(int i=0; i<l.real_size; i++){
      print_disc(*l.iterator);
      l.iterator = l.iterator->next;
    }
    
  }
  else{
    printf("There is no disciplines yet\n");
  }
}

void change_message(lista *l, int code, char *mensagem){
  int found = 0;
  if(l->size){
    l->iterator = l->head;
    for(int i=0; i< l->real_size; i++){
      if(l->iterator->code == code){
	strcpy(l->iterator->mensagem , mensagem);
	found = 1;
	printf("Changed the message inside discipline %d\n",code);
      }

	if(l->iterator->next != NULL)
	  l->iterator = l->iterator->next;

    }
    if(!found){
      printf("Discipline %d not found\n",code);
    }
  }
  else{
    printf("There is no disciplines yet\n");
  }
}

void delete_disc(lista *l, int code){
  int found = 0;
  if(l->size){
    l->iterator = l->head;
    for(int i=0; i< l->real_size; i++){
      if(l->iterator->code == code){
	l->iterator->deleted = 1;
	found = 1;
	printf("Discipline %d is deleted\n",code);
	l->size--;
      }

      if(l->iterator->next != NULL)
	l->iterator = l->iterator->next;
  
    }
    if(!found){
      printf("Discipline %d not found\n",code);
    }
  }
  else{
    printf("There is no disciplines yet\n");
  }
}

void remove_head(lista *l){
  disciplina *aux = l->head;
  if(l->head->next != NULL){
    l->head = l->head->next;
  }
  else
    l->head = NULL;

  free(aux);
  l->size--;
}

