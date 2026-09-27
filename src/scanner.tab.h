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
    IDENTIFIER = 258,              /* IDENTIFIER  */
    STRING_LITERAL = 259,          /* STRING_LITERAL  */
    CHAR_LITERAL = 260,            /* CHAR_LITERAL  */
    FLOAT_LITERAL = 261,           /* FLOAT_LITERAL  */
    INT_LITERAL = 262,             /* INT_LITERAL  */
    PUBLIC = 263,                  /* PUBLIC  */
    CLASS = 264,                   /* CLASS  */
    STATIC = 265,                  /* STATIC  */
    VOID = 266,                    /* VOID  */
    BOOLEAN = 267,                 /* BOOLEAN  */
    BYTE = 268,                    /* BYTE  */
    CHAR = 269,                    /* CHAR  */
    DOUBLE = 270,                  /* DOUBLE  */
    FLOAT = 271,                   /* FLOAT  */
    INT = 272,                     /* INT  */
    LONG = 273,                    /* LONG  */
    SHORT = 274,                   /* SHORT  */
    STRING_TYPE = 275,             /* STRING_TYPE  */
    IF = 276,                      /* IF  */
    ELSE = 277,                    /* ELSE  */
    SWITCH = 278,                  /* SWITCH  */
    CASE = 279,                    /* CASE  */
    DEFAULT = 280,                 /* DEFAULT  */
    WHILE = 281,                   /* WHILE  */
    DO = 282,                      /* DO  */
    FOR = 283,                     /* FOR  */
    BREAK = 284,                   /* BREAK  */
    CONTINUE = 285,                /* CONTINUE  */
    RETURN = 286,                  /* RETURN  */
    FINAL = 287,                   /* FINAL  */
    TOKEN_TRUE = 288,              /* TOKEN_TRUE  */
    TOKEN_FALSE = 289,             /* TOKEN_FALSE  */
    NULL_LITERAL = 290,            /* NULL_LITERAL  */
    PLUS = 291,                    /* PLUS  */
    MINUS = 292,                   /* MINUS  */
    MULT = 293,                    /* MULT  */
    DIV = 294,                     /* DIV  */
    MOD = 295,                     /* MOD  */
    INC = 296,                     /* INC  */
    DEC = 297,                     /* DEC  */
    EQ = 298,                      /* EQ  */
    NEQ = 299,                     /* NEQ  */
    LT = 300,                      /* LT  */
    LE = 301,                      /* LE  */
    GT = 302,                      /* GT  */
    GE = 303,                      /* GE  */
    AND = 304,                     /* AND  */
    OR = 305,                      /* OR  */
    NOT = 306,                     /* NOT  */
    ASSIGN = 307,                  /* ASSIGN  */
    LBRACE = 308,                  /* LBRACE  */
    RBRACE = 309,                  /* RBRACE  */
    LPAREN = 310,                  /* LPAREN  */
    RPAREN = 311,                  /* RPAREN  */
    LBRACKET = 312,                /* LBRACKET  */
    RBRACKET = 313,                /* RBRACKET  */
    SEMI = 314,                    /* SEMI  */
    COMMA = 315,                   /* COMMA  */
    PRINTLN = 316,                 /* PRINTLN  */
    PRINT = 317,                   /* PRINT  */
    MATH_SQRT = 318,               /* MATH_SQRT  */
    MATH_POW = 319                 /* MATH_POW  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 14 "scanner.y"

    int intValue;
    float floatValue;
    char charValue;
    char* stringValue;


#line 136 "scanner.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_SCANNER_TAB_H_INCLUDED  */
