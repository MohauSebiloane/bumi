%{
    #include <stdio.h>
    #include "expr.tab.h"

    #define YY_SKIP_YYWRAP
    int yywrap(void) { return 1; }

    int lineN = 1;

    void printToken(const char* name, const char* text) {
        printf("%4d  %-15s  %s\n", lineN, name, text); // right aligned 4 char int, left aligned 15 char string, another string
    }
%}

ID       [a-z|A-Z][a-zA-Z0-9]*
INTTK    [0-9]+
FLOATTK  ([0-9]+\.[0-9]+)|([0-9]+\.)
STRINGTK \"([^\"\\]|\\.)*\"
ADDOP    [+-]
MULTOP   [*/%]

%%

\n                  { lineN++; }
[ \t\r]+            { /* skip whitespace, but not endls yet... */ }
"//"[^\n]*          { /* skip comments */ }

"class"             { printToken("CLASSTK", yytext); }
"void"              { printToken("VOIDTK", yytext); }
"int"               { printToken("INTTK", yytext); }
"float"             { printToken("FLOATTK", yytext); }
"bool"              { printToken("BOOLTK", yytext); }
"string"            { printToken("STRINGTK", yytext); }

"if"                { printToken("IFTK", yytext); }
"else"              { printToken("ELSETK", yytext); }
"for"               { printToken("FORTK", yytext); }
"while"             { printToken("WHILETK", yytext); }
"return"            { printToken("RETURNTK", yytext); }

"new"               { printToken("NEWTK", yytext); }
"this"              { printToken("THISTK", yytext); }
"printf"            { printToken("PRINTTK", yytext); }

{ID}                { printToken("IDENTIFIER", yytext); }

{INTTK}             { printToken("INTTK", yytext); }
{FLOATTK}           { printToken("FLOATTK", yytext); }
{STRINGTK}          { printToken("STRINGTK", yytext); }

"=="                { printToken("EQOP", yytext); }
"!="                { printToken("NEQOP", yytext); }
"<="                { printToken("LEOP", yytext); }
">="                { printToken("GEOP", yytext); }
"&&"                { printToken("ANDOP", yytext); }
"||"                { printToken("OROP", yytext); }

"<"                 { printToken("LTOP", yytext); }
">"                 { printToken("GTOP", yytext); }
"="                 { printToken("ASSIGNOP", yytext); }
"!"                 { printToken("NOTOP", yytext); }

{ADDOP}             { printToken("ADDOP", yytext); }
{MULTOP}            { printToken("MULTOP", yytext); }

"."                 { printToken("DOT", yytext); }
","                 { printToken("COMMA", yytext); }
";"                 { printToken("SEMICOL", yytext); }
"("                 { printToken("LPAREN", yytext); }
")"                 { printToken("RPAREN", yytext); }
"{"                 { printToken("LBRACE", yytext); }
"}"                 { printToken("RBRACE", yytext); }
"["                 { printToken("LBRACK", yytext); }
"]"                 { printToken("RBRACK", yytext); }

.                   { printToken("IDK", yytext); }

%%

int main(int argc, char** argv) {
    if (argc > 1) {
        FILE* f = fopen(argv[1], "r");
        if (!f) {
            fprintf(stderr, "Could not open %s\n", argv[1]);
            return 1;
        }
        yyin = f;
    }

    printf(" line  token           keyword type\n");
    printf("------------------------------------\n");

    while (yylex() != 0) {
        /* whatev */
    }

    return 0;
}
