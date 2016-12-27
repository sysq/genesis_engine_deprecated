
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
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

/* Line 1676 of yacc.c  */
#line 33 "GenesisShaderBison.ycc"

	char* str;
	float num;
	Graphic::ShaderParamType spt;
	bool boolean;



/* Line 1676 of yacc.c  */
#line 127 "GenesisShaderBison.hpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE Genesislval;


