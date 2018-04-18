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
 


disciplina disciplina_constructor(char *code, char *sala, char *horarios, char *mensagem){
  disciplina obj;
  strcpy(obj.code,code);
  strcpy(obj.sala,sala);
  strcpy(obj.horarios,horarios);
  strcpy(obj.mensagem,mensagem);
  obj.deleted = 'n';
  obj.next = NULL;
  //printf("%s %s %s %s\n",obj.code,obj.sala,obj.horarios,obj.mensagem);
  return obj;
}

void add_disciplina(lista *l, char *code, char *sala, char *horarios, char *mensagem){
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
  if(d.deleted == 'n')
    printf("\nCódigo: %s\nSala: %s\nHorário: %s\nMensagem: %s\n",d.code,d.sala,d.horarios,d.mensagem);
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

void change_message(lista *l, char *code, char *mensagem){
  int found = 0;
  if(l->size){
    l->iterator = l->head;
    for(int i=0; i< l->real_size; i++){
      if( strcmp(l->iterator->code , code) == 0){
	strcpy(l->iterator->mensagem , mensagem);
	found = 1;
	printf("Changed the message inside discipline %s\n",code);
      }

	if(l->iterator->next != NULL)
	  l->iterator = l->iterator->next;

    }
    if(!found){
      printf("Discipline %s not found\n",code);
    }
  }
  else{
    printf("There is no disciplines yet\n");
  }
}

void delete_disc(lista *l, char *code){
  int found = 0;
  if(l->size){
    l->iterator = l->head;
    for(int i=0; i< l->real_size; i++){
      if(strcmp(l->iterator->code , code) == 0){
	l->iterator->deleted = 'y';
	found = 1;
	printf("Discipline %s is deleted\n",code);
	l->size--;
      }

      if(l->iterator->next != NULL)
	l->iterator = l->iterator->next;
  
    }
    if(!found){
      printf("Discipline %s not found\n",code);
    }
  }
  else{
    printf("There is no disciplines yet\n");
  }
}


char *p_disc(disciplina d){
  char *a = (char*)malloc(300*sizeof(char));
  if(d.deleted == 'n')
    sprintf(a,"\nCódigo: %s\nSala: %s\nHorário: %s\nMensagem: %s\n",d.code,d.sala,d.horarios,d.mensagem);
  else;
    printf("SERVER: %s\n",a);
  return a;
}

char *p_list(lista l){
  char *s, *d = (char*)malloc((300)*sizeof(char));
  if(l.size){
    s = (char*)malloc((l.size*300+60)*sizeof(char));
    l.iterator = l.head;
    strcpy(s,"Disciplines registered:\n");
    for(int i=0; i<l.real_size; i++){
      d = p_disc(*l.iterator);
      strcat(s,d);
      l.iterator = l.iterator->next;
    }
    
  }
  else{
    s = (char*)malloc((60)*sizeof(char));
    strcpy(s,"There is no disciplines yet\n");
  }
  strcat(s,"\nPress enter to go back\n");
  return s;
}
