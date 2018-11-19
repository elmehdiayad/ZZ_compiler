#include "error.h"




void add_smerror(char* name, int line, smerror_type type){
	list_error* new_error = malloc(sizeof(list_error*));
	new_error->error.name = (char *) malloc((strlen(name) + 1) * sizeof(char*));
	stpcpy(new_error->error.name, name);
	new_error->error.line = line;
	new_error->error.type = type;
	new_error->next = lerror;
	lerror = new_error;
}

int nmbr_smerror(){
	int nmbr = 0;
	list_error* current = lerror;
	while(current != NULL){
		nmbr++;
		current = current->next;
	}
	return nmbr;
}

void show_smerror(){
	printf("%d semantic error:\n", nmbr_smerror());
	list_error* current = lerror;
	while(current != NULL){
		printf("line %d: %s ", current->error.line, current->error.name);
		switch(current->error.type){
			case not_declared : puts("variable not declared"); break; 
			case already_declared : puts("variable already declared"); break; 
			case bad_initialized : puts("variable badly initialized"); break; 
			case bad_assigned : puts("variable badly assigned"); break; 
			case bad_operation : puts("variable badly operated"); break; 
		}
		current = current->next;
	}
}

