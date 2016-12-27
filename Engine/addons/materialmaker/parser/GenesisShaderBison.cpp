
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0

/* Substitute the variable and function names.  */
#define yyparse         Genesisparse
#define yylex           Genesislex
#define yyerror         Genesiserror
#define yylval          Genesislval
#define yychar          Genesischar
#define yydebug         Genesisdebug
#define yynerrs         Genesisnerrs


/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "GenesisShaderBison.ycc"

#include "stdneb.h"
#include "rendersystem/base/RenderDeviceTypes.h"
#include "GenesisShaderParser.h"
#include "../GenesisMaterial.h"
#include "addons/shadercompiler/Utility/ShaderCompilerUtil.h"

void ResetParserParams();
int yyerror (const char *s);
extern int Genesislineno;
extern char* yytext;
int yylex ();

using namespace GenesisMaterialMaker;
using namespace ShaderProgramCompiler;

GenesisMaterial* g_GenesisMaterial;
static  GenesisMakePass* g_curMakePass = NULL;
static  GenesisMakeTechnique* g_curGenesisMakeTechnique = NULL;
static  GenesisMakeMaterial* g_curGenesisMakeMaterial = NULL;
static  GenesisMakeGPUProgram* g_curGenesisMakeGPUProgram = NULL;
static  GenesisSubGPUProgram* g_curGenesisSubGPUProgram = NULL;
static  Graphic::ShaderParam* g_curShaderParameter = NULL;
static  Graphic::MaterialParam* g_curMatParam = NULL;
static  GPtr<RenderBase::RenderStateDesc> g_rsDesc = 0;

#define ASSIGN(s,d) {s = *d; delete d;}
#define YYDEBUG 1



/* Line 189 of yacc.c  */
#line 113 "GenesisShaderBison.cpp"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TSHADER = 258,
     TTECHNIQUE = 259,
     TPASS = 260,
     TSETSHADERCODE = 261,
     TPARAMETERS = 262,
     TRENDERQUEUE = 263,
     TSHADERTYPE = 264,
     TRENDERDEVICETYPE = 265,
     TSETPARAM = 266,
     TRENDERSTATE = 267,
     TCULLMODE = 268,
     TFILLMODE = 269,
     TCOLORMASK = 270,
     TDEPTHTEST = 271,
     TDEPTHWRITE = 272,
     TBLENDCOLOR = 273,
     TALPHATEST = 274,
     TSAMPLER = 275,
     TSUBGPUPROGRAM = 276,
     TBUILTINMACRO = 277,
     TCUSTOMMACRO = 278,
     TMATTYPE = 279,
     TMATRIX = 280,
     TVECTOR = 281,
     TFLOAT = 282,
     TTEXTURE = 283,
     TREALSTRING = 284,
     TVAR = 285,
     TOPERATOR = 286,
     TNUMBER = 287,
     TBOOLEAN = 288
   };
#endif
/* Tokens.  */
#define TSHADER 258
#define TTECHNIQUE 259
#define TPASS 260
#define TSETSHADERCODE 261
#define TPARAMETERS 262
#define TRENDERQUEUE 263
#define TSHADERTYPE 264
#define TRENDERDEVICETYPE 265
#define TSETPARAM 266
#define TRENDERSTATE 267
#define TCULLMODE 268
#define TFILLMODE 269
#define TCOLORMASK 270
#define TDEPTHTEST 271
#define TDEPTHWRITE 272
#define TBLENDCOLOR 273
#define TALPHATEST 274
#define TSAMPLER 275
#define TSUBGPUPROGRAM 276
#define TBUILTINMACRO 277
#define TCUSTOMMACRO 278
#define TMATTYPE 279
#define TMATRIX 280
#define TVECTOR 281
#define TFLOAT 282
#define TTEXTURE 283
#define TREALSTRING 284
#define TVAR 285
#define TOPERATOR 286
#define TNUMBER 287
#define TBOOLEAN 288




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 33 "GenesisShaderBison.ycc"

	char* str;
	float num;
	Graphic::ShaderParamType spt;
	bool boolean;



/* Line 214 of yacc.c  */
#line 224 "GenesisShaderBison.cpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 236 "GenesisShaderBison.cpp"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   116

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  37
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  40
/* YYNRULES -- Number of rules.  */
#define YYNRULES  80
/* YYNRULES -- Number of states.  */
#define YYNSTATES  146

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   288

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    36,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    34,     2,    35,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30,    31,    32,    33
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     5,     6,    15,    16,    17,    18,
      26,    30,    31,    37,    38,    45,    46,    52,    59,    66,
      72,    79,    85,    92,    98,    99,   100,   104,   105,   109,
     110,   111,   112,   122,   123,   126,   127,   130,   133,   139,
     145,   146,   147,   148,   156,   157,   161,   165,   169,   172,
     176,   179,   182,   185,   189,   194,   197,   200,   205,   208,
     212,   215,   216,   217,   218,   227,   228,   229,   230,   231,
     241,   242,   243,   244,   245,   255,   256,   260,   266,   272,
     278
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      38,     0,    -1,    -1,    -1,    -1,     3,    29,    39,    34,
      40,    42,    41,    35,    -1,    -1,    -1,    -1,    42,     7,
      34,    43,    47,    44,    35,    -1,    42,     8,    29,    -1,
      -1,    42,     4,    34,    45,    48,    -1,    -1,    42,     4,
      29,    34,    46,    48,    -1,    -1,    47,    28,    30,    36,
      29,    -1,    47,    28,    30,    31,    36,    29,    -1,    47,
      25,    30,    31,    36,    29,    -1,    47,    25,    30,    36,
      29,    -1,    47,    26,    30,    31,    36,    29,    -1,    47,
      26,    30,    36,    29,    -1,    47,    27,    30,    31,    36,
      29,    -1,    47,    27,    30,    36,    29,    -1,    -1,    -1,
      50,    49,    35,    -1,    -1,    50,    24,    29,    -1,    -1,
      -1,    -1,    50,     5,    54,    51,    34,    52,    55,    53,
      35,    -1,    -1,    54,    29,    -1,    -1,    55,    56,    -1,
      55,    64,    -1,    55,    22,    34,    29,    35,    -1,    55,
      23,    34,    29,    35,    -1,    -1,    -1,    -1,    56,    12,
      34,    57,    59,    58,    35,    -1,    -1,    59,    13,    29,
      -1,    59,    14,    29,    -1,    59,    15,    29,    -1,    59,
      61,    -1,    59,    17,    33,    -1,    59,    62,    -1,    59,
      63,    -1,    59,    60,    -1,    20,    29,    30,    -1,    20,
      29,    30,    30,    -1,    16,    30,    -1,    16,    33,    -1,
      18,    30,    29,    29,    -1,    18,    33,    -1,    19,    30,
      29,    -1,    19,    33,    -1,    -1,    -1,    -1,     9,    29,
      65,    34,    66,    68,    67,    35,    -1,    -1,    -1,    -1,
      -1,    68,    10,    29,    69,    34,    70,    72,    71,    35,
      -1,    -1,    -1,    -1,    -1,    72,    21,    29,    73,    34,
      74,    76,    75,    35,    -1,    -1,    76,     6,    29,    -1,
      76,    11,    32,    30,    28,    -1,    76,    11,    32,    30,
      25,    -1,    76,    11,    32,    30,    26,    -1,    76,    11,
      32,    30,    27,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    72,    72,    78,    80,    72,    87,    89,    91,    89,
      95,    98,    98,   103,   103,   109,   111,   155,   199,   207,
     215,   223,   231,   239,   248,   250,   250,   258,   260,   263,
     265,   268,   263,   277,   282,   288,   290,   292,   294,   298,
     303,   305,   310,   305,   317,   319,   324,   329,   334,   336,
     341,   343,   345,   348,   351,   355,   362,   369,   378,   385,
     402,   409,   415,   417,   409,   428,   429,   434,   436,   429,
     441,   442,   444,   448,   442,   459,   461,   466,   475,   484,
     493
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TSHADER", "TTECHNIQUE", "TPASS",
  "TSETSHADERCODE", "TPARAMETERS", "TRENDERQUEUE", "TSHADERTYPE",
  "TRENDERDEVICETYPE", "TSETPARAM", "TRENDERSTATE", "TCULLMODE",
  "TFILLMODE", "TCOLORMASK", "TDEPTHTEST", "TDEPTHWRITE", "TBLENDCOLOR",
  "TALPHATEST", "TSAMPLER", "TSUBGPUPROGRAM", "TBUILTINMACRO",
  "TCUSTOMMACRO", "TMATTYPE", "TMATRIX", "TVECTOR", "TFLOAT", "TTEXTURE",
  "TREALSTRING", "TVAR", "TOPERATOR", "TNUMBER", "TBOOLEAN", "'{'", "'}'",
  "'='", "$accept", "shader", "$@1", "$@2", "$@3", "PropertySection",
  "$@4", "$@5", "$@6", "$@7", "ParameterSection", "TechniqueSection",
  "$@8", "PassSection", "$@9", "$@10", "$@11", "PassType", "codeSection",
  "StateSection", "$@12", "$@13", "RenderStateSetup", "SamplerSetup",
  "DepthTestSetup", "BlendSetup", "AlphaTestSetup", "shadertype", "$@14",
  "$@15", "$@16", "DeviceTypeSetup", "$@17", "$@18", "$@19",
  "SubGPUProgramSetup", "$@20", "$@21", "$@22", "CodeBlock", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   123,   125,    61
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    37,    39,    40,    41,    38,    42,    43,    44,    42,
      42,    45,    42,    46,    42,    47,    47,    47,    47,    47,
      47,    47,    47,    47,    48,    49,    48,    50,    50,    51,
      52,    53,    50,    54,    54,    55,    55,    55,    55,    55,
      56,    57,    58,    56,    59,    59,    59,    59,    59,    59,
      59,    59,    59,    60,    60,    61,    61,    62,    62,    63,
      63,    65,    66,    67,    64,    68,    69,    70,    71,    68,
      72,    73,    74,    75,    72,    76,    76,    76,    76,    76,
      76
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     0,     0,     8,     0,     0,     0,     7,
       3,     0,     5,     0,     6,     0,     5,     6,     6,     5,
       6,     5,     6,     5,     0,     0,     3,     0,     3,     0,
       0,     0,     9,     0,     2,     0,     2,     2,     5,     5,
       0,     0,     0,     7,     0,     3,     3,     3,     2,     3,
       2,     2,     2,     3,     4,     2,     2,     4,     2,     3,
       2,     0,     0,     0,     8,     0,     0,     0,     0,     9,
       0,     0,     0,     0,     9,     0,     3,     5,     5,     5,
       5
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     2,     1,     0,     3,     6,     4,     0,
       0,     0,     0,     0,    11,     7,    10,     5,    13,    24,
      15,    24,    12,    25,     8,    14,    33,     0,     0,     0,
       0,     0,     0,     0,    29,    28,    26,     0,     0,     0,
       0,     9,    34,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    30,     0,    19,     0,    21,     0,    23,     0,
      16,    35,    18,    20,    22,    17,    31,     0,     0,     0,
       0,    36,    37,    61,     0,     0,    32,     0,     0,     0,
       0,    41,    62,    38,    39,    44,    65,    42,    63,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    52,    48,
      50,    51,     0,     0,    45,    46,    47,    55,    56,    49,
       0,    58,     0,    60,     0,    43,    66,    64,     0,    59,
      53,     0,    57,    54,    67,    70,    68,     0,     0,    71,
      69,     0,    72,    75,    73,     0,     0,     0,    76,     0,
      74,     0,    78,    79,    80,    77
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     5,     7,    12,     8,    20,    33,    19,    21,
      24,    22,    28,    23,    43,    61,    70,    34,    66,    71,
      85,    97,    87,    98,    99,   100,   101,    72,    78,    86,
     103,    88,   121,   125,   128,   126,   131,   133,   137,   134
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -28
static const yytype_int8 yypact[] =
{
       1,     2,    24,   -28,   -28,    18,   -28,   -28,    28,   -12,
      20,    22,    -8,    21,   -28,   -28,   -28,   -28,   -28,    -5,
     -28,    -5,   -28,    -4,    12,   -28,   -28,    27,    11,    23,
      29,    30,    31,    14,    33,   -28,   -28,   -13,   -10,    -3,
      -2,   -28,   -28,    32,    34,    35,    36,    38,    37,    39,
      40,    42,   -28,    45,   -28,    46,   -28,    48,   -28,    49,
     -28,   -28,   -28,   -28,   -28,   -28,    -7,    50,    47,    51,
      52,    53,   -28,   -28,    54,    55,   -28,    56,    57,    58,
      59,   -28,   -28,   -28,   -28,   -28,   -28,    -6,    70,    60,
      63,    66,   -27,    25,    15,    17,    67,    62,   -28,   -28,
     -28,   -28,    69,    64,   -28,   -28,   -28,   -28,   -28,   -28,
      71,   -28,    72,   -28,    73,   -28,   -28,   -28,    75,   -28,
      76,    68,   -28,   -28,   -28,   -28,    61,    78,    74,   -28,
     -28,    77,   -28,   -28,    19,    79,    80,    81,   -28,    83,
     -28,    16,   -28,   -28,   -28,   -28
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -28,   -28,   -28,   -28,   -28,   -28,   -28,   -28,   -28,   -28,
     -28,    65,   -28,   -28,   -28,   -28,   -28,   -28,   -28,   -28,
     -28,   -28,   -28,   -28,   -28,   -28,   -28,   -28,   -28,   -28,
     -28,   -28,   -28,   -28,   -28,   -28,   -28,   -28,   -28,   -28
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -41
static const yytype_int16 yytable[] =
{
     -27,    26,    67,   107,     1,   -40,   108,    89,    90,    91,
      92,    93,    94,    95,    96,    68,    69,    13,    44,   -27,
      27,    46,    14,    45,     4,   135,    47,    17,    48,    50,
     136,     3,     9,    49,    51,    10,    11,    29,    30,    31,
      32,   142,   143,   144,   145,   110,    36,   112,   111,    41,
     113,    16,     6,    37,    15,    18,    35,     0,   109,    38,
      39,    40,    42,     0,    54,    77,    52,    56,    58,     0,
      53,    60,    55,    57,    62,    63,    59,    64,    65,    73,
     102,    74,   127,    79,    80,    75,    25,    76,     0,   104,
      81,    82,   105,    83,    84,   106,   114,   115,   116,   117,
     118,   119,   124,   120,   122,     0,   123,   129,   138,   130,
       0,   132,   139,   141,     0,     0,   140
};

static const yytype_int8 yycheck[] =
{
       5,     5,     9,    30,     3,    12,    33,    13,    14,    15,
      16,    17,    18,    19,    20,    22,    23,    29,    31,    24,
      24,    31,    34,    36,     0,     6,    36,    35,    31,    31,
      11,    29,     4,    36,    36,     7,     8,    25,    26,    27,
      28,    25,    26,    27,    28,    30,    35,    30,    33,    35,
      33,    29,    34,    30,    34,    34,    29,    -1,    33,    30,
      30,    30,    29,    -1,    29,    12,    34,    29,    29,    -1,
      36,    29,    36,    36,    29,    29,    36,    29,    29,    29,
      10,    34,    21,    29,    29,    34,    21,    35,    -1,    29,
      34,    34,    29,    35,    35,    29,    29,    35,    29,    35,
      29,    29,    34,    30,    29,    -1,    30,    29,    29,    35,
      -1,    34,    32,    30,    -1,    -1,    35
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    38,    29,     0,    39,    34,    40,    42,     4,
       7,     8,    41,    29,    34,    34,    29,    35,    34,    45,
      43,    46,    48,    50,    47,    48,     5,    24,    49,    25,
      26,    27,    28,    44,    54,    29,    35,    30,    30,    30,
      30,    35,    29,    51,    31,    36,    31,    36,    31,    36,
      31,    36,    34,    36,    29,    36,    29,    36,    29,    36,
      29,    52,    29,    29,    29,    29,    55,     9,    22,    23,
      53,    56,    64,    29,    34,    34,    35,    12,    65,    29,
      29,    34,    34,    35,    35,    57,    66,    59,    68,    13,
      14,    15,    16,    17,    18,    19,    20,    58,    60,    61,
      62,    63,    10,    67,    29,    29,    29,    30,    33,    33,
      30,    33,    30,    33,    29,    35,    29,    35,    29,    29,
      30,    69,    29,    30,    34,    70,    72,    21,    71,    29,
      35,    73,    34,    74,    76,     6,    11,    75,    29,    32,
      35,    30,    25,    26,    27,    28
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
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



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

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
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1455 of yacc.c  */
#line 72 "GenesisShaderBison.ycc"
    { //n_debuglog("init genesisshader\n");
									  g_GenesisMaterial->SetName((yyvsp[(2) - (2)].str));
										delete[] (yyvsp[(2) - (2)].str);
									  ResetParserParams();
									  g_curGenesisMakeMaterial = new GenesisMakeMaterial();
									}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 78 "GenesisShaderBison.ycc"
    { //n_debuglog("in genesisshader,left\n"); 
									}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 80 "GenesisShaderBison.ycc"
    { //n_debuglog("from PropertySection to genesisshader\n"); 
									}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 82 "GenesisShaderBison.ycc"
    { //n_debuglog("out genesisshader,right\n");
										g_GenesisMaterial->AddMaterial(*g_curGenesisMakeMaterial);
										delete	g_curGenesisMakeMaterial;
										g_curGenesisMakeMaterial = 0;
									 }
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 87 "GenesisShaderBison.ycc"
    {//n_debuglog("init PropertySection\n");
									}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 89 "GenesisShaderBison.ycc"
    {//n_debuglog("in ParameterSection,left\n"); 
														}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 91 "GenesisShaderBison.ycc"
    {//n_debuglog("from ParameterSection to PropertySection\n");
									}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 93 "GenesisShaderBison.ycc"
    { //n_debuglog("out ParameterSection,right\n");
									}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 95 "GenesisShaderBison.ycc"
    {	g_curGenesisMakeMaterial->SetRenderQueue(Graphic::RenderQueue::FromString((yyvsp[(3) - (3)].str)));
															//n_debuglog("in PropertySection,setrenderqueue:%s\n", Util::String($3).AsCharPtr()); 
														}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 98 "GenesisShaderBison.ycc"
    {	//n_debuglog("in TechniqueSection,left\n");
														g_curGenesisMakeTechnique = new GenesisMakeTechnique(); 
													}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 101 "GenesisShaderBison.ycc"
    {//n_debuglog("from TechniqueSection to PropertySection\n");
									}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 103 "GenesisShaderBison.ycc"
    {	//n_debuglog("in TechniqueSection,left\n");
														g_curGenesisMakeTechnique = new GenesisMakeTechnique();
														g_curGenesisMakeTechnique->SetName((yyvsp[(3) - (4)].str)); 
													}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 107 "GenesisShaderBison.ycc"
    {//n_debuglog("from TechniqueSection to PropertySection\n");
									}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 109 "GenesisShaderBison.ycc"
    {//n_debuglog("init ParameterSection\n");
										}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 111 "GenesisShaderBison.ycc"
    { if((yyvsp[(2) - (5)].spt) == Graphic::eShaderParamTexture2D)
																		{
																			g_curMatParam = new Graphic::MaterialParamTex2D();
																		}
																		else if((yyvsp[(2) - (5)].spt) == Graphic::eShaderParamTextureCUBE)
																		{
																			g_curMatParam = new Graphic::MaterialParamTexCube();
																		}
																		else if((yyvsp[(2) - (5)].spt) == Graphic::eShaderParamTexture1D)
																		{
																			g_curMatParam = new Graphic::MaterialParamTex1D();
																		}
																		else if((yyvsp[(2) - (5)].spt) == Graphic::eShaderParamTexture3D)
																		{
																			g_curMatParam = new Graphic::MaterialParamTex3D();
																		}
																		else
																		{
																			n_error("GenesisShader Parser : Invalid Texture Type !");
																		}
																		
																		g_curMatParam->SetName((yyvsp[(3) - (5)].str));
																		g_curMatParam->SetDesc((yyvsp[(3) - (5)].str));
																		g_curMatParam->SetStringValue((yyvsp[(5) - (5)].str));

																		if ( g_curMatParam->GetStringValue() == "#UserDefTex")
																		{
																			g_curMatParam->SetHiddenInEditor(true);
																		}

																		//n_debuglog("define texture\n");
																		g_curGenesisMakeMaterial->AddMatParam(g_curMatParam);
																		g_curMatParam = NULL;

                                                                        if ((yyvsp[(2) - (5)].spt) == Graphic::eShaderParamTexture2D)
                                                                        {
																		char texOffestScaleValue[] = "0.0,0.0,1.0,1.0";
																		Util::String texOffestScale;
																		texOffestScale.Clear();
																		texOffestScale.Format("%s_UV_OffsetScale",(yyvsp[(3) - (5)].str));

																		g_curMatParam = new Graphic::MaterialParamVector();
																		g_curMatParam->SetName(texOffestScale);
																		g_curMatParam->SetDesc(texOffestScale);
																		g_curMatParam->SetStringValue(texOffestScaleValue);
																		g_curGenesisMakeMaterial->AddMatParam(g_curMatParam);
																		g_curMatParam = NULL;
                                                                        }

																		}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 155 "GenesisShaderBison.ycc"
    { if((yyvsp[(2) - (6)].spt) == Graphic::eShaderParamTexture2D)
																		{
																			g_curMatParam = new Graphic::MaterialParamTex2D();
																		}
																		else if((yyvsp[(2) - (6)].spt) == Graphic::eShaderParamTextureCUBE)
																		{
																			g_curMatParam = new Graphic::MaterialParamTexCube();
																		}
																		else if((yyvsp[(2) - (6)].spt) == Graphic::eShaderParamTexture1D)
																		{
																			g_curMatParam = new Graphic::MaterialParamTex1D();
																		}
																		else if((yyvsp[(2) - (6)].spt) == Graphic::eShaderParamTexture3D)
																		{
																			g_curMatParam = new Graphic::MaterialParamTex3D();
																		}
																		else
																		{
																			n_error("GenesisShader Parser : Invalid Texture Type !");
																		}
																		
																		g_curMatParam->SetName((yyvsp[(3) - (6)].str));
																		g_curMatParam->SetDesc((yyvsp[(4) - (6)].str));
																		g_curMatParam->SetStringValue((yyvsp[(6) - (6)].str));

																		if ( g_curMatParam->GetStringValue() == "#UserDefTex")
																		{
																			g_curMatParam->SetHiddenInEditor(true);
																		}

																		//n_debuglog("define texture\n");
																		g_curGenesisMakeMaterial->AddMatParam(g_curMatParam);
																		g_curMatParam = NULL;

                                                                        if ((yyvsp[(2) - (6)].spt) == Graphic::eShaderParamTexture2D)
                                                                        {
																		char texOffestScaleValue[] = "0.0,0.0,1.0,1.0";
																		Util::String texOffestScale;
																		texOffestScale.Clear();
																		texOffestScale.Format("%s_UV_OffsetScale",(yyvsp[(3) - (6)].str));

																		g_curMatParam = new Graphic::MaterialParamVector();
																		g_curMatParam->SetName(texOffestScale);
																		g_curMatParam->SetDesc(texOffestScale);
																		g_curMatParam->SetStringValue(texOffestScaleValue);
																		g_curGenesisMakeMaterial->AddMatParam(g_curMatParam);
																		g_curMatParam = NULL;
                                                                        }

																		}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 199 "GenesisShaderBison.ycc"
    { g_curMatParam = new Graphic::MaterialParamMatrix();
																		g_curMatParam->SetName((yyvsp[(3) - (6)].str));
																		g_curMatParam->SetDesc((yyvsp[(4) - (6)].str));
																		g_curMatParam->SetStringValue((yyvsp[(6) - (6)].str));
																		
																		g_curGenesisMakeMaterial->AddMatParam(g_curMatParam);
																		g_curMatParam = NULL;
																		}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 207 "GenesisShaderBison.ycc"
    { g_curMatParam = new Graphic::MaterialParamMatrix();
																		g_curMatParam->SetName((yyvsp[(3) - (5)].str));
																		g_curMatParam->SetDesc((yyvsp[(3) - (5)].str));
																		g_curMatParam->SetStringValue((yyvsp[(5) - (5)].str));
																		
																		g_curGenesisMakeMaterial->AddMatParam(g_curMatParam);
																		g_curMatParam = NULL;
																		}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 215 "GenesisShaderBison.ycc"
    { g_curMatParam = new Graphic::MaterialParamVector();
																		g_curMatParam->SetName((yyvsp[(3) - (6)].str));
																		g_curMatParam->SetDesc((yyvsp[(4) - (6)].str));
																		g_curMatParam->SetStringValue((yyvsp[(6) - (6)].str));

																		if (
																			Util::String::MatchPattern(g_curMatParam->GetName(), "*Color*")
																			|| Util::String::MatchPattern(g_curMatParam->GetName(), "*color*")
																			|| Util::String::MatchPattern(g_curMatParam->GetName(), "*emissive*")
																			|| Util::String::MatchPattern(g_curMatParam->GetName(), "*specular*")
																			|| Util::String::MatchPattern(g_curMatParam->GetName(), "*diffuse*")
																			)
																		{
																			g_curMatParam->SetUseForColor(true);
																		}

																		
																		g_curGenesisMakeMaterial->AddMatParam(g_curMatParam);
																		g_curMatParam = NULL;
																		}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 223 "GenesisShaderBison.ycc"
    { g_curMatParam = new Graphic::MaterialParamVector();
																		g_curMatParam->SetName((yyvsp[(3) - (5)].str));
																		g_curMatParam->SetDesc((yyvsp[(3) - (5)].str));
																		g_curMatParam->SetStringValue((yyvsp[(5) - (5)].str));
																		
																		if (
																			Util::String::MatchPattern(g_curMatParam->GetName(), "*Color*")
																			|| Util::String::MatchPattern(g_curMatParam->GetName(), "*color*")
																			|| Util::String::MatchPattern(g_curMatParam->GetName(), "*emissive*")
																			|| Util::String::MatchPattern(g_curMatParam->GetName(), "*specular*")
																			|| Util::String::MatchPattern(g_curMatParam->GetName(), "*diffuse*")
																			)
																		{
																			g_curMatParam->SetUseForColor(true);
																		}

																		g_curGenesisMakeMaterial->AddMatParam(g_curMatParam);
																		g_curMatParam = NULL;
																		}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 231 "GenesisShaderBison.ycc"
    { g_curMatParam = new Graphic::MaterialParamFloat();
																		g_curMatParam->SetName((yyvsp[(3) - (6)].str));
																		g_curMatParam->SetDesc((yyvsp[(4) - (6)].str));
																		g_curMatParam->SetStringValue((yyvsp[(6) - (6)].str));
																			
																		g_curGenesisMakeMaterial->AddMatParam(g_curMatParam);
																		g_curMatParam = NULL;
																		}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 239 "GenesisShaderBison.ycc"
    { g_curMatParam = new Graphic::MaterialParamFloat();
																		g_curMatParam->SetName((yyvsp[(3) - (5)].str));
																		g_curMatParam->SetDesc((yyvsp[(3) - (5)].str));
																		g_curMatParam->SetStringValue((yyvsp[(5) - (5)].str));
																			
																		g_curGenesisMakeMaterial->AddMatParam(g_curMatParam);
																		g_curMatParam = NULL;
																		}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 248 "GenesisShaderBison.ycc"
    { //n_debuglog("init TechniqueSection\n");
									 }
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 250 "GenesisShaderBison.ycc"
    { //n_debuglog("from PassSection to TechniqueSection\n"); 
									}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 252 "GenesisShaderBison.ycc"
    { //n_debuglog("out TechniqueSection,right\n");
									  g_curGenesisMakeMaterial->AddTechnique(*g_curGenesisMakeTechnique);
										delete g_curGenesisMakeTechnique;
										g_curGenesisMakeTechnique = 0; 
									}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 258 "GenesisShaderBison.ycc"
    { //n_debuglog("init PassSection\n");
									}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 260 "GenesisShaderBison.ycc"
    { printf("set MatType\n");
													g_curGenesisMakeTechnique->SetIsMatTemplate((yyvsp[(3) - (3)].str));
												}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 263 "GenesisShaderBison.ycc"
    { //n_debuglog("init Pass\n");
												}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 265 "GenesisShaderBison.ycc"
    { //n_debuglog("in PassSection,left\n");
									  
									}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 268 "GenesisShaderBison.ycc"
    { //n_debuglog("from codeSection to PassSection\n");
									}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 270 "GenesisShaderBison.ycc"
    {
										//n_debuglog("out PassSection,right\n");
										g_curGenesisMakeTechnique->AddPass(*g_curMakePass); 
										delete g_curMakePass;
										g_curMakePass = 0; 
									}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 277 "GenesisShaderBison.ycc"
    {
											n_debuglog("in PassSection,left\n");
											g_curMakePass = new GenesisMakePass(); 
											g_curMakePass->SetName("NoName");
										}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 282 "GenesisShaderBison.ycc"
    {
											n_debuglog("in PassSection,left\n");
											g_curMakePass = new GenesisMakePass(); 
											g_curMakePass->SetName((yyvsp[(2) - (2)].str));
										}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 288 "GenesisShaderBison.ycc"
    { //n_debuglog("in codeSection\n");
									}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 290 "GenesisShaderBison.ycc"
    { //n_debuglog("from shadertype,to StateSection\n"); 
									}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 292 "GenesisShaderBison.ycc"
    { //n_debuglog("from shadertype,to shadertype\n"); 
									}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 294 "GenesisShaderBison.ycc"
    {
																g_curMakePass->SetBuiltInMacro((yyvsp[(4) - (5)].str));
																n_debuglog("set builtinMacro\n"); 
															}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 298 "GenesisShaderBison.ycc"
    {
																g_curMakePass->SetCustumMacro((yyvsp[(4) - (5)].str));
																n_debuglog("set builtinMacro\n");
															}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 303 "GenesisShaderBison.ycc"
    { //n_debuglog("in StateSection\n");
									}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 305 "GenesisShaderBison.ycc"
    { 
												g_rsDesc = RenderBase::RenderStateDesc::Create();
												g_rsDesc->Setup(); 
												//n_debuglog("Create StateSection\n");//n_debuglog("init StateSection\n");
											}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 310 "GenesisShaderBison.ycc"
    { }
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 311 "GenesisShaderBison.ycc"
    {
												g_curMakePass->SetRenderStateDesc(g_rsDesc);
												g_rsDesc = 0;
												//n_debuglog("from RenderStateSetup,to shadertype\n");
											}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 317 "GenesisShaderBison.ycc"
    { //n_debuglog("in RenderStateSetup\n");
									}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 319 "GenesisShaderBison.ycc"
    {   RenderBase::DeviceRasterizerState rrs = g_rsDesc->GetRasterizerState();
															rrs.m_cullMode = RenderBase::CullModeConverter::FromString((yyvsp[(3) - (3)].str));	
															
															g_rsDesc->SetRasterizerState(rrs);
														}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 324 "GenesisShaderBison.ycc"
    {   RenderBase::DeviceRasterizerState rrs = g_rsDesc->GetRasterizerState();
															rrs.m_fillMode = RenderBase::FillModeConverter::FromString((yyvsp[(3) - (3)].str));	
															
															g_rsDesc->SetRasterizerState(rrs);
														}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 329 "GenesisShaderBison.ycc"
    {   RenderBase::DeviceBlendState rbs = g_rsDesc->GetBlendState();
															rbs.m_colorWriteMask[0] = RenderBase::ColorMaskConverter::FromString((yyvsp[(3) - (3)].str));	
															
															g_rsDesc->SetBlendState(rbs);
														}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 334 "GenesisShaderBison.ycc"
    { //n_debuglog("set depthtest complete \n");
														}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 336 "GenesisShaderBison.ycc"
    {   RenderBase::DeviceDepthAndStencilState rdss = g_rsDesc->GetDepthAndStencilState();
																rdss.m_depthWriteMask = (yyvsp[(3) - (3)].boolean);	
																
																g_rsDesc->SetDepthAndStencilState(rdss);
															}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 341 "GenesisShaderBison.ycc"
    {  //n_debuglog("set blendmode complete \n"); 
														}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 343 "GenesisShaderBison.ycc"
    { //n_debuglog("set alphatest complete \n");
														}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 345 "GenesisShaderBison.ycc"
    { //n_debuglog("set samplerstate complete \n");
														}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 348 "GenesisShaderBison.ycc"
    { g_curGenesisMakeMaterial->AddTextureSampler((yyvsp[(2) - (3)].str),RenderBase::TextureAddressModeConverter::FromString((yyvsp[(3) - (3)].str)));
														 
														}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 351 "GenesisShaderBison.ycc"
    {
																	g_curGenesisMakeMaterial->AddTextureSampler((yyvsp[(2) - (4)].str),RenderBase::TextureAddressModeConverter::FromString((yyvsp[(3) - (4)].str)),RenderBase::TextureFilterOperationConverter::FromString((yyvsp[(4) - (4)].str)));
																	
																}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 355 "GenesisShaderBison.ycc"
    {   
															RenderBase::DeviceDepthAndStencilState rdss = g_rsDesc->GetDepthAndStencilState();
															rdss.m_depthEnable = true;
															rdss.m_zFunc = RenderBase::CompareFunctionConverter::FromString((yyvsp[(2) - (2)].str));			
															
															g_rsDesc->SetDepthAndStencilState(rdss);
											}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 362 "GenesisShaderBison.ycc"
    {
															RenderBase::DeviceDepthAndStencilState rdss = g_rsDesc->GetDepthAndStencilState();
															rdss.m_depthEnable = (yyvsp[(2) - (2)].boolean);		
															
															g_rsDesc->SetDepthAndStencilState(rdss);
											}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 369 "GenesisShaderBison.ycc"
    {
																	RenderBase::DeviceBlendState rbs = g_rsDesc->GetBlendState();
																	rbs.m_alphaBlendEnable[0] = true;
																	rbs.m_blendOP[0] = RenderBase::BlendOperationConverter::FromString((yyvsp[(2) - (4)].str));
																	rbs.m_srcBlend[0] = RenderBase::AlphaBlendFactorConverter::FromString((yyvsp[(3) - (4)].str));
																	rbs.m_destBlend[0] = RenderBase::AlphaBlendFactorConverter::FromString((yyvsp[(4) - (4)].str));	
																	
																	g_rsDesc->SetBlendState(rbs);		
																}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 378 "GenesisShaderBison.ycc"
    {
																	RenderBase::DeviceBlendState rbs = g_rsDesc->GetBlendState();
																	rbs.m_alphaBlendEnable[0] = (yyvsp[(2) - (2)].boolean);
																	
																	g_rsDesc->SetBlendState(rbs);		
																}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 385 "GenesisShaderBison.ycc"
    {
																	RenderBase::DeviceBlendState rbs = g_rsDesc->GetBlendState();
																	rbs.m_alphaTestEnable = true;
																	rbs.m_alphaFunc = RenderBase::CompareFunctionConverter::FromString((yyvsp[(2) - (3)].str));
																	const Util::String& valueStr = g_curGenesisMakeMaterial->GetMatParamValueByName((yyvsp[(3) - (3)].str));
																	if(!valueStr.IsValidFloat() || valueStr.IsEmpty())
																	{
																		n_error("Invalid alpha_to_coverage_ref value!please check your parameter type(float) and name!");
																	}
																	else
																	{
																		rbs.m_alphaRef = valueStr.AsFloat();
																	}
																	
																	
																	g_rsDesc->SetBlendState(rbs);
																}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 402 "GenesisShaderBison.ycc"
    {
																	RenderBase::DeviceBlendState rbs = g_rsDesc->GetBlendState();
																	rbs.m_alphaTestEnable = (yyvsp[(2) - (2)].boolean);
																	
																	g_rsDesc->SetBlendState(rbs);
																}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 409 "GenesisShaderBison.ycc"
    {
											g_curGenesisMakeGPUProgram = new GenesisMakeGPUProgram();
											g_curGenesisMakeGPUProgram->SetShaderType((yyvsp[(2) - (2)].str));
											//n_debuglog("in shaderType,SetShaderType\n");
											delete[] (yyvsp[(2) - (2)].str);
										}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 415 "GenesisShaderBison.ycc"
    { //n_debuglog("in shaderType,left\n");
										}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 417 "GenesisShaderBison.ycc"
    { //n_debuglog("from DeviceTypeSetup to shaderType\n");
										}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 419 "GenesisShaderBison.ycc"
    { 
											if(g_curGenesisMakeGPUProgram != NULL)
											{
												delete g_curGenesisMakeGPUProgram;
												g_curGenesisMakeGPUProgram = NULL;
											}
											//n_debuglog("out shaderType,right\n");
										}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 428 "GenesisShaderBison.ycc"
    {	n_debuglog("in DeviceTypeSetup\n");}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 429 "GenesisShaderBison.ycc"
    { 
											g_curGenesisMakeGPUProgram->SetDeviceType((yyvsp[(3) - (3)].str));
											n_debuglog("in DeviceTypeSetup\n");
											delete[] (yyvsp[(3) - (3)].str);
											}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 434 "GenesisShaderBison.ycc"
    { n_debuglog("in DeviceTypeSetup,left\n");
										}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 436 "GenesisShaderBison.ycc"
    { n_debuglog("from SubGPUProgramSetup to DeviceTypeSetup\n");
										}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 438 "GenesisShaderBison.ycc"
    { n_debuglog("out DeviceTypeSetup,right\n");
										  g_curMakePass->AddShaderProgram(*g_curGenesisMakeGPUProgram);
										}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 441 "GenesisShaderBison.ycc"
    {	n_debuglog("in empty SubGPUProgramSetup\n");}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 442 "GenesisShaderBison.ycc"
    { n_debuglog("in SubGPUProgramSetup\n");
													}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 444 "GenesisShaderBison.ycc"
    { n_debuglog("in SubGPUProgramSetup,left\n");
										  g_curGenesisSubGPUProgram = new GenesisSubGPUProgram();
										  g_curGenesisSubGPUProgram->SetShaderMask((yyvsp[(3) - (5)].str));
										}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 448 "GenesisShaderBison.ycc"
    { n_debuglog("from CodeBlock to SubGPUProgramSetup\n");
										}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 450 "GenesisShaderBison.ycc"
    { n_debuglog("out SubGPUProgramSetup,right\n");
										  g_curGenesisMakeGPUProgram->AddSubGpuProgram(*g_curGenesisSubGPUProgram);
										  if(g_curGenesisSubGPUProgram != NULL)
											{
												delete g_curGenesisSubGPUProgram;
												g_curGenesisSubGPUProgram = NULL;
											}
										}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 459 "GenesisShaderBison.ycc"
    { n_debuglog("in CodeBlock\n");
										}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 461 "GenesisShaderBison.ycc"
    { 
												g_curGenesisSubGPUProgram->SetShaderCode((yyvsp[(3) - (3)].str));
												n_debuglog("in CodeBlock,AddGPUProgram\n");
												delete[] (yyvsp[(3) - (3)].str);
											 }
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 466 "GenesisShaderBison.ycc"
    { g_curShaderParameter = new Graphic::ShaderParam();
																		  g_curShaderParameter->SetParamType((yyvsp[(5) - (5)].spt));
																		  g_curShaderParameter->SetRegister((yyvsp[(3) - (5)].num));
																		  g_curShaderParameter->SetName((yyvsp[(4) - (5)].str));
																		  g_curGenesisSubGPUProgram->AddParam(*g_curShaderParameter);
																		  n_debuglog("bind texture\n"); delete[] (yyvsp[(4) - (5)].str);
																		  delete g_curShaderParameter;
																		  g_curShaderParameter = 0;
																		 }
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 475 "GenesisShaderBison.ycc"
    {	g_curShaderParameter = new Graphic::ShaderParam();
																			g_curShaderParameter->SetParamType((yyvsp[(5) - (5)].spt));
																			g_curShaderParameter->SetRegister((yyvsp[(3) - (5)].num));
																			g_curShaderParameter->SetName((yyvsp[(4) - (5)].str));
																			g_curGenesisSubGPUProgram->AddParam(*g_curShaderParameter);
																			//n_debuglog("setparam matrix register\n"); delete[] $4;
																			delete g_curShaderParameter;
																			g_curShaderParameter = 0;
																		}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 484 "GenesisShaderBison.ycc"
    {	g_curShaderParameter = new Graphic::ShaderParam();
																			g_curShaderParameter->SetParamType((yyvsp[(5) - (5)].spt));
																			g_curShaderParameter->SetRegister((yyvsp[(3) - (5)].num));
																			g_curShaderParameter->SetName((yyvsp[(4) - (5)].str));
																			g_curGenesisSubGPUProgram->AddParam(*g_curShaderParameter);
																			//n_debuglog("setparam vector register\n"); delete[] $4;
																			delete g_curShaderParameter;
																			g_curShaderParameter = 0;
																		}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 493 "GenesisShaderBison.ycc"
    {	g_curShaderParameter = new Graphic::ShaderParam();
																			g_curShaderParameter->SetParamType((yyvsp[(5) - (5)].spt));
																			g_curShaderParameter->SetRegister((yyvsp[(3) - (5)].num));
																			g_curShaderParameter->SetName((yyvsp[(4) - (5)].str));
																			g_curGenesisSubGPUProgram->AddParam(*g_curShaderParameter);
																			//n_debuglog("setparam float register\n"); delete[] $4;
																			delete g_curShaderParameter;
																			g_curShaderParameter = 0;
																		}
    break;



/* Line 1455 of yacc.c  */
#line 2453 "GenesisShaderBison.cpp"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
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
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 504 "GenesisShaderBison.ycc"


int yyerror (const char *s)
{
	n_debuglog("GenesisShader Error: %s At line:%d\n",s,Genesislineno);
	return 0;
}

void ResetParserParams()
{

	if(g_curGenesisMakeGPUProgram != NULL)
	{
		delete g_curGenesisMakeGPUProgram;
		g_curGenesisMakeGPUProgram = NULL;
	}

	if(g_curGenesisSubGPUProgram != NULL)
	{
		delete g_curGenesisSubGPUProgram;
		g_curGenesisSubGPUProgram = NULL;
	}
	
	if(g_curShaderParameter != NULL)
	{
		delete g_curShaderParameter;
		g_curShaderParameter = NULL;
	}
	
	if(g_curMatParam != NULL)
	{
		delete g_curMatParam;
		g_curMatParam = NULL;
	}
	
	if(g_rsDesc.isvalid())
	{
		g_rsDesc = 0;
	}
	
	if(g_curMakePass != NULL)
	{
		delete g_curMakePass;
		g_curMakePass = NULL;
	}
	
	if(g_curGenesisMakeTechnique != NULL)
	{
		delete g_curGenesisMakeTechnique;
		g_curGenesisMakeTechnique = NULL;
	}
	
	if(g_curGenesisMakeMaterial != NULL)
	{
		delete g_curGenesisMakeMaterial;
		g_curGenesisMakeMaterial = NULL;
	}

}

