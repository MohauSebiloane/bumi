%{
    #include <stdio.h>
    #include <string.h>
    #include "expr.tab.h"

    #define YY_SKIP_YYWRAP
    int yywrap(void) { return 1; }

    int lineN = 1;

    void printToken(const char* name, const char* text) {
        printf("%4d  %-15s  %s\n", lineN, name, text); // right aligned 4 char int, left aligned 15 char string, another string
    }
%}

ID       [a-zA-Z][a-zA-Z0-9]*
INTTK    [0-9]+
FLOATTK  ([0-9]+\.[0-9]+)|([0-9]+\.)
STRINGTK \"([^\"\\]|\\.)*\"
ADDOP    [+-]
MULTOP   [*/%]

%%

\n                  { lineN++; }
[ \t\r]+            { /* skip whitespace, but not endls yet... */ }
"//"[^\n]*          { /* skip comments */ }

"class"             { return(CLASSTK);  } // printToken("CLASSTK", yytext);
"void"              { return(VOIDTK);   } // printToken("VOIDTK", yytext); 
"int"               { return(INTTK);    } // printToken("INTTK", yytext);  
"float"             { return(FLOATTK);  } // printToken("FLOATTK", yytext);
"bool"              { return(BOOLTK);   } // printToken("BOOLTK", yytext); 
"string"            { return(STRINGTK); } // printToken("STRINGTK", yytext)

"if"                { return(IFTK);     } // printToken("IFTK", yytext);   
"else"              { return(ELSETK);   } // printToken("ELSETK", yytext); 
"for"               { return(FORTK);    } // printToken("FORTK", yytext);  
"while"             { return(WHILETK);  } // printToken("WHILETK", yytext);
"return"            { return(RETURNTK); } // printToken("RETURNTK", yytext)

"new"               { return(NEWTK);    } // printToken("NEWTK", yytext);  
"this"              { return(THISTK);   } // printToken("THISTK", yytext); 
"printf"            { return(PRINTTK);  } // printToken("PRINTTK", yytext);

{ID} {
    yylval.val = (char*) strdup(yytext);
    return(IDENTIFIER);
}
{INTTK} {
    yylval.val = (char*) strdup(yytext);
    return(INTTK);
}
{FLOATTK} {
    yylval.val = (char*) strdup(yytext);
    return(FLOATTK);
}
{STRINGTK} {
    yylval.val = (char*) strdup(yytext);
    return(STRINGTK);
}

"=="                { return(EQOP);     } // printToken("EQOP", yytext);    
"!="                { return(NEQOP);    } // printToken("NEQOP", yytext);   
"<="                { return(LEOP);     } // printToken("LEOP", yytext);    
">="                { return(GEOP);     } // printToken("GEOP", yytext);    
"&&"                { return(ANDOP);    } // printToken("ANDOP", yytext);   
"||"                { return(OROP);     } // printToken("OROP", yytext);    

"<"                 { return(LTOP);     } // printToken("LTOP", yytext);    
">"                 { return(GTOP);     } // printToken("GTOP", yytext);    
"="                 { return(ASSIGNOP); } // printToken("ASSIGNOP", yytext);
"!"                 { return(NOTOP);    } // printToken("NOTOP", yytext);   

{ADDOP}             { return(ADDOP);    } // printToken("ADDOP", yytext);   
{MULTOP}            { return(MULTOP);   } // printToken("MULTOP", yytext);  

"."                 { return(DOT);      } // printToken("DOT", yytext);     
","                 { return(COMMA);    } // printToken("COMMA", yytext);   
";"                 { return(SEMICOL);  } // printToken("SEMICOL", yytext); 
"("                 { return(LPAREN);   } // printToken("LPAREN", yytext);  
")"                 { return(RPAREN);   } // printToken("RPAREN", yytext);  
"{"                 { return(LBRACE);   } // printToken("LBRACE", yytext);  
"}"                 { return(RBRACE);   } // printToken("RBRACE", yytext);  
"["                 { return(LBRACK);   } // printToken("LBRACK", yytext);  
"]"                 { return(RBRACK);   } // printToken("RBRACK", yytext);  

. { printf( "Unknown: '%s'\n", yytext ); }

%%