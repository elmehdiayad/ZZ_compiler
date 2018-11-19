#ifndef ERROR
#define ERROR

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


typedef enum{
	not_declared,
	already_declared,
	bad_initialized,
	bad_assigned,
	bad_operation,
}smerror_type;


typedef struct{
	char* name;
	int line;
	smerror_type type;
}smerror;

typedef struct _list_error{
	smerror error;
	struct _list_error* next;
}list_error;

list_error* lerror;

extern void init_list_error();
extern void add_smerror(char* name, int line, smerror_type type);
extern int nmbr_smerror();
extern void show_smerror();

#endif