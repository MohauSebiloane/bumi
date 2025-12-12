/* A Bison parser, made by GNU Bison 3.7.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_EXPR_TAB_H_INCLUDED
# define YY_YY_EXPR_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    IDENTIFIER = 258,              /* IDENTIFIER  */
    INTLIT = 259,                  /* INTLIT  */
    FLOATLIT = 260,                /* FLOATLIT  */
    STRINGLIT = 261,               /* STRINGLIT  */
    CLASSTK = 262,                 /* CLASSTK  */
    VOIDTK = 263,                  /* VOIDTK  */
    INTTK = 264,                   /* INTTK  */
    FLOATTK = 265,                 /* FLOATTK  */
    BOOLTK = 266,                  /* BOOLTK  */
    STRINGTK = 267,                /* STRINGTK  */
    IFTK = 268,                    /* IFTK  */
    ELSETK = 269,                  /* ELSETK  */
    FORTK = 270,                   /* FORTK  */
    WHILETK = 271,                 /* WHILETK  */
    RETURNTK = 272,                /* RETURNTK  */
    NEWTK = 273,                   /* NEWTK  */
    THISTK = 274,                  /* THISTK  */
    EQOP = 275,                    /* EQOP  */
    NEQOP = 276,                   /* NEQOP  */
    LEOP = 277,                    /* LEOP  */
    GEOP = 278,                    /* GEOP  */
    ANDOP = 279,                   /* ANDOP  */
    OROP = 280,                    /* OROP  */
    LTOP = 281,                    /* LTOP  */
    GTOP = 282,                    /* GTOP  */
    NOTOP = 283,                   /* NOTOP  */
    INC = 284,                     /* INC  */
    DEC = 285,                     /* DEC  */
    ADDOP = 286,                   /* ADDOP  */
    MULTOP = 287,                  /* MULTOP  */
    ASSIGNOP = 288,                /* ASSIGNOP  */
    DOT = 289,                     /* DOT  */
    COMMA = 290,                   /* COMMA  */
    SEMICOL = 291,                 /* SEMICOL  */
    LPAREN = 292,                  /* LPAREN  */
    RPAREN = 293,                  /* RPAREN  */
    LBRACE = 294,                  /* LBRACE  */
    RBRACE = 295,                  /* RBRACE  */
    LBRACK = 296,                  /* LBRACK  */
    RBRACK = 297                   /* RBRACK  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 21 "expr.y"
 /*SEMANTIC RECORD */
	char *val; 

#line 110 "expr.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_EXPR_TAB_H_INCLUDED  */
