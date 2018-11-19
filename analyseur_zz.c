//#include "analyseur_zz.h"

#ifndef TABSYMBOL
#include "tabsymb.h"
#endif

 
typetoken token;
bool follow_token = false;
symb symbol;

typetoken read_token(){
	if(follow_token){
		follow_token = false;
		return token;
	}
	return (typetoken) yylex();
}


bool _type(){
	bool result = false;
	if(token == INT || token == DOUBLE || token == BOOL){
		result = true;
	}
	return result;
}

//const : inumber|dnumber|true|false

bool _const(){
	bool result = false;
	if(token == INUMBER || token == DNUMBER || token == TRUE || token == FALSE){
		result = true;
	}
	return result;
}

//_decl_aux : const';'|';' 

bool _decl_aux(){
	bool result = false;
	if(token == PVIRG){
		result = true;
		symbol.init = false;
		if(!add_symbol(symbol.name, symbol.type, symbol.init, symbol.val_init, symbol.line))
			add_smerror(symbol.name, symbol.line, already_declared);
	}
	else if(_const()){
		symbol.init = true;
		if (!check_type(symbol.type, token))
      add_smerror(symbol.name, symbol.line, bad_initialized);
		else {
			if (!strcasecmp(symbol.type, "bool")) {
        if (!strcasecmp(yytext, "true")) {
          symbol.val_init = 1.0;
        } else {
          symbol.val_init = 0.0;
        }
      } 
			else {
        symbol.val_init = atof(yytext);
      }
		}
		token = read_token();
		if(token == PVIRG){
			result = true;
			if(add_symbol(symbol.name, symbol.type, symbol.init, symbol.val_init, symbol.line))
				add_smerror(symbol.name, symbol.line, already_declared);
		}
	}
	return result;
}

//DECL : idf TYPE DECL_AUX

bool _decl(){
	bool result = false;
	if(token == IDF){
		symbol.name = (char *) malloc((strlen(yytext) + 1) * sizeof(char));
    strcpy(symbol.name, yytext);
    symbol.line = yylineno;
		token = read_token();	
		if(_type()){
			symbol.type = (char *) malloc((strlen(yytext) + 1) * sizeof(char));
      strcpy(symbol.type, yytext);
			token = read_token();
			if(_decl_aux()){
				result = true;			
			}
		}
	}
	return result;
}

//list_DECL : DECL list_DECLAUX

bool _list_decl(){
	bool result = false;
	if(_decl()){
		token = read_token();
		if(_list_decl_aux()){
			result = true;		
		}	
	}
	return result;
}

//list_DECLAUX : list_DECL | epsilon
//follow : BEGIN

bool _list_decl_aux(){
	bool result = false;
	if(token == BEG_IN){
		follow_token = true;
		result = true;
	}
	else if(_list_decl()){
		result =  true;
	}
	return result;
}

//PROG : list_DECL begin list_INST end

bool _prog(){
	bool result = false;
	if(_list_decl()){
		token = read_token();
		if(token == BEG_IN){
			token = read_token();
			if(_list_inst()){
				token = read_token();
				if(token == END){
					result = true;
				}
			}
		}
	}
	return result;
}

/*
INST : idf = CONST ';' ==> INST : idf = ADDSUB ';' | true ';' | false ';'
| if ‘(‘ idf == CONST ‘)’ then list_INST IF_INSTAUX ==> if ‘(‘ idf == ADDSUB ‘)’ then list_INST IF_INSTAUX
| print IDF ';'
| for IDF = inumber to inumber do list_INST endfor
*/
bool _inst(){
	bool result = false;
	if(token == IDF){
		token = read_token();
		if(token == EQ){
			token = read_token();
			if(_addsub()){
				token = read_token();
				if(token == PVIRG){
					result = true;
				}
			}
			else if(token == TRUE){
				token = read_token();
				if(token == PVIRG){
					result = true;
				}
			}
			else if(token == FALSE){
				token = read_token();
				if(token == PVIRG){
					result = true;
				}
			}
			
		}
	}
	else if(token == IF){
		token = read_token();
		if(token == P_OPEN){
			token = read_token();
			if(token == IDF){
				token = read_token();
				if(token == EQEQ){
					token = read_token();
					if(_addsub()){
						token = read_token();
						if(token == P_CLOSE){
							token = read_token();
							if(token == THEN){
								token = read_token();
								if(_list_inst()){
									token = read_token();
									if(_if_inst_aux()){
										result = true;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	else if(token == PRINT){
		token = read_token();
		if(token == IDF){
			token = read_token();
			if(token == PVIRG){
				result = true;
			}
		}
	}
	else if(token == FOR){
		token = read_token();
		if(token == IDF){
			token = read_token();
			if (token == EQ){
				token = read_token();
				if(token == INUMBER){
					token = read_token();
					if(token == TO){
						token = read_token();
						if(token == INUMBER){
							token = read_token();
							if(token == DO){
								token = read_token();
								if(_list_inst()){
									token = read_token();
									if(token == ENDFOR){
										result = true;
									}
								}
							}
						}
					}
				}	
			}
		}
	}
	return result ;
}

//IF_INSTAUX : endif | else list_INST endif

bool _if_inst_aux(){
	bool result = false;
	if(token == ENDIF){
		result = true;
	}
	else if(token == ELSE){
		token = read_token();
		if(_list_inst()){
			token = read_token();
			if(token == ENDIF){
				result = true;
			}
		}
	}

	return result;

}

//list_INST : INST list_INSTAUX

bool _list_inst(){
	bool result = false;
	if(_inst()){
		token = read_token();
		if(_list_inst_aux()){
			result = true;
		}
	}
	return result;
}


//list_INSTAUX : list_INST | epsilon
//NULLABLE 
//follow : END, ELSE, ENDFOR, ENDIF  

bool _list_inst_aux(){
	bool result = false;
	if(token == END || token == ENDIF || token == ENDFOR || token == ELSE){
		follow_token = true;
		result = true;
	}
	else if(_list_inst()){
		result = true;
	}
	return result;
}

//_addsub : MULTDIV ADDSUBAUX

bool _addsub(){
	bool result = false;
	if(_multdiv()){
		token = read_token();
		if(_addsubaux()){
			result = true;
		}
	}
	return result;
}

//ADDSUBAUX : - MULTDIV ADDSUBAUX | + MULTDIV ADDSUBAUX | epsilon
//NULLABLE
//follow : PCLOSE, PVIRG, POPEN

bool _addsubaux(){
	bool result = false;
	if((token == PVIRG) || (token == P_CLOSE)){
		follow_token = true;
		result = true;
	}
	else if(token == MINUS){
		token = read_token();
		if(_multdiv()){
			token = read_token();
			if(_addsubaux()){
				result = true;
			}
		}
	}
	else if(token == PLUS){
		token = read_token();
		if(_multdiv()){
			token = read_token();
			if(_addsubaux()){
				result = true;
			}
		}
	}
	return result;
}

//MULTDIVAUX : * AUX  MULTDIVAUX | / AUX  MULTDIVAUX | epsilon
//NULLABLE 
//follow : PVIRG POPEN PCLOSE PLUS MINUS

bool _multdivaux(){
	bool result = false;
	if((token == PLUS) || (token == MINUS) || (token == PVIRG) || (token == P_CLOSE)){
		follow_token = true;
		result = true;
	}
	else if(token == MULT){
		token = read_token();
		if(_aux()){
			token = read_token();
			if(_multdivaux()){
				result = true;
			}
		}
	}
	else if(token == DIV){
		token = read_token();
		if(_aux()){
			token = read_token();
			if(_multdivaux()){
				result = true;
			}
		}
	}
	return result;
}


//AUX : idf | inumber | dnumber | ( ADDSUB )

bool _aux(){
	bool result;
	if(token == IDF || token == INUMBER || token == DNUMBER){
		result = true;
	}
	else if(token == P_OPEN){
		token = read_token();
		if(_addsub()){
			token = read_token();
			if(token == P_CLOSE){
				result = true;
			}		
		}
	}
	return result;
}

//MULTDIV : AUX MULTDIVAUX

bool _multdiv(){
	bool result = false;
	if(_aux){
		token = read_token();
		if(_multdivaux()){
			result = true;		
		}
	}
	return result;
}

int main(int argc, char * argv){
	//init_list_symbol();
	//init_list_error();
	token = read_token();
	if (_prog()) {
		printf("good execution\n") ;
	}
	else {
		printf("bad execution\n");
	}
	show_smerror();
	return 0;
}
