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
  YYSYMBOL_ADD_ASSIGN = 53,                /* ADD_ASSIGN  */
  YYSYMBOL_SUB_ASSIGN = 54,                /* SUB_ASSIGN  */
  YYSYMBOL_MUL_ASSIGN = 55,                /* MUL_ASSIGN  */
  YYSYMBOL_DIV_ASSIGN = 56,                /* DIV_ASSIGN  */
  YYSYMBOL_LBRACE = 57,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 58,                    /* RBRACE  */
  YYSYMBOL_LPAREN = 59,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 60,                    /* RPAREN  */
  YYSYMBOL_LBRACKET = 61,                  /* LBRACKET  */
  YYSYMBOL_RBRACKET = 62,                  /* RBRACKET  */
  YYSYMBOL_SEMI = 63,                      /* SEMI  */
  YYSYMBOL_COMMA = 64,                     /* COMMA  */
  YYSYMBOL_COLON = 65,                     /* COLON  */
  YYSYMBOL_PRINTLN = 66,                   /* PRINTLN  */
  YYSYMBOL_PRINT = 67,                     /* PRINT  */
  YYSYMBOL_MATH_SQRT = 68,                 /* MATH_SQRT  */
  YYSYMBOL_MATH_POW = 69,                  /* MATH_POW  */
  YYSYMBOL_YYACCEPT = 70,                  /* $accept  */
  YYSYMBOL_inicio = 71,                    /* inicio  */
  YYSYMBOL_programa = 72,                  /* programa  */
  YYSYMBOL_comando = 73,                   /* comando  */
  YYSYMBOL_bloco = 74,                     /* bloco  */
  YYSYMBOL_declaracao = 75,                /* declaracao  */
  YYSYMBOL_atribuicao = 76,                /* atribuicao  */
  YYSYMBOL_operador_atribuicao = 77,       /* operador_atribuicao  */
  YYSYMBOL_atualizacao = 78,               /* atualizacao  */
  YYSYMBOL_inicializacao_for = 79,         /* inicializacao_for  */
  YYSYMBOL_condicao_for = 80,              /* condicao_for  */
  YYSYMBOL_atualizacao_for = 81,           /* atualizacao_for  */
  YYSYMBOL_casos = 82,                     /* casos  */
  YYSYMBOL_caso = 83,                      /* caso  */
  YYSYMBOL_expressao = 84,                 /* expressao  */
  YYSYMBOL_tipo = 85,                      /* tipo  */
  YYSYMBOL_incrementacao = 86              /* incrementacao  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;



/* Unqualified %code blocks.  */
#line 21 "jtoc.y"

/* Uma entrada para cada declaracao encontrada no programa. */
typedef struct Simbolo {
    char *simbolo;
    char *tipo;
    char *valor;
    const char *escopo;
    struct Simbolo *proximo;
} Simbolo;

void inserir_simbolo(NoAST *declaracao, int nivel);
void montar_tabela(NoAST *no, int nivel);
void imprimir_tabela(void);
void liberar_tabela(void);

NoAST *raiz = NULL;
Simbolo *tabela_simbolos = NULL;

#line 225 "jtoc.tab.c"

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
typedef yytype_uint8 yy_state_t;

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
#define YYLAST   384

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  70
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  17
/* YYNRULES -- Number of rules.  */
#define YYNRULES  69
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  137

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   324


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
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    90,    90,    94,    95,    99,   100,   101,   102,   103,
     104,   105,   109,   110,   111,   112,   116,   120,   121,   125,
     133,   134,   135,   136,   137,   141,   142,   147,   148,   149,
     153,   154,   158,   159,   163,   164,   168,   169,   173,   174,
     175,   176,   177,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   190,   192,   193,   195,   196,   197,   198,
     199,   200,   201,   205,   206,   207,   208,   209,   213,   214
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
  "AND", "OR", "NOT", "ASSIGN", "ADD_ASSIGN", "SUB_ASSIGN", "MUL_ASSIGN",
  "DIV_ASSIGN", "LBRACE", "RBRACE", "LPAREN", "RPAREN", "LBRACKET",
  "RBRACKET", "SEMI", "COMMA", "COLON", "PRINTLN", "PRINT", "MATH_SQRT",
  "MATH_POW", "$accept", "inicio", "programa", "comando", "bloco",
  "declaracao", "atribuicao", "operador_atribuicao", "atualizacao",
  "inicializacao_for", "condicao_for", "atualizacao_for", "casos", "caso",
  "expressao", "tipo", "incrementacao", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-58)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -58,     6,    80,   -58,   273,   -58,   -58,   -58,   -58,   -58,
     -37,   -36,   -29,   -23,   -26,   -58,     0,     1,   -58,   -58,
      -1,   -58,     3,    60,   -58,   -58,   -58,   -58,   -58,   -58,
     -58,    21,   -58,    21,    21,    21,   124,   -58,    53,    21,
      21,   -58,   -58,    12,   -58,   -58,   -58,   -58,   -58,   -58,
     -58,    21,    21,    21,     9,    18,   294,   137,   155,   173,
     -43,   -58,   -58,     8,   -58,   191,   209,    21,   -58,   -58,
     227,    21,    21,    21,    21,    21,    21,    21,    21,    21,
      21,    21,    21,    21,    21,    21,    27,    29,    27,    21,
      15,    24,   294,   -58,   245,   115,    -6,    -6,   -58,   -58,
     -58,   336,   336,   -22,   -22,   -22,   -22,   323,   309,    66,
     -58,   -58,    30,   294,   -58,   -58,   -58,    21,    27,   -20,
      92,   263,   -58,    21,    33,   -58,   -58,   -58,    39,   -58,
      85,   -58,    27,   -58,    80,   -58,    80
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       3,     0,     2,     1,     0,    66,    65,    64,    63,    67,
       0,     0,     0,     0,     0,     3,     0,     0,     4,     7,
       0,    25,     0,     0,    68,    69,    20,    21,    22,    23,
      24,     0,    26,     0,     0,     0,    27,    13,     0,     0,
       0,     5,     6,    18,    56,    60,    59,    57,    58,    61,
      62,     0,     0,     0,     0,     0,    19,     0,     0,     0,
       0,    28,    29,     0,    16,     0,     0,     0,    45,    46,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    30,
       0,     0,    17,    53,     0,     0,    38,    39,    40,    41,
      42,    47,    48,    49,    50,    51,    52,    43,    44,     9,
      34,    10,     0,    31,    14,    15,    54,     0,     0,     0,
      32,     0,     8,     0,     0,    12,    35,    33,     0,    55,
       0,     3,     0,     3,    37,    11,    36
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -58,   -58,   -15,   -58,   -57,    68,    69,   -58,   -18,   -58,
     -58,   -58,   -58,   -58,   -32,   -58,   -58
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,     2,    18,    19,    20,    21,    31,    22,    63,
     112,   128,   119,   126,    56,    23,    32
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      38,    57,    58,    59,   123,   124,     3,    65,    66,    26,
      27,    28,    29,    30,    73,    74,    75,    76,    77,    68,
      69,    70,    33,    34,    44,    45,    46,    47,    48,   109,
      35,   111,    75,    76,    77,    92,    36,    37,   125,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,    49,    50,     4,   113,    51,    39,
      40,   122,    41,    43,    67,     5,    42,     6,    71,     7,
       8,    89,    52,     9,    10,   135,    11,    72,   114,    12,
      53,    13,    14,     4,    15,   121,   110,   115,   118,    54,
      55,   130,     5,   120,     6,     4,     7,     8,   131,   132,
       9,    10,   127,    11,    61,    62,    12,     0,    13,    14,
      15,    64,     0,     0,     0,     0,   134,     0,   136,    16,
      17,    73,    74,    75,    76,    77,     0,    60,    78,    79,
      80,    81,    82,    83,    84,    85,     5,    15,     6,     0,
       7,     8,     0,     0,     9,     0,    16,    17,     0,     0,
     133,    73,    74,    75,    76,    77,     0,     0,    78,    79,
      80,    81,    82,    83,    84,    85,     0,     0,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,     0,   117,
      78,    79,    80,    81,    82,    83,    84,    85,     0,     0,
       0,    73,    74,    75,    76,    77,     0,    86,    78,    79,
      80,    81,    82,    83,    84,    85,     0,     0,     0,    73,
      74,    75,    76,    77,     0,    87,    78,    79,    80,    81,
      82,    83,    84,    85,     0,     0,     0,    73,    74,    75,
      76,    77,     0,    88,    78,    79,    80,    81,    82,    83,
      84,    85,     0,     0,     0,    73,    74,    75,    76,    77,
       0,    90,    78,    79,    80,    81,    82,    83,    84,    85,
       0,     0,     0,    73,    74,    75,    76,    77,     0,    91,
      78,    79,    80,    81,    82,    83,    84,    85,     0,     0,
       0,    73,    74,    75,    76,    77,     0,    93,    78,    79,
      80,    81,    82,    83,    84,    85,     0,     0,     0,    73,
      74,    75,    76,    77,     0,   116,    78,    79,    80,    81,
      82,    83,    84,    85,    24,    25,     0,     0,     0,     0,
       0,     0,     0,   129,     0,    26,    27,    28,    29,    30,
      73,    74,    75,    76,    77,     0,     0,    78,    79,    80,
      81,    82,    83,    84,    85,    73,    74,    75,    76,    77,
       0,     0,    78,    79,    80,    81,    82,    83,    84,    73,
      74,    75,    76,    77,     0,     0,    78,    79,    80,    81,
      82,    83,    73,    74,    75,    76,    77,     0,     0,     0,
       0,    80,    81,    82,    83
};

static const yytype_int16 yycheck[] =
{
      15,    33,    34,    35,    24,    25,     0,    39,    40,    52,
      53,    54,    55,    56,    36,    37,    38,    39,    40,    51,
      52,    53,    59,    59,     3,     4,     5,     6,     7,    86,
      59,    88,    38,    39,    40,    67,    59,    63,    58,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    33,    34,     3,    89,    37,    59,
      59,   118,    63,     3,    52,    12,    63,    14,    59,    16,
      17,    63,    51,    20,    21,   132,    23,    59,    63,    26,
      59,    28,    29,     3,    57,   117,    57,    63,    22,    68,
      69,   123,    12,    63,    14,     3,    16,    17,    65,    60,
      20,    21,   120,    23,    36,    36,    26,    -1,    28,    29,
      57,    58,    -1,    -1,    -1,    -1,   131,    -1,   133,    66,
      67,    36,    37,    38,    39,    40,    -1,     3,    43,    44,
      45,    46,    47,    48,    49,    50,    12,    57,    14,    -1,
      16,    17,    -1,    -1,    20,    -1,    66,    67,    -1,    -1,
      65,    36,    37,    38,    39,    40,    -1,    -1,    43,    44,
      45,    46,    47,    48,    49,    50,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    36,    37,    38,    39,    40,    -1,    64,
      43,    44,    45,    46,    47,    48,    49,    50,    -1,    -1,
      -1,    36,    37,    38,    39,    40,    -1,    60,    43,    44,
      45,    46,    47,    48,    49,    50,    -1,    -1,    -1,    36,
      37,    38,    39,    40,    -1,    60,    43,    44,    45,    46,
      47,    48,    49,    50,    -1,    -1,    -1,    36,    37,    38,
      39,    40,    -1,    60,    43,    44,    45,    46,    47,    48,
      49,    50,    -1,    -1,    -1,    36,    37,    38,    39,    40,
      -1,    60,    43,    44,    45,    46,    47,    48,    49,    50,
      -1,    -1,    -1,    36,    37,    38,    39,    40,    -1,    60,
      43,    44,    45,    46,    47,    48,    49,    50,    -1,    -1,
      -1,    36,    37,    38,    39,    40,    -1,    60,    43,    44,
      45,    46,    47,    48,    49,    50,    -1,    -1,    -1,    36,
      37,    38,    39,    40,    -1,    60,    43,    44,    45,    46,
      47,    48,    49,    50,    41,    42,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    60,    -1,    52,    53,    54,    55,    56,
      36,    37,    38,    39,    40,    -1,    -1,    43,    44,    45,
      46,    47,    48,    49,    50,    36,    37,    38,    39,    40,
      -1,    -1,    43,    44,    45,    46,    47,    48,    49,    36,
      37,    38,    39,    40,    -1,    -1,    43,    44,    45,    46,
      47,    48,    36,    37,    38,    39,    40,    -1,    -1,    -1,
      -1,    45,    46,    47,    48
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    71,    72,     0,     3,    12,    14,    16,    17,    20,
      21,    23,    26,    28,    29,    57,    66,    67,    73,    74,
      75,    76,    78,    85,    41,    42,    52,    53,    54,    55,
      56,    77,    86,    59,    59,    59,    59,    63,    72,    59,
      59,    63,    63,     3,     3,     4,     5,     6,     7,    33,
      34,    37,    51,    59,    68,    69,    84,    84,    84,    84,
       3,    75,    76,    79,    58,    84,    84,    52,    84,    84,
      84,    59,    59,    36,    37,    38,    39,    40,    43,    44,
      45,    46,    47,    48,    49,    50,    60,    60,    60,    63,
      60,    60,    84,    60,    84,    84,    84,    84,    84,    84,
      84,    84,    84,    84,    84,    84,    84,    84,    84,    74,
      57,    74,    80,    84,    63,    63,    60,    64,    22,    82,
      63,    84,    74,    24,    25,    58,    83,    78,    81,    60,
      84,    65,    60,    65,    72,    74,    72
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    70,    71,    72,    72,    73,    73,    73,    73,    73,
      73,    73,    73,    73,    73,    73,    74,    75,    75,    76,
      77,    77,    77,    77,    77,    78,    78,    79,    79,    79,
      80,    80,    81,    81,    82,    82,    83,    83,    84,    84,
      84,    84,    84,    84,    84,    84,    84,    84,    84,    84,
      84,    84,    84,    84,    84,    84,    84,    84,    84,    84,
      84,    84,    84,    85,    85,    85,    85,    85,    86,    86
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     2,     2,     1,     7,     5,
       5,     9,     7,     2,     5,     5,     3,     4,     2,     3,
       1,     1,     1,     1,     1,     1,     2,     0,     1,     1,
       0,     1,     0,     1,     0,     2,     4,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     2,     3,     3,     3,
       3,     3,     3,     3,     4,     6,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1
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
#line 53 "jtoc.y"
            { free(((*yyvaluep).stringValue)); }
#line 1059 "jtoc.tab.c"
        break;

    case YYSYMBOL_STRING_LITERAL: /* STRING_LITERAL  */
#line 53 "jtoc.y"
            { free(((*yyvaluep).stringValue)); }
#line 1065 "jtoc.tab.c"
        break;

    case YYSYMBOL_CHAR_LITERAL: /* CHAR_LITERAL  */
#line 53 "jtoc.y"
            { free(((*yyvaluep).stringValue)); }
#line 1071 "jtoc.tab.c"
        break;

    case YYSYMBOL_FLOAT_LITERAL: /* FLOAT_LITERAL  */
#line 53 "jtoc.y"
            { free(((*yyvaluep).stringValue)); }
#line 1077 "jtoc.tab.c"
        break;

    case YYSYMBOL_INT_LITERAL: /* INT_LITERAL  */
#line 53 "jtoc.y"
            { free(((*yyvaluep).stringValue)); }
#line 1083 "jtoc.tab.c"
        break;

    case YYSYMBOL_programa: /* programa  */
#line 54 "jtoc.y"
            { liberar_ast(((*yyvaluep).no)); }
#line 1089 "jtoc.tab.c"
        break;

    case YYSYMBOL_comando: /* comando  */
#line 54 "jtoc.y"
            { liberar_ast(((*yyvaluep).no)); }
#line 1095 "jtoc.tab.c"
        break;

    case YYSYMBOL_bloco: /* bloco  */
#line 54 "jtoc.y"
            { liberar_ast(((*yyvaluep).no)); }
#line 1101 "jtoc.tab.c"
        break;

    case YYSYMBOL_declaracao: /* declaracao  */
#line 54 "jtoc.y"
            { liberar_ast(((*yyvaluep).no)); }
#line 1107 "jtoc.tab.c"
        break;

    case YYSYMBOL_atribuicao: /* atribuicao  */
#line 54 "jtoc.y"
            { liberar_ast(((*yyvaluep).no)); }
#line 1113 "jtoc.tab.c"
        break;

    case YYSYMBOL_operador_atribuicao: /* operador_atribuicao  */
#line 54 "jtoc.y"
            { liberar_ast(((*yyvaluep).no)); }
#line 1119 "jtoc.tab.c"
        break;

    case YYSYMBOL_atualizacao: /* atualizacao  */
#line 54 "jtoc.y"
            { liberar_ast(((*yyvaluep).no)); }
#line 1125 "jtoc.tab.c"
        break;

    case YYSYMBOL_inicializacao_for: /* inicializacao_for  */
#line 54 "jtoc.y"
            { liberar_ast(((*yyvaluep).no)); }
#line 1131 "jtoc.tab.c"
        break;

    case YYSYMBOL_condicao_for: /* condicao_for  */
#line 54 "jtoc.y"
            { liberar_ast(((*yyvaluep).no)); }
#line 1137 "jtoc.tab.c"
        break;

    case YYSYMBOL_atualizacao_for: /* atualizacao_for  */
#line 54 "jtoc.y"
            { liberar_ast(((*yyvaluep).no)); }
#line 1143 "jtoc.tab.c"
        break;

    case YYSYMBOL_casos: /* casos  */
#line 54 "jtoc.y"
            { liberar_ast(((*yyvaluep).no)); }
#line 1149 "jtoc.tab.c"
        break;

    case YYSYMBOL_caso: /* caso  */
#line 54 "jtoc.y"
            { liberar_ast(((*yyvaluep).no)); }
#line 1155 "jtoc.tab.c"
        break;

    case YYSYMBOL_expressao: /* expressao  */
#line 54 "jtoc.y"
            { liberar_ast(((*yyvaluep).no)); }
#line 1161 "jtoc.tab.c"
        break;

    case YYSYMBOL_tipo: /* tipo  */
#line 54 "jtoc.y"
            { liberar_ast(((*yyvaluep).no)); }
#line 1167 "jtoc.tab.c"
        break;

    case YYSYMBOL_incrementacao: /* incrementacao  */
#line 54 "jtoc.y"
            { liberar_ast(((*yyvaluep).no)); }
#line 1173 "jtoc.tab.c"
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
#line 90 "jtoc.y"
               { raiz = (yyvsp[0].no); }
#line 1443 "jtoc.tab.c"
    break;

  case 3: /* programa: %empty  */
#line 94 "jtoc.y"
             { (yyval.no) = criar_no("Bloco"); }
#line 1449 "jtoc.tab.c"
    break;

  case 4: /* programa: programa comando  */
#line 95 "jtoc.y"
                       { adicionar_filho((yyvsp[-1].no), (yyvsp[0].no)); (yyval.no) = (yyvsp[-1].no); }
#line 1455 "jtoc.tab.c"
    break;

  case 5: /* comando: declaracao SEMI  */
#line 99 "jtoc.y"
                      { (yyval.no) = (yyvsp[-1].no); }
#line 1461 "jtoc.tab.c"
    break;

  case 6: /* comando: atualizacao SEMI  */
#line 100 "jtoc.y"
                       { (yyval.no) = (yyvsp[-1].no); }
#line 1467 "jtoc.tab.c"
    break;

  case 7: /* comando: bloco  */
#line 101 "jtoc.y"
            { (yyval.no) = (yyvsp[0].no); }
#line 1473 "jtoc.tab.c"
    break;

  case 8: /* comando: IF LPAREN expressao RPAREN bloco ELSE bloco  */
#line 102 "jtoc.y"
                                                  { (yyval.no) = montar_no("If", (yyvsp[-4].no), (yyvsp[-2].no), (yyvsp[0].no)); }
#line 1479 "jtoc.tab.c"
    break;

  case 9: /* comando: IF LPAREN expressao RPAREN bloco  */
#line 103 "jtoc.y"
                                       { (yyval.no) = montar_no("If", (yyvsp[-2].no), (yyvsp[0].no), NULL); }
#line 1485 "jtoc.tab.c"
    break;

  case 10: /* comando: WHILE LPAREN expressao RPAREN bloco  */
#line 104 "jtoc.y"
                                          { (yyval.no) = montar_no("While", (yyvsp[-2].no), (yyvsp[0].no), NULL); }
#line 1491 "jtoc.tab.c"
    break;

  case 11: /* comando: FOR LPAREN inicializacao_for SEMI condicao_for SEMI atualizacao_for RPAREN bloco  */
#line 105 "jtoc.y"
                                                                                       {
          (yyval.no) = montar_no("For", (yyvsp[-6].no), (yyvsp[-4].no), (yyvsp[-2].no));
          adicionar_filho((yyval.no), (yyvsp[0].no));
      }
#line 1500 "jtoc.tab.c"
    break;

  case 12: /* comando: SWITCH LPAREN expressao RPAREN LBRACE casos RBRACE  */
#line 109 "jtoc.y"
                                                         { (yyval.no) = montar_no("Switch", (yyvsp[-4].no), (yyvsp[-1].no), NULL); }
#line 1506 "jtoc.tab.c"
    break;

  case 13: /* comando: BREAK SEMI  */
#line 110 "jtoc.y"
                 { (yyval.no) = criar_no("Break"); }
#line 1512 "jtoc.tab.c"
    break;

  case 14: /* comando: PRINTLN LPAREN expressao RPAREN SEMI  */
#line 111 "jtoc.y"
                                           { (yyval.no) = montar_no("Println", (yyvsp[-2].no), NULL, NULL); }
#line 1518 "jtoc.tab.c"
    break;

  case 15: /* comando: PRINT LPAREN expressao RPAREN SEMI  */
#line 112 "jtoc.y"
                                         { (yyval.no) = montar_no("Print", (yyvsp[-2].no), NULL, NULL); }
#line 1524 "jtoc.tab.c"
    break;

  case 16: /* bloco: LBRACE programa RBRACE  */
#line 116 "jtoc.y"
                             { (yyval.no) = (yyvsp[-1].no); }
#line 1530 "jtoc.tab.c"
    break;

  case 17: /* declaracao: tipo IDENTIFIER ASSIGN expressao  */
#line 120 "jtoc.y"
                                       { (yyval.no) = montar_no("Declaracao", (yyvsp[-3].no), criar_folha((yyvsp[-2].stringValue)), (yyvsp[0].no)); }
#line 1536 "jtoc.tab.c"
    break;

  case 18: /* declaracao: tipo IDENTIFIER  */
#line 121 "jtoc.y"
                      { (yyval.no) = montar_no("Declaracao", (yyvsp[-1].no), criar_folha((yyvsp[0].stringValue)), NULL); }
#line 1542 "jtoc.tab.c"
    break;

  case 19: /* atribuicao: IDENTIFIER operador_atribuicao expressao  */
#line 125 "jtoc.y"
                                               {
          adicionar_filho((yyvsp[-1].no), criar_folha((yyvsp[-2].stringValue)));
          adicionar_filho((yyvsp[-1].no), (yyvsp[0].no));
          (yyval.no) = (yyvsp[-1].no);
      }
#line 1552 "jtoc.tab.c"
    break;

  case 20: /* operador_atribuicao: ASSIGN  */
#line 133 "jtoc.y"
             { (yyval.no) = criar_no("="); }
#line 1558 "jtoc.tab.c"
    break;

  case 21: /* operador_atribuicao: ADD_ASSIGN  */
#line 134 "jtoc.y"
                 { (yyval.no) = criar_no("+="); }
#line 1564 "jtoc.tab.c"
    break;

  case 22: /* operador_atribuicao: SUB_ASSIGN  */
#line 135 "jtoc.y"
                 { (yyval.no) = criar_no("-="); }
#line 1570 "jtoc.tab.c"
    break;

  case 23: /* operador_atribuicao: MUL_ASSIGN  */
#line 136 "jtoc.y"
                 { (yyval.no) = criar_no("*="); }
#line 1576 "jtoc.tab.c"
    break;

  case 24: /* operador_atribuicao: DIV_ASSIGN  */
#line 137 "jtoc.y"
                 { (yyval.no) = criar_no("/="); }
#line 1582 "jtoc.tab.c"
    break;

  case 25: /* atualizacao: atribuicao  */
#line 141 "jtoc.y"
                 { (yyval.no) = (yyvsp[0].no); }
#line 1588 "jtoc.tab.c"
    break;

  case 26: /* atualizacao: IDENTIFIER incrementacao  */
#line 142 "jtoc.y"
                               { adicionar_filho((yyvsp[0].no), criar_folha((yyvsp[-1].stringValue))); (yyval.no) = (yyvsp[0].no); }
#line 1594 "jtoc.tab.c"
    break;

  case 27: /* inicializacao_for: %empty  */
#line 147 "jtoc.y"
             { (yyval.no) = criar_no("Vazio"); }
#line 1600 "jtoc.tab.c"
    break;

  case 28: /* inicializacao_for: declaracao  */
#line 148 "jtoc.y"
                 { (yyval.no) = (yyvsp[0].no); }
#line 1606 "jtoc.tab.c"
    break;

  case 29: /* inicializacao_for: atribuicao  */
#line 149 "jtoc.y"
                 { (yyval.no) = (yyvsp[0].no); }
#line 1612 "jtoc.tab.c"
    break;

  case 30: /* condicao_for: %empty  */
#line 153 "jtoc.y"
             { (yyval.no) = criar_no("Vazio"); }
#line 1618 "jtoc.tab.c"
    break;

  case 31: /* condicao_for: expressao  */
#line 154 "jtoc.y"
                { (yyval.no) = (yyvsp[0].no); }
#line 1624 "jtoc.tab.c"
    break;

  case 32: /* atualizacao_for: %empty  */
#line 158 "jtoc.y"
             { (yyval.no) = criar_no("Vazio"); }
#line 1630 "jtoc.tab.c"
    break;

  case 33: /* atualizacao_for: atualizacao  */
#line 159 "jtoc.y"
                  { (yyval.no) = (yyvsp[0].no); }
#line 1636 "jtoc.tab.c"
    break;

  case 34: /* casos: %empty  */
#line 163 "jtoc.y"
             { (yyval.no) = criar_no("Casos"); }
#line 1642 "jtoc.tab.c"
    break;

  case 35: /* casos: casos caso  */
#line 164 "jtoc.y"
                 { adicionar_filho((yyvsp[-1].no), (yyvsp[0].no)); (yyval.no) = (yyvsp[-1].no); }
#line 1648 "jtoc.tab.c"
    break;

  case 36: /* caso: CASE expressao COLON programa  */
#line 168 "jtoc.y"
                                    { (yyval.no) = montar_no("Case", (yyvsp[-2].no), (yyvsp[0].no), NULL); }
#line 1654 "jtoc.tab.c"
    break;

  case 37: /* caso: DEFAULT COLON programa  */
#line 169 "jtoc.y"
                             { (yyval.no) = montar_no("Default", (yyvsp[0].no), NULL, NULL); }
#line 1660 "jtoc.tab.c"
    break;

  case 38: /* expressao: expressao PLUS expressao  */
#line 173 "jtoc.y"
                               { (yyval.no) = montar_no("+", (yyvsp[-2].no), (yyvsp[0].no), NULL); }
#line 1666 "jtoc.tab.c"
    break;

  case 39: /* expressao: expressao MINUS expressao  */
#line 174 "jtoc.y"
                                { (yyval.no) = montar_no("-", (yyvsp[-2].no), (yyvsp[0].no), NULL); }
#line 1672 "jtoc.tab.c"
    break;

  case 40: /* expressao: expressao MULT expressao  */
#line 175 "jtoc.y"
                               { (yyval.no) = montar_no("*", (yyvsp[-2].no), (yyvsp[0].no), NULL); }
#line 1678 "jtoc.tab.c"
    break;

  case 41: /* expressao: expressao DIV expressao  */
#line 176 "jtoc.y"
                              { (yyval.no) = montar_no("/", (yyvsp[-2].no), (yyvsp[0].no), NULL); }
#line 1684 "jtoc.tab.c"
    break;

  case 42: /* expressao: expressao MOD expressao  */
#line 177 "jtoc.y"
                              { (yyval.no) = montar_no("%", (yyvsp[-2].no), (yyvsp[0].no), NULL); }
#line 1690 "jtoc.tab.c"
    break;

  case 43: /* expressao: expressao AND expressao  */
#line 179 "jtoc.y"
                              { (yyval.no) = montar_no("&&", (yyvsp[-2].no), (yyvsp[0].no), NULL); }
#line 1696 "jtoc.tab.c"
    break;

  case 44: /* expressao: expressao OR expressao  */
#line 180 "jtoc.y"
                             { (yyval.no) = montar_no("||", (yyvsp[-2].no), (yyvsp[0].no), NULL); }
#line 1702 "jtoc.tab.c"
    break;

  case 45: /* expressao: MINUS expressao  */
#line 181 "jtoc.y"
                                { (yyval.no) = montar_no("-", (yyvsp[0].no), NULL, NULL); }
#line 1708 "jtoc.tab.c"
    break;

  case 46: /* expressao: NOT expressao  */
#line 182 "jtoc.y"
                    { (yyval.no) = montar_no("!", (yyvsp[0].no), NULL, NULL); }
#line 1714 "jtoc.tab.c"
    break;

  case 47: /* expressao: expressao EQ expressao  */
#line 183 "jtoc.y"
                             { (yyval.no) = montar_no("==", (yyvsp[-2].no), (yyvsp[0].no), NULL); }
#line 1720 "jtoc.tab.c"
    break;

  case 48: /* expressao: expressao NEQ expressao  */
#line 184 "jtoc.y"
                              { (yyval.no) = montar_no("!=", (yyvsp[-2].no), (yyvsp[0].no), NULL); }
#line 1726 "jtoc.tab.c"
    break;

  case 49: /* expressao: expressao LT expressao  */
#line 185 "jtoc.y"
                             { (yyval.no) = montar_no("<", (yyvsp[-2].no), (yyvsp[0].no), NULL); }
#line 1732 "jtoc.tab.c"
    break;

  case 50: /* expressao: expressao LE expressao  */
#line 186 "jtoc.y"
                             { (yyval.no) = montar_no("<=", (yyvsp[-2].no), (yyvsp[0].no), NULL); }
#line 1738 "jtoc.tab.c"
    break;

  case 51: /* expressao: expressao GT expressao  */
#line 187 "jtoc.y"
                             { (yyval.no) = montar_no(">", (yyvsp[-2].no), (yyvsp[0].no), NULL); }
#line 1744 "jtoc.tab.c"
    break;

  case 52: /* expressao: expressao GE expressao  */
#line 188 "jtoc.y"
                             { (yyval.no) = montar_no(">=", (yyvsp[-2].no), (yyvsp[0].no), NULL); }
#line 1750 "jtoc.tab.c"
    break;

  case 53: /* expressao: LPAREN expressao RPAREN  */
#line 190 "jtoc.y"
                              { (yyval.no) = (yyvsp[-1].no); }
#line 1756 "jtoc.tab.c"
    break;

  case 54: /* expressao: MATH_SQRT LPAREN expressao RPAREN  */
#line 192 "jtoc.y"
                                        { (yyval.no) = montar_no("Math.sqrt", (yyvsp[-1].no), NULL, NULL); }
#line 1762 "jtoc.tab.c"
    break;

  case 55: /* expressao: MATH_POW LPAREN expressao COMMA expressao RPAREN  */
#line 193 "jtoc.y"
                                                       { (yyval.no) = montar_no("Math.pow", (yyvsp[-3].no), (yyvsp[-1].no), NULL); }
#line 1768 "jtoc.tab.c"
    break;

  case 56: /* expressao: IDENTIFIER  */
#line 195 "jtoc.y"
                 { (yyval.no) = criar_folha((yyvsp[0].stringValue)); }
#line 1774 "jtoc.tab.c"
    break;

  case 57: /* expressao: FLOAT_LITERAL  */
#line 196 "jtoc.y"
                    { (yyval.no) = criar_folha((yyvsp[0].stringValue)); }
#line 1780 "jtoc.tab.c"
    break;

  case 58: /* expressao: INT_LITERAL  */
#line 197 "jtoc.y"
                  { (yyval.no) = criar_folha((yyvsp[0].stringValue)); }
#line 1786 "jtoc.tab.c"
    break;

  case 59: /* expressao: CHAR_LITERAL  */
#line 198 "jtoc.y"
                   { (yyval.no) = criar_folha((yyvsp[0].stringValue)); }
#line 1792 "jtoc.tab.c"
    break;

  case 60: /* expressao: STRING_LITERAL  */
#line 199 "jtoc.y"
                     { (yyval.no) = criar_folha((yyvsp[0].stringValue)); }
#line 1798 "jtoc.tab.c"
    break;

  case 61: /* expressao: TOKEN_TRUE  */
#line 200 "jtoc.y"
                 { (yyval.no) = criar_no("true"); }
#line 1804 "jtoc.tab.c"
    break;

  case 62: /* expressao: TOKEN_FALSE  */
#line 201 "jtoc.y"
                  { (yyval.no) = criar_no("false"); }
#line 1810 "jtoc.tab.c"
    break;

  case 63: /* tipo: INT  */
#line 205 "jtoc.y"
          { (yyval.no) = criar_no("int"); }
#line 1816 "jtoc.tab.c"
    break;

  case 64: /* tipo: FLOAT  */
#line 206 "jtoc.y"
            { (yyval.no) = criar_no("float"); }
#line 1822 "jtoc.tab.c"
    break;

  case 65: /* tipo: CHAR  */
#line 207 "jtoc.y"
           { (yyval.no) = criar_no("char"); }
#line 1828 "jtoc.tab.c"
    break;

  case 66: /* tipo: BOOLEAN  */
#line 208 "jtoc.y"
              { (yyval.no) = criar_no("boolean"); }
#line 1834 "jtoc.tab.c"
    break;

  case 67: /* tipo: STRING_TYPE  */
#line 209 "jtoc.y"
                  { (yyval.no) = criar_no("String"); }
#line 1840 "jtoc.tab.c"
    break;

  case 68: /* incrementacao: INC  */
#line 213 "jtoc.y"
          { (yyval.no) = criar_no("++"); }
#line 1846 "jtoc.tab.c"
    break;

  case 69: /* incrementacao: DEC  */
#line 214 "jtoc.y"
          { (yyval.no) = criar_no("--"); }
#line 1852 "jtoc.tab.c"
    break;


#line 1856 "jtoc.tab.c"

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

#line 217 "jtoc.y"


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
        } else if (strcmp(comando->texto, "Bloco") == 0) {
            montar_tabela(comando, nivel + 1);
        } else if (strcmp(comando->texto, "If") == 0 ||
                   strcmp(comando->texto, "While") == 0) {
            /* O primeiro filho e a condicao; os demais sao os blocos. */
            NoAST *corpo = comando->filho->irmao;
            while (corpo != NULL) {
                montar_tabela(corpo, nivel + 1);
                corpo = corpo->irmao;
            }
        } else if (strcmp(comando->texto, "For") == 0) {
            NoAST *inicializacao = comando->filho;
            NoAST *corpo = inicializacao->irmao->irmao->irmao;
            if (strcmp(inicializacao->texto, "Declaracao") == 0) {
                inserir_simbolo(inicializacao, nivel + 1);
            }
            montar_tabela(corpo, nivel + 2);
        } else if (strcmp(comando->texto, "Switch") == 0) {
            NoAST *caso = comando->filho->irmao->filho;
            while (caso != NULL) {
                NoAST *corpo = caso->filho;
                if (strcmp(caso->texto, "Case") == 0) corpo = corpo->irmao;
                montar_tabela(corpo, nivel + 1);
                caso = caso->irmao;
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
