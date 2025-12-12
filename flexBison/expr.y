// BUMI Bison Grammar //////////////////////
// Author: Colin Fairborn 
// Date:   12/12/2025
// Citation:    - ChatGPT 5.2, accessed 12/11/2025: Suggested I add inc/dec after I asked it about a compilation error.
//              - Programming Language Pragmatics, 4th Edition, Michael L. Scott: Relative precedence order.
//              - CODE ADAPTED FROM EX04, authors: Dr. Pete Tucker, Colin Fairborn.
////////////////////////////////////////////

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define YYDEBUG 1

int yylex (void);
int yyerror(char* s);

%}

%union semrec { /*SEMANTIC RECORD */
	char *val; 
}

%start program

%token <val> IDENTIFIER INTLIT FLOATLIT STRINGLIT       /* almost landed in trouble with bad token names! */
%token CLASSTK VOIDTK INTTK FLOATTK BOOLTK STRINGTK
%token IFTK ELSETK FORTK WHILETK RETURNTK
%token NEWTK THISTK
%token EQOP NEQOP LEOP GEOP ANDOP OROP
%token LTOP GTOP NOTOP
%token INC DEC
%token ADDOP MULTOP ASSIGNOP
%token DOT COMMA SEMICOL
%token LPAREN RPAREN LBRACE RBRACE LBRACK RBRACK

%%

program : decList;
decList : /* empty */ | decList declared;
declared: classDec | funcDec;

classDec: CLASSTK IDENTIFIER LBRACE classCont RBRACE optSemicol;    /* class LABEL { content }; */
classCont: /* empty */ | classCont classMem;                        /* content: variables and methods */
classMem: varDec | methDec;                                         /* variable declaration or method declaration */

methDec: typeOrVoid IDENTIFIER LPAREN paramListOption RPAREN LBRACE stmtList RBRACE;
funcDec: typeOrVoid IDENTIFIER LPAREN paramListOption RPAREN LBRACE stmtList RBRACE;

typeOrVoid: types | VOIDTK; 
types: INTTK | FLOATTK | BOOLTK | STRINGTK | IDENTIFIER;

optSemicol: /* empty */ | SEMICOL;

paramListOption: /* empty */ | paramList;
paramList: param | paramList COMMA param;
param: types IDENTIFIER;
varDec: types IDENTIFIER SEMICOL | types IDENTIFIER ASSIGNOP expression SEMICOL; /* expression has a defined value */

stmtList: /* empty */ | stmtList statement;
statement: varDec | exprStmt | ifStmt | whileStmt | forStmt | returnStmt | block;

block: LBRACE stmtList RBRACE;
exprStmt: expression SEMICOL;
returnStmt: RETURNTK SEMICOL | RETURNTK expression SEMICOL;

ifStmt: 
    IFTK LPAREN expression RPAREN statement 
    | IFTK LPAREN expression RPAREN statement ELSETK statement
;

whileStmt: WHILETK LPAREN expression RPAREN statement;

forStmt: FORTK LPAREN forInit forCond forIter RPAREN statement;
forInit: varDec | expression SEMICOL | SEMICOL;
forCond: expression SEMICOL | SEMICOL;
forIter: /* empty */ | expression;

/* expressions */
/* assignment = lowest precedence, right-associative */
expression: assignmentExpr;
assignmentExpr: logicalOrExpr | lvalue ASSIGNOP assignmentExpr;

/* things that can appear on the left of = */
lvalue: IDENTIFIER | accessExpr DOT IDENTIFIER;

/* EQOP ANDOP OROP */
logicalOrExpr: logicalAndExpr | logicalOrExpr OROP logicalAndExpr;  /* next checks for && */
logicalAndExpr: equalityExpr | logicalAndExpr ANDOP equalityExpr;   /* next == */
equalityExpr: relationExpr | equalityExpr EQOP relationExpr | equalityExpr NEQOP relationExpr;  /* next is <, >, <=, ... */

/* LEOP GEOP LTOP GTOP */
relationExpr: 
    addOper
    | relationExpr LEOP addOper
    | relationExpr GEOP addOper
    | relationExpr LTOP addOper
    | relationExpr GTOP addOper
;

addOper: mulOper | addOper ADDOP mulOper;
mulOper: unaryExpr | mulOper MULTOP unaryExpr;

unaryExpr:
    postfixExpr
    | NOTOP unaryExpr
    | ADDOP unaryExpr
;

// Adapted code, first SUGGESTED BY ChatGPT, 12/11/2025:
postfixExpr: 
    accessExpr
    | postfixExpr INC
    | postfixExpr DEC
;
//// end citation /////

accessExpr:
    plain
    | accessExpr DOT IDENTIFIER
    | accessExpr DOT IDENTIFIER LPAREN argListOpt RPAREN
    | accessExpr LPAREN argListOpt RPAREN
;

plain: 
    literal
    | IDENTIFIER
    | THISTK
    | NEWTK IDENTIFIER LPAREN argListOpt RPAREN
    | LPAREN expression RPAREN
;

argListOpt: /* empty */ | argList;
argList: expression | argList COMMA expression;

literal: INTLIT | FLOATLIT | STRINGLIT;

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
    /* call compiler function :-) */
    
}