/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "jtoc.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylex();
extern int yylineno;
extern char* yytext;
extern FILE *yyin;
extern int erros_lexicos;
int yywrap(void);
void yyerror(const char *s);

#line 85 "jtoc.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "jtoc.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_IDENTIFIER = 3,                 /* IDENTIFIER  */
  YYSYMBOL_STRING_LITERAL = 4,             /* STRING_LITERAL  */
  YYSYMBOL_CHAR_LITERAL = 5,               /* CHAR_LITERAL  */
  YYSYMBOL_FLOAT_LITERAL = 6,              /* FLOAT_LITERAL  */
  YYSYMBOL_INT_LITERAL = 7,                /* INT_LITERAL  */
  YYSYMBOL_PUBLIC = 8,                     /* PUBLIC  */
  YYSYMBOL_CLASS = 9,                      /* CLASS  */
  YYSYMBOL_STATIC = 10,                    /* STATIC  */
  YYSYMBOL_VOID = 11,                      /* VOID  */
  YYSYMBOL_BOOLEAN = 12,                   /* BOOLEAN  */
  YYSYMBOL_BYTE = 13,                      /* BYTE  */
  YYSYMBOL_CHAR = 14,                      /* CHAR  */
  YYSYMBOL_DOUBLE = 15,                    /* DOUBLE  */
  YYSYMBOL_FLOAT = 16,                     /* FLOAT  */
  YYSYMBOL_INT = 17,                       /* INT  */
  YYSYMBOL_LONG = 18,                      /* LONG  */
  YYSYMBOL_SHORT = 19,                     /* SHORT  */
  YYSYMBOL_STRING_TYPE = 20,               /* STRING_TYPE  */
  YYSYMBOL_IF = 21,                        /* IF  */
  YYSYMBOL_ELSE = 22,                      /* ELSE  */
  YYSYMBOL_SWITCH = 23,                    /* SWITCH  */
  YYSYMBOL_CASE = 24,                      /* CASE  */
  YYSYMBOL_DEFAULT = 25,                   /* DEFAULT  */
  YYSYMBOL_WHILE = 26,                     /* WHILE  */
  YYSYMBOL_DO = 27,                        /* DO  */
  YYSYMBOL_FOR = 28,                       /* FOR  */
  YYSYMBOL_BREAK = 29,                     /* BREAK  */
  YYSYMBOL_CONTINUE = 30,                  /* CONTINUE  */
  YYSYMBOL_RETURN = 31,                    /* RETURN  */
  YYSYMBOL_FINAL = 32,                     /* FINAL  */
  YYSYMBOL_TOKEN_TRUE = 33,                /* TOKEN_TRUE  */
  YYSYMBOL_TOKEN_FALSE = 34,               /* TOKEN_FALSE  */
  YYSYMBOL_NULL_LITERAL = 35,              /* NULL_LITERAL  */
  YYSYMBOL_PLUS = 36,                      /* PLUS  */
  YYSYMBOL_MINUS = 37,                     /* MINUS  */
  YYSYMBOL_MULT = 38,                      /* MULT  */
  YYSYMBOL_DIV = 39,                       /* DIV  */
  YYSYMBOL_MOD = 40,                       /* MOD  */
  YYSYMBOL_INC = 41,                       /* INC  */
  YYSYMBOL_DEC = 42,                       /* DEC  */
  YYSYMBOL_EQ = 43,                        /* EQ  */
  YYSYMBOL_NEQ = 44,                       /* NEQ  */
  YYSYMBOL_LT = 45,                        /* LT  */
  YYSYMBOL_LE = 46,                        /* LE  */
  YYSYMBOL_GT = 47,                        /* GT  */
  YYSYMBOL_GE = 48,                        /* GE  */
  YYSYMBOL_AND = 49,                       /* AND  */
  YYSYMBOL_OR = 50,                        /* OR  */
  YYSYMBOL_NOT = 51,                       /* NOT  */
  YYSYMBOL_ASSIGN = 52,                    /* ASSIGN  */
  YYSYMBOL_LBRACE = 53,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 54,                    /* RBRACE  */
  YYSYMBOL_LPAREN = 55,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 56,                    /* RPAREN  */
  YYSYMBOL_LBRACKET = 57,                  /* LBRACKET  */
  YYSYMBOL_RBRACKET = 58,                  /* RBRACKET  */
  YYSYMBOL_SEMI = 59,                      /* SEMI  */
  YYSYMBOL_COMMA = 60,                     /* COMMA  */
  YYSYMBOL_PRINTLN = 61,                   /* PRINTLN  */
  YYSYMBOL_PRINT = 62,                     /* PRINT  */
  YYSYMBOL_MATH_SQRT = 63,                 /* MATH_SQRT  */
  YYSYMBOL_MATH_POW = 64,                  /* MATH_POW  */
  YYSYMBOL_YYACCEPT = 65,                  /* $accept  */
  YYSYMBOL_inicio = 66,                    /* inicio  */
  YYSYMBOL_programa = 67,                  /* programa  */
  YYSYMBOL_comando = 68,                   /* comando  */
  YYSYMBOL_expressao = 69,                 /* expressao  */
  YYSYMBOL_tipo = 70,                      /* tipo  */
  YYSYMBOL_incrementacao = 71              /* incrementacao  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;



/* Unqualified %code blocks.  */
#line 21 "jtoc.y"

struct NoAST {
    char *texto;
    NoAST *filho;
    NoAST *irmao;
};

/* Uma entrada para cada declaracao encontrada no programa. */
typedef struct Simbolo {
    char *simbolo;
    char *tipo;
    char *valor;
    const char *escopo;
    struct Simbolo *proximo;
} Simbolo;

NoAST *criar_no(const char *texto);
NoAST *criar_folha(char *texto);
NoAST *montar_no(const char *texto, NoAST *a, NoAST *b, NoAST *c);
void adicionar_filho(NoAST *pai, NoAST *filho);
void imprimir_ast(NoAST *no, int nivel);
void liberar_ast(NoAST *no);
char *texto_expressao(NoAST *no);
void inserir_simbolo(NoAST *declaracao, int nivel);
void montar_tabela(NoAST *no, int nivel);
void imprimir_tabela(void);
void liberar_tabela(void);

NoAST *raiz = NULL;
Simbolo *tabela_simbolos = NULL;

#line 223 "jtoc.tab.c"

#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   351

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  65
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  7
/* YYNRULES -- Number of rules.  */
#define YYNRULES  45
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  102

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   319


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,   100,   100,   104,   105,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   121,   122,   123,   124,   125,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   138,
     140,   141,   143,   144,   145,   146,   147,   148,   149,   153,
     154,   155,   156,   157,   161,   162
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "IDENTIFIER",
  "STRING_LITERAL", "CHAR_LITERAL", "FLOAT_LITERAL", "INT_LITERAL",
  "PUBLIC", "CLASS", "STATIC", "VOID", "BOOLEAN", "BYTE", "CHAR", "DOUBLE",
  "FLOAT", "INT", "LONG", "SHORT", "STRING_TYPE", "IF", "ELSE", "SWITCH",
  "CASE", "DEFAULT", "WHILE", "DO", "FOR", "BREAK", "CONTINUE", "RETURN",
  "FINAL", "TOKEN_TRUE", "TOKEN_FALSE", "NULL_LITERAL", "PLUS", "MINUS",
  "MULT", "DIV", "MOD", "INC", "DEC", "EQ", "NEQ", "LT", "LE", "GT", "GE",
  "AND", "OR", "NOT", "ASSIGN", "LBRACE", "RBRACE", "LPAREN", "RPAREN",
  "LBRACKET", "RBRACKET", "SEMI", "COMMA", "PRINTLN", "PRINT", "MATH_SQRT",
  "MATH_POW", "$accept", "inicio", "programa", "comando", "expressao",
  "tipo", "incrementacao", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-80)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -80,     4,   108,   -80,   -31,   -80,   -80,   -80,   -80,   -80,
     -50,   -47,   -46,   -33,   -80,    24,   -80,   -80,    12,   -12,
      12,    12,    12,    12,    -8,   -80,   -80,   -80,   -80,   -80,
     -80,   -80,    12,    12,    12,    -7,    -5,   137,   -80,   171,
     186,   201,   216,    12,   -80,   -80,   -80,   231,    12,    12,
      12,    12,    12,    12,    12,    12,    12,    12,    12,    12,
      12,    12,    12,   -80,     0,     2,    -1,     3,   154,   -80,
     246,   112,   -14,   -14,   -80,   -80,   -80,   303,   303,    60,
      60,    60,    60,   290,   276,   -80,   -80,   -80,   -80,   -80,
     -80,    12,    40,    65,   261,    37,   -80,   -80,    11,   -80,
      92,   -80
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       3,     0,     2,     1,     0,    42,    41,    40,    39,    43,
       0,     0,     0,     0,     4,     0,    44,    45,     0,     0,
       0,     0,     0,     0,     0,    32,    36,    35,    33,    34,
      37,    38,     0,     0,     0,     0,     0,     0,    11,     0,
       0,     0,     0,     0,     6,    21,    22,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     7,     0,     0,     0,     0,     0,    29,
       0,     0,    14,    15,    16,    17,    18,    23,    24,    25,
      26,    27,    28,    19,    20,     3,     3,    12,    13,     5,
      30,     0,     0,     0,     0,     9,    10,    31,     0,     3,
       0,     8
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -80,   -80,   -79,   -80,   -20,   -80,   -80
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     1,     2,    14,    37,    15,    19
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      39,    40,    41,    42,     3,    20,    92,    93,    21,    22,
      16,    17,    45,    46,    47,    25,    26,    27,    28,    29,
     100,    18,    23,    68,    52,    53,    54,    24,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,     4,    43,    30,    31,    38,    48,    32,
      49,    44,     5,    85,     6,    86,     7,     8,    87,    98,
       9,    10,    88,    33,    99,     0,    11,    34,     4,     0,
       0,    94,     0,     0,     0,    35,    36,     5,     0,     6,
       0,     7,     8,     0,     0,     9,    10,     0,     0,     0,
       0,    11,     0,     0,    95,     4,    50,    51,    52,    53,
      54,    12,    13,     0,     5,     0,     6,     0,     7,     8,
       0,     4,     9,    10,     0,     0,     0,     0,    11,    96,
       5,     0,     6,     0,     7,     8,    12,    13,     9,    10,
       0,     0,     0,     0,    11,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   101,     0,    50,    51,
      52,    53,    54,    12,    13,    55,    56,    57,    58,    59,
      60,    61,    62,     0,     0,     0,     0,     0,     0,    12,
      13,     0,    91,    50,    51,    52,    53,    54,     0,     0,
      55,    56,    57,    58,    59,    60,    61,    62,     0,     0,
      50,    51,    52,    53,    54,     0,    63,    55,    56,    57,
      58,    59,    60,    61,    62,     0,     0,    50,    51,    52,
      53,    54,     0,    89,    55,    56,    57,    58,    59,    60,
      61,    62,    50,    51,    52,    53,    54,    64,     0,    55,
      56,    57,    58,    59,    60,    61,    62,    50,    51,    52,
      53,    54,    65,     0,    55,    56,    57,    58,    59,    60,
      61,    62,    50,    51,    52,    53,    54,    66,     0,    55,
      56,    57,    58,    59,    60,    61,    62,    50,    51,    52,
      53,    54,    67,     0,    55,    56,    57,    58,    59,    60,
      61,    62,    50,    51,    52,    53,    54,    69,     0,    55,
      56,    57,    58,    59,    60,    61,    62,    50,    51,    52,
      53,    54,    90,     0,    55,    56,    57,    58,    59,    60,
      61,    62,    50,    51,    52,    53,    54,    97,     0,    55,
      56,    57,    58,    59,    60,    61,    50,    51,    52,    53,
      54,     0,     0,    55,    56,    57,    58,    59,    60,    50,
      51,    52,    53,    54,     0,     0,     0,     0,    57,    58,
      59,    60
};

static const yytype_int8 yycheck[] =
{
      20,    21,    22,    23,     0,    55,    85,    86,    55,    55,
      41,    42,    32,    33,    34,     3,     4,     5,     6,     7,
      99,    52,    55,    43,    38,    39,    40,     3,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,     3,    52,    33,    34,    59,    55,    37,
      55,    59,    12,    53,    14,    53,    16,    17,    59,    22,
      20,    21,    59,    51,    53,    -1,    26,    55,     3,    -1,
      -1,    91,    -1,    -1,    -1,    63,    64,    12,    -1,    14,
      -1,    16,    17,    -1,    -1,    20,    21,    -1,    -1,    -1,
      -1,    26,    -1,    -1,    54,     3,    36,    37,    38,    39,
      40,    61,    62,    -1,    12,    -1,    14,    -1,    16,    17,
      -1,     3,    20,    21,    -1,    -1,    -1,    -1,    26,    54,
      12,    -1,    14,    -1,    16,    17,    61,    62,    20,    21,
      -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    54,    -1,    36,    37,
      38,    39,    40,    61,    62,    43,    44,    45,    46,    47,
      48,    49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      62,    -1,    60,    36,    37,    38,    39,    40,    -1,    -1,
      43,    44,    45,    46,    47,    48,    49,    50,    -1,    -1,
      36,    37,    38,    39,    40,    -1,    59,    43,    44,    45,
      46,    47,    48,    49,    50,    -1,    -1,    36,    37,    38,
      39,    40,    -1,    59,    43,    44,    45,    46,    47,    48,
      49,    50,    36,    37,    38,    39,    40,    56,    -1,    43,
      44,    45,    46,    47,    48,    49,    50,    36,    37,    38,
      39,    40,    56,    -1,    43,    44,    45,    46,    47,    48,
      49,    50,    36,    37,    38,    39,    40,    56,    -1,    43,
      44,    45,    46,    47,    48,    49,    50,    36,    37,    38,
      39,    40,    56,    -1,    43,    44,    45,    46,    47,    48,
      49,    50,    36,    37,    38,    39,    40,    56,    -1,    43,
      44,    45,    46,    47,    48,    49,    50,    36,    37,    38,
      39,    40,    56,    -1,    43,    44,    45,    46,    47,    48,
      49,    50,    36,    37,    38,    39,    40,    56,    -1,    43,
      44,    45,    46,    47,    48,    49,    36,    37,    38,    39,
      40,    -1,    -1,    43,    44,    45,    46,    47,    48,    36,
      37,    38,    39,    40,    -1,    -1,    -1,    -1,    45,    46,
      47,    48
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    66,    67,     0,     3,    12,    14,    16,    17,    20,
      21,    26,    61,    62,    68,    70,    41,    42,    52,    71,
      55,    55,    55,    55,     3,     3,     4,     5,     6,     7,
      33,    34,    37,    51,    55,    63,    64,    69,    59,    69,
      69,    69,    69,    52,    59,    69,    69,    69,    55,    55,
      36,    37,    38,    39,    40,    43,    44,    45,    46,    47,
      48,    49,    50,    59,    56,    56,    56,    56,    69,    56,
      69,    69,    69,    69,    69,    69,    69,    69,    69,    69,
      69,    69,    69,    69,    69,    53,    53,    59,    59,    59,
      56,    60,    67,    67,    69,    54,    54,    56,    22,    53,
      67,    54
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    65,    66,    67,    67,    68,    68,    68,    68,    68,
      68,    68,    68,    68,    69,    69,    69,    69,    69,    69,
      69,    69,    69,    69,    69,    69,    69,    69,    69,    69,
      69,    69,    69,    69,    69,    69,    69,    69,    69,    70,
      70,    70,    70,    70,    71,    71
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     5,     3,     4,    11,     7,
       7,     3,     5,     5,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     3,     3,     3,     3,     3,     3,     3,
       4,     6,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  switch (yykind)
    {
    case YYSYMBOL_IDENTIFIER: /* IDENTIFIER  */
#line 64 "jtoc.y"
            { free(((*yyvaluep).stringValue)); }
#line 1030 "jtoc.tab.c"
        break;

    case YYSYMBOL_STRING_LITERAL: /* STRING_LITERAL  */
#line 64 "jtoc.y"
            { free(((*yyvaluep).stringValue)); }
#line 1036 "jtoc.tab.c"
        break;

    case YYSYMBOL_CHAR_LITERAL: /* CHAR_LITERAL  */
#line 64 "jtoc.y"
            { free(((*yyvaluep).stringValue)); }
#line 1042 "jtoc.tab.c"
        break;

    case YYSYMBOL_FLOAT_LITERAL: /* FLOAT_LITERAL  */
#line 64 "jtoc.y"
            { free(((*yyvaluep).stringValue)); }
#line 1048 "jtoc.tab.c"
        break;

    case YYSYMBOL_INT_LITERAL: /* INT_LITERAL  */
#line 64 "jtoc.y"
            { free(((*yyvaluep).stringValue)); }
#line 1054 "jtoc.tab.c"
        break;

    case YYSYMBOL_programa: /* programa  */
#line 65 "jtoc.y"
            { liberar_ast(((*yyvaluep).no)); }
#line 1060 "jtoc.tab.c"
        break;

    case YYSYMBOL_comando: /* comando  */
#line 65 "jtoc.y"
            { liberar_ast(((*yyvaluep).no)); }
#line 1066 "jtoc.tab.c"
        break;

    case YYSYMBOL_expressao: /* expressao  */
#line 65 "jtoc.y"
            { liberar_ast(((*yyvaluep).no)); }
#line 1072 "jtoc.tab.c"
        break;

    case YYSYMBOL_tipo: /* tipo  */
#line 65 "jtoc.y"
            { liberar_ast(((*yyvaluep).no)); }
#line 1078 "jtoc.tab.c"
        break;

    case YYSYMBOL_incrementacao: /* incrementacao  */
#line 65 "jtoc.y"
            { liberar_ast(((*yyvaluep).no)); }
#line 1084 "jtoc.tab.c"
        break;

      default:
        break;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* inicio: programa  */
#line 100 "jtoc.y"
               { raiz = (yyvsp[0].no); }
#line 1354 "jtoc.tab.c"
    break;

  case 3: /* programa: %empty  */
#line 104 "jtoc.y"
             { (yyval.no) = criar_no("Bloco"); }
#line 1360 "jtoc.tab.c"
    break;

  case 4: /* programa: programa comando  */
#line 105 "jtoc.y"
                       { adicionar_filho((yyvsp[-1].no), (yyvsp[0].no)); (yyval.no) = (yyvsp[-1].no); }
#line 1366 "jtoc.tab.c"
    break;

  case 5: /* comando: tipo IDENTIFIER ASSIGN expressao SEMI  */
#line 109 "jtoc.y"
                                           { (yyval.no) = montar_no("Declaracao", (yyvsp[-4].no), criar_folha((yyvsp[-3].stringValue)), (yyvsp[-1].no)); }
#line 1372 "jtoc.tab.c"
    break;

  case 6: /* comando: tipo IDENTIFIER SEMI  */
#line 110 "jtoc.y"
                           { (yyval.no) = montar_no("Declaracao", (yyvsp[-2].no), criar_folha((yyvsp[-1].stringValue)), NULL); }
#line 1378 "jtoc.tab.c"
    break;

  case 7: /* comando: IDENTIFIER ASSIGN expressao SEMI  */
#line 111 "jtoc.y"
                                       { (yyval.no) = montar_no("=", criar_folha((yyvsp[-3].stringValue)), (yyvsp[-1].no), NULL); }
#line 1384 "jtoc.tab.c"
    break;

  case 8: /* comando: IF LPAREN expressao RPAREN LBRACE programa RBRACE ELSE LBRACE programa RBRACE  */
#line 112 "jtoc.y"
                                                                                    { (yyval.no) = montar_no("If", (yyvsp[-8].no), (yyvsp[-5].no), (yyvsp[-1].no)); }
#line 1390 "jtoc.tab.c"
    break;

  case 9: /* comando: IF LPAREN expressao RPAREN LBRACE programa RBRACE  */
#line 113 "jtoc.y"
                                                        { (yyval.no) = montar_no("If", (yyvsp[-4].no), (yyvsp[-1].no), NULL); }
#line 1396 "jtoc.tab.c"
    break;

  case 10: /* comando: WHILE LPAREN expressao RPAREN LBRACE programa RBRACE  */
#line 114 "jtoc.y"
                                                           { (yyval.no) = montar_no("While", (yyvsp[-4].no), (yyvsp[-1].no), NULL); }
#line 1402 "jtoc.tab.c"
    break;

  case 11: /* comando: IDENTIFIER incrementacao SEMI  */
#line 115 "jtoc.y"
                                    { adicionar_filho((yyvsp[-1].no), criar_folha((yyvsp[-2].stringValue))); (yyval.no) = (yyvsp[-1].no); }
#line 1408 "jtoc.tab.c"
    break;

  case 12: /* comando: PRINTLN LPAREN expressao RPAREN SEMI  */
#line 116 "jtoc.y"
                                           { (yyval.no) = montar_no("Println", (yyvsp[-2].no), NULL, NULL); }
#line 1414 "jtoc.tab.c"
    break;

  case 13: /* comando: PRINT LPAREN expressao RPAREN SEMI  */
#line 117 "jtoc.y"
                                         { (yyval.no) = montar_no("Print", (yyvsp[-2].no), NULL, NULL); }
#line 1420 "jtoc.tab.c"
    break;

  case 14: /* expressao: expressao PLUS expressao  */
#line 121 "jtoc.y"
                               { (yyval.no) = montar_no("+", (yyvsp[-2].no), (yyvsp[0].no), NULL); }
#line 1426 "jtoc.tab.c"
    break;

  case 15: /* expressao: expressao MINUS expressao  */
#line 122 "jtoc.y"
                                { (yyval.no) = montar_no("-", (yyvsp[-2].no), (yyvsp[0].no), NULL); }
#line 1432 "jtoc.tab.c"
    break;

  case 16: /* expressao: expressao MULT expressao  */
#line 123 "jtoc.y"
                               { (yyval.no) = montar_no("*", (yyvsp[-2].no), (yyvsp[0].no), NULL); }
#line 1438 "jtoc.tab.c"
    break;

  case 17: /* expressao: expressao DIV expressao  */
#line 124 "jtoc.y"
                              { (yyval.no) = montar_no("/", (yyvsp[-2].no), (yyvsp[0].no), NULL); }
#line 1444 "jtoc.tab.c"
    break;

  case 18: /* expressao: expressao MOD expressao  */
#line 125 "jtoc.y"
                              { (yyval.no) = montar_no("%", (yyvsp[-2].no), (yyvsp[0].no), NULL); }
#line 1450 "jtoc.tab.c"
    break;

  case 19: /* expressao: expressao AND expressao  */
#line 127 "jtoc.y"
                              { (yyval.no) = montar_no("&&", (yyvsp[-2].no), (yyvsp[0].no), NULL); }
#line 1456 "jtoc.tab.c"
    break;

  case 20: /* expressao: expressao OR expressao  */
#line 128 "jtoc.y"
                             { (yyval.no) = montar_no("||", (yyvsp[-2].no), (yyvsp[0].no), NULL); }
#line 1462 "jtoc.tab.c"
    break;

  case 21: /* expressao: MINUS expressao  */
#line 129 "jtoc.y"
                                { (yyval.no) = montar_no("-", (yyvsp[0].no), NULL, NULL); }
#line 1468 "jtoc.tab.c"
    break;

  case 22: /* expressao: NOT expressao  */
#line 130 "jtoc.y"
                    { (yyval.no) = montar_no("!", (yyvsp[0].no), NULL, NULL); }
#line 1474 "jtoc.tab.c"
    break;

  case 23: /* expressao: expressao EQ expressao  */
#line 131 "jtoc.y"
                             { (yyval.no) = montar_no("==", (yyvsp[-2].no), (yyvsp[0].no), NULL); }
#line 1480 "jtoc.tab.c"
    break;

  case 24: /* expressao: expressao NEQ expressao  */
#line 132 "jtoc.y"
                              { (yyval.no) = montar_no("!=", (yyvsp[-2].no), (yyvsp[0].no), NULL); }
#line 1486 "jtoc.tab.c"
    break;

  case 25: /* expressao: expressao LT expressao  */
#line 133 "jtoc.y"
                             { (yyval.no) = montar_no("<", (yyvsp[-2].no), (yyvsp[0].no), NULL); }
#line 1492 "jtoc.tab.c"
    break;

  case 26: /* expressao: expressao LE expressao  */
#line 134 "jtoc.y"
                             { (yyval.no) = montar_no("<=", (yyvsp[-2].no), (yyvsp[0].no), NULL); }
#line 1498 "jtoc.tab.c"
    break;

  case 27: /* expressao: expressao GT expressao  */
#line 135 "jtoc.y"
                             { (yyval.no) = montar_no(">", (yyvsp[-2].no), (yyvsp[0].no), NULL); }
#line 1504 "jtoc.tab.c"
    break;

  case 28: /* expressao: expressao GE expressao  */
#line 136 "jtoc.y"
                             { (yyval.no) = montar_no(">=", (yyvsp[-2].no), (yyvsp[0].no), NULL); }
#line 1510 "jtoc.tab.c"
    break;

  case 29: /* expressao: LPAREN expressao RPAREN  */
#line 138 "jtoc.y"
                              { (yyval.no) = (yyvsp[-1].no); }
#line 1516 "jtoc.tab.c"
    break;

  case 30: /* expressao: MATH_SQRT LPAREN expressao RPAREN  */
#line 140 "jtoc.y"
                                        { (yyval.no) = montar_no("Math.sqrt", (yyvsp[-1].no), NULL, NULL); }
#line 1522 "jtoc.tab.c"
    break;

  case 31: /* expressao: MATH_POW LPAREN expressao COMMA expressao RPAREN  */
#line 141 "jtoc.y"
                                                       { (yyval.no) = montar_no("Math.pow", (yyvsp[-3].no), (yyvsp[-1].no), NULL); }
#line 1528 "jtoc.tab.c"
    break;

  case 32: /* expressao: IDENTIFIER  */
#line 143 "jtoc.y"
                 { (yyval.no) = criar_folha((yyvsp[0].stringValue)); }
#line 1534 "jtoc.tab.c"
    break;

  case 33: /* expressao: FLOAT_LITERAL  */
#line 144 "jtoc.y"
                    { (yyval.no) = criar_folha((yyvsp[0].stringValue)); }
#line 1540 "jtoc.tab.c"
    break;

  case 34: /* expressao: INT_LITERAL  */
#line 145 "jtoc.y"
                  { (yyval.no) = criar_folha((yyvsp[0].stringValue)); }
#line 1546 "jtoc.tab.c"
    break;

  case 35: /* expressao: CHAR_LITERAL  */
#line 146 "jtoc.y"
                   { (yyval.no) = criar_folha((yyvsp[0].stringValue)); }
#line 1552 "jtoc.tab.c"
    break;

  case 36: /* expressao: STRING_LITERAL  */
#line 147 "jtoc.y"
                     { (yyval.no) = criar_folha((yyvsp[0].stringValue)); }
#line 1558 "jtoc.tab.c"
    break;

  case 37: /* expressao: TOKEN_TRUE  */
#line 148 "jtoc.y"
                 { (yyval.no) = criar_no("true"); }
#line 1564 "jtoc.tab.c"
    break;

  case 38: /* expressao: TOKEN_FALSE  */
#line 149 "jtoc.y"
                  { (yyval.no) = criar_no("false"); }
#line 1570 "jtoc.tab.c"
    break;

  case 39: /* tipo: INT  */
#line 153 "jtoc.y"
          { (yyval.no) = criar_no("int"); }
#line 1576 "jtoc.tab.c"
    break;

  case 40: /* tipo: FLOAT  */
#line 154 "jtoc.y"
            { (yyval.no) = criar_no("float"); }
#line 1582 "jtoc.tab.c"
    break;

  case 41: /* tipo: CHAR  */
#line 155 "jtoc.y"
           { (yyval.no) = criar_no("char"); }
#line 1588 "jtoc.tab.c"
    break;

  case 42: /* tipo: BOOLEAN  */
#line 156 "jtoc.y"
              { (yyval.no) = criar_no("boolean"); }
#line 1594 "jtoc.tab.c"
    break;

  case 43: /* tipo: STRING_TYPE  */
#line 157 "jtoc.y"
                  { (yyval.no) = criar_no("String"); }
#line 1600 "jtoc.tab.c"
    break;

  case 44: /* incrementacao: INC  */
#line 161 "jtoc.y"
          { (yyval.no) = criar_no("++"); }
#line 1606 "jtoc.tab.c"
    break;

  case 45: /* incrementacao: DEC  */
#line 162 "jtoc.y"
          { (yyval.no) = criar_no("--"); }
#line 1612 "jtoc.tab.c"
    break;


#line 1616 "jtoc.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 165 "jtoc.y"


/* Cada no guarda um texto, seu primeiro filho e o proximo irmao.
 * Assim, um bloco pode ter quantos comandos forem necessarios. */
NoAST *criar_no(const char *texto) {
    NoAST *no = malloc(sizeof(NoAST));
    if (no == NULL) {
        fprintf(stderr, "Memoria insuficiente para a AST.\n");
        exit(EXIT_FAILURE);
    }
    no->texto = strdup(texto);
    if (no->texto == NULL) {
        free(no);
        fprintf(stderr, "Memoria insuficiente para a AST.\n");
        exit(EXIT_FAILURE);
    }
    no->filho = NULL;
    no->irmao = NULL;
    return no;
}

void adicionar_filho(NoAST *pai, NoAST *filho) {
    NoAST *atual;
    if (filho == NULL) return;
    if (pai->filho == NULL) {
        pai->filho = filho;
        return;
    }
    atual = pai->filho;
    while (atual->irmao != NULL) atual = atual->irmao;
    atual->irmao = filho;
}

/* Atalho para criar comandos e operadores com ate tres filhos. */
NoAST *montar_no(const char *texto, NoAST *a, NoAST *b, NoAST *c) {
    NoAST *no = criar_no(texto);
    adicionar_filho(no, a);
    adicionar_filho(no, b);
    adicionar_filho(no, c);
    return no;
}

/* O texto recebido pertence ao token e pode ser liberado apos a copia.
 * Guardamos o lexema inteiro para preservar aspas e escapes de literais. */
NoAST *criar_folha(char *texto) {
    NoAST *no = criar_no(texto);
    free(texto);
    return no;
}

void imprimir_ast(NoAST *no, int nivel) {
    while (no != NULL) {
        printf("%*s%s\n", nivel * 2, "", no->texto);
        imprimir_ast(no->filho, nivel + 1);
        no = no->irmao;
    }
}

void liberar_ast(NoAST *no) {
    while (no != NULL) {
        NoAST *proximo = no->irmao;
        liberar_ast(no->filho);
        free(no->texto);
        free(no);
        no = proximo;
    }
}

/* Converte a expressao da AST em texto, sem executar o programa.
 * O valor registrado e o da inicializacao, nao o resultado em tempo de execucao. */
char *texto_expressao(NoAST *no) {
    char *esquerda, *direita, *texto;
    size_t tamanho;
    if (no == NULL) return strdup("nao inicializado");
    if (no->filho == NULL) return strdup(no->texto);

    esquerda = texto_expressao(no->filho);
    direita = no->filho->irmao != NULL
        ? texto_expressao(no->filho->irmao) : NULL;
    tamanho = strlen(no->texto) + strlen(esquerda) + 6;
    if (direita != NULL) tamanho += strlen(direita);
    texto = malloc(tamanho);
    if (texto == NULL) {
        fprintf(stderr, "Memoria insuficiente para a tabela de simbolos.\n");
        exit(EXIT_FAILURE);
    }

    if (strcmp(no->texto, "Math.pow") == 0) {
        snprintf(texto, tamanho, "Math.pow(%s, %s)", esquerda, direita);
    } else if (strcmp(no->texto, "Math.sqrt") == 0) {
        snprintf(texto, tamanho, "Math.sqrt(%s)", esquerda);
    } else if (direita == NULL) {
        snprintf(texto, tamanho, "(%s%s)", no->texto, esquerda);
    } else {
        snprintf(texto, tamanho, "(%s %s %s)", esquerda, no->texto, direita);
    }
    free(esquerda);
    free(direita);
    return texto;
}

void inserir_simbolo(NoAST *declaracao, int nivel) {
    NoAST *tipo = declaracao->filho;
    NoAST *nome = tipo->irmao;
    Simbolo *atual;
    Simbolo *novo = malloc(sizeof(Simbolo));
    if (novo == NULL) {
        fprintf(stderr, "Memoria insuficiente para a tabela de simbolos.\n");
        exit(EXIT_FAILURE);
    }
    novo->simbolo = strdup(nome->texto);
    novo->tipo = strdup(tipo->texto);
    novo->valor = texto_expressao(nome->irmao);
    novo->escopo = nivel == 0 ? "global" : "local";
    novo->proximo = NULL;
    if (novo->simbolo == NULL || novo->tipo == NULL || novo->valor == NULL) {
        fprintf(stderr, "Memoria insuficiente para a tabela de simbolos.\n");
        exit(EXIT_FAILURE);
    }

    if (tabela_simbolos == NULL) {
        tabela_simbolos = novo;
    } else {
        atual = tabela_simbolos;
        while (atual->proximo != NULL) atual = atual->proximo;
        atual->proximo = novo;
    }
}

/* O bloco principal tem nivel 0. Qualquer bloco interno e local.
 * Cada declaracao ocupa uma linha, inclusive nomes iguais em blocos diferentes.
 * Visitamos apenas comandos e blocos, sem confundir identificadores com comandos. */
void montar_tabela(NoAST *bloco, int nivel) {
    NoAST *comando = bloco->filho;
    while (comando != NULL) {
        if (strcmp(comando->texto, "Declaracao") == 0) {
            inserir_simbolo(comando, nivel);
        } else if (strcmp(comando->texto, "If") == 0 ||
                   strcmp(comando->texto, "While") == 0) {
            /* O primeiro filho e a condicao; os demais sao os blocos. */
            NoAST *corpo = comando->filho->irmao;
            while (corpo != NULL) {
                montar_tabela(corpo, nivel + 1);
                corpo = corpo->irmao;
            }
        }
        comando = comando->irmao;
    }
}

void imprimir_tabela(void) {
    Simbolo *atual = tabela_simbolos;
    printf("\nTabela de simbolos (valor de inicializacao):\n");
    printf("%-20s | %-10s | %-30s | %s\n", "Simbolo", "Tipo", "Valor", "Escopo");
    while (atual != NULL) {
        printf("%-20s | %-10s | %-30s | %s\n",
               atual->simbolo, atual->tipo, atual->valor, atual->escopo);
        atual = atual->proximo;
    }
}

void liberar_tabela(void) {
    while (tabela_simbolos != NULL) {
        Simbolo *proximo = tabela_simbolos->proximo;
        free(tabela_simbolos->simbolo);
        free(tabela_simbolos->tipo);
        free(tabela_simbolos->valor);
        free(tabela_simbolos);
        tabela_simbolos = proximo;
    }
}

void yyerror(const char *s) {
    fprintf(stderr, "Erro sintatico na linha %d: %s perto de '%s'\n", yylineno, s, yytext);
}

int yywrap(void) {
    return 1;
}

int main(int argc, char **argv) {
    if (argc > 1) {
        FILE *f = fopen(argv[1], "r");
        if (!f) {
            perror("Erro ao abrir o arquivo");
            return 1;
        }
        yyin = f;
    }

    int resultado = yyparse();
    if (resultado == 0 && erros_lexicos == 0) {
        printf("Analise concluida com sucesso! Nenhum erro sintatico encontrado.\n");
        printf("AST:\n");
        imprimir_ast(raiz, 0);
        montar_tabela(raiz, 0);
        imprimir_tabela();
    }

    liberar_tabela();
    liberar_ast(raiz);
    if (yyin) fclose(yyin);
    return (resultado != 0 || erros_lexicos != 0) ? EXIT_FAILURE : EXIT_SUCCESS;
}
