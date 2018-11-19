#ifndef TABSYMBOL
#define TABSYMBOL

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#ifndef ANALYSEUR_ZZ
#include "analyseur_zz.h"
#endif

typedef struct{
	char* name;
	char* type;
	bool init;
	double val_init;
	int line;
}symb;

typedef struct _list_symbol{
	symb symbol;
	struct _list_symbol* next;
}list_symbol;

list_symbol* lsymb;

extern void init_list_symbol();
extern bool add_symbol(char* name, char* type, bool init, double val_init, int line);
extern bool check_type(char *type, typetoken token);

#endif