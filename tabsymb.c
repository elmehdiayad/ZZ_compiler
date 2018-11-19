#include "tabsymb.h"




bool add_symbol(char* name, char* type, bool init, double val_init, int line){
	list_symbol* current = lsymb;
	bool flag = false;
	while(current != NULL && !flag){
		if(strcmp(current->symbol.name, name) == 0)
			flag = true;
		current = current->next;
	}
	if(!flag){
		list_symbol* new_symbol = malloc(sizeof(list_symbol*));
		new_symbol->symbol.name = (char *) malloc ((1 + strlen(name)) * sizeof(char));
		stpcpy(new_symbol->symbol.name, name);
		new_symbol->symbol.type = (char *) malloc ((1 + strlen(type)) * sizeof(char));
		stpcpy(new_symbol->symbol.type, type);
		new_symbol->symbol.init = init;
		new_symbol->symbol.val_init = val_init;
		new_symbol->symbol.line = line;	
		new_symbol->next = lsymb;
		lsymb = new_symbol;
	}	
	return !flag;
}

bool check_type(char *type, typetoken token){
  bool result = false;
  if (!strcasecmp(type, "bool") && (token == TRUE || token == FALSE)) {
    result = true;
  } else if (!strcasecmp(type, "int") && token == INT) {
    result = true;
  } else if (!strcasecmp(type, "double") && token == DOUBLE) {
    result = true;
  }
  return result;
}
