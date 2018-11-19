#ifndef ANALYSEUR_ZZ
#define ANALUSEUR_ZZ

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef ERROR
#include "error.h"
#endif


typedef enum{
IDF, INUMBER, DNUMBER, INT, DOUBLE, BOOL, TRUE, FALSE, PVIRG, BEG_IN, IF, ELSE, THEN, ENDIF, FOR, ENDFOR, DO, WHILE, ENDWHILE, REM, PRINT, PLUS, MINUS, MULT, DIV, EQEQ, EQ, P_OPEN, P_CLOSE,
END, INF, SUP, INF_EQ, SUP_EQ, TO
}typetoken;


extern int yylex();
extern int yylineno;
extern char* yytext;


bool _prog();
bool _type();
bool _decl_aux();
bool _const();
bool _decl();
bool _list_decl();
bool _list_decl_aux();
bool _list_inst();
bool _list_inst_aux();
bool _inst();
bool _if_inst_aux();
bool _addsub();
bool _addsubaux();
bool _multdiv();
bool _multdivaux();
bool _aux();


typetoken read_token();

#endif
