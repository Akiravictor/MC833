#include "disciplinas.h"

lista createLista(){
	lista l;
	l.size = 0;
	l.real_size = 0;
	l.head = NULL;
	l.tail = NULL;
	l.prox = NULL;
	
	return l;
}