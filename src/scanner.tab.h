/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

#ifndef YY_YY_SCANNER_TAB_H_INCLUDED
# define YY_YY_SCANNER_TAB_H_INCLUDED
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
    INT_LITERAL = 258,             /* INT_LITERAL  */
    PUBLIC = 259,                  /* PUBLIC  */
    CLASS = 260,                   /* CLASS  */
    STATIC = 261,                  /* STATIC  */
    VOID = 262,                    /* VOID  */
    BOOLEAN = 263,                 /* BOOLEAN  */
    BYTE = 264,                    /* BYTE  */
    CHAR = 265,                    /* CHAR  */
    DOUBLE = 266,                  /* DOUBLE  */
    FLOAT = 267,                   /* FLOAT  */
    INT = 268,                     /* INT  */
    LONG = 269,                    /* LONG  */
    SHORT = 270,                   /* SHORT  */
    IF = 271,                      /* IF  */
    ELSE = 272,                    /* ELSE  */
    SWITCH = 273,                  /* SWITCH  */
    CASE = 274,                    /* CASE  */
    DEFAULT = 275,                 /* DEFAULT  */
    WHILE = 276,                   /* WHILE  */
    DO = 277,                      /* DO  */
    FOR = 278,                     /* FOR  */
    BREAK = 279,                   /* BREAK  */
    CONTINUE = 280,                /* CONTINUE  */
    RETURN = 281,                  /* RETURN  */
    FINAL = 282,                   /* FINAL  */
    TOKEN_TRUE = 283,              /* TOKEN_TRUE  */
    TOKEN_FALSE = 284,             /* TOKEN_FALSE  */
    NULL_LITERAL = 285,            /* NULL_LITERAL  */
    PLUS = 286,                    /* PLUS  */
    MINUS = 287,                   /* MINUS  */
    MULT = 288,                    /* MULT  */
    DIV = 289,                     /* DIV  */
    MOD = 290,                     /* MOD  */
    EQ = 291,                      /* EQ  */
    NEQ = 292,                     /* NEQ  */
    LT = 293,                      /* LT  */
    LE = 294,                      /* LE  */
    GT = 295,                      /* GT  */
    GE = 296,                      /* GE  */
    AND = 297,                     /* AND  */
    OR = 298,                      /* OR  */
    NOT = 299,                     /* NOT  */
    ASSIGN = 300,                  /* ASSIGN  */
    LBRACE = 301,                  /* LBRACE  */
    RBRACE = 302,                  /* RBRACE  */
    LPAREN = 303,                  /* LPAREN  */
    RPAREN = 304,                  /* RPAREN  */
    LBRACKET = 305,                /* LBRACKET  */
    RBRACKET = 306,                /* RBRACKET  */
    SEMI = 307,                    /* SEMI  */
    COMMA = 308,                   /* COMMA  */
    FLOAT_LITERAL = 309,           /* FLOAT_LITERAL  */
    IDENTIFIER = 310,              /* IDENTIFIER  */
    STRING_LITERAL = 311,          /* STRING_LITERAL  */
    CHAR_LITERAL = 312             /* CHAR_LITERAL  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 13 "scanner.y"

    int intValue;

#line 125 "scanner.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_SCANNER_TAB_H_INCLUDED  */
