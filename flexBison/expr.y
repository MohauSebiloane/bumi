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

program : stmt_list END;

stmt_list : stmt | stmt_list stmt;

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