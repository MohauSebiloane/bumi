%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbols.h"

#define YYDEBUG 1

int yylex (void);
int yyerror(char* s);

%}

%union semrec { /*SEMANTIC RECORD */
	char *val; 
}

%start program

%token <val> IDENTIFIER
%token <val> INTTK
%token <val> FLOATTK
%token <val> STRINGTK

%token CLASSTK VOIDTK INTTK FLOATTK BOOLTK STRINGTK
%token IFTK ELSETK FORTK WHILETK RETURNTK
%token NEWTK THISTK PRINTTK
%token EQOP NEQOP LEOP GEOP ANDOP OROP
%token LTOP GTOP NOTOP
%token ADDOP MULTOP ASSIGNOP
%token DOT COMMA SEMICOL
%token LPAREN RPAREN LBRACE RBRACE LBRACK RBRACK

%%
-- NEED: varDec, paramList, paramListOPtion, stmtList

program : decList END;
decList : | decList declared;
declared: class | function;

classDec: CLASSTK IDENTIFIER LBRACE classCont RBRACE optSemicol;    -- class LABEL { content };
classCont: | classCont classMem;                                    -- content: variables and methods
classMem: varDec | methDec;                                         -- variable declaration or method declaration

methDec: typeOrVoid IDENTIFIER LPAREN paramListOption RPAREN LBRACE stmtList RBRACE;

typeOrVoid: types | VOIDTK; 
types: INTTK | FLOATTK | BOOLTK | STRINGTK | IDENTIFIER;

optSemicol: | SEMICOL;

%%

int yyerror(char* s) {
    printf("error: %s\n", s);
    return 0;
}

int main(int argc, char**argv) {
    extern FILE* yyin;

    if (argc == 2) {
        yyin = fopen(argv[1], "r");
        if (yyin == NULL) {
            printf("%s doesn't exist\n", argv[1]);
            return -1;
        }
    }
    yyparse();

    printf("compiling...\n");
    -- call compiler function :-)
    
}