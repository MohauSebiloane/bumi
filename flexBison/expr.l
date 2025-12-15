/* BUMI Flex Lexer ------------------------------------------------------------ */
/* Author: Colin Fairborn                                                       */
/* Date:   12/11/2025                                                           */
/* Citation: - CODE ADAPTED FROM EX04, authors: Dr. Pete Tucker, Colin Fairborn.*/
/* ---------------------------------------------------------------------------- */

%{
    #include <stdio.h>      // printf() capabilities
    #include <string.h>     // for copying strings into yylval
    #include "expr.tab.h"   // token defs and yylval type from Bison

    #define YY_SKIP_YYWRAP
    int yywrap(void) { return 1; }

    int lineN = 1;

%}

/* Regex macro defininitions */
ID       [a-zA-Z][a-zA-Z0-9]*
INTLIT    [0-9]+
FLOATLIT  ([0-9]+\.[0-9]+)|([0-9]+\.)
STRINGLIT \"([^\"\\]|\\.)*\"
ADDOP    [+-]
MULTOP   [*/%]

%%
\n                  { lineN++; } /* count newlines */
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
"printf" {
    yylval.val = strdup(yytext);    // store text
    return(IDENTIFIER);             // return token type to parser
}

{ID} {
    yylval.val = (char*) strdup(yytext);
    return(IDENTIFIER);
}
{INTLIT}    { 
    yylval.val = strdup(yytext);
    return(INTLIT);    
}
{FLOATLIT}  { 
    yylval.val = strdup(yytext);
    return(FLOATLIT);  
}
{STRINGLIT} { 
    yylval.val = strdup(yytext);
    return(STRINGLIT); 
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
"++"                { return(INC);      } // printToken("DOT", yytext);     
"--"                { return(DEC);      } // printToken("DOT", yytext);   

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

. { printf( "Unknown: '%s'\n", yytext ); } // Print unknown

%%