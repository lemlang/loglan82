/*     Loglan82 Compiler&Interpreter
     Copyright (C) 1993 Institute of Informatics, University of Warsaw
     Copyright (C)  1993, 1994 LITA, Pau

     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation; either version 2 of the License, or
     (at your option) any later version.

     This program is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU General Public License for more details.

             You should have received a copy of the GNU General Public License
             along with this program; if not, write to the Free Software
             Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

 contacts:  Andrzej.Salwicki@univ-pau.fr

or             Andrzej Salwicki
                LITA   Departement d'Informatique
                Universite de Pau
                Avenue de l'Universite
                64000 Pau   FRANCE
		 tel.  ++33 59923154	fax. ++33 59841696

=======================================================================
*/

#include <stdio.h>
#include <malloc.h>
#include <setjmp.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
    char opcode;
    char args[ 3 ];
} extopcode;

typedef char *lword;	/* max(word, char *) but in every case was char* */

#define BINARYREAD	"rb"
#define BINARYWRITE	"wb"
#define DIRECTOLD	"r+b"
#define DIRECTNEW	"w+b"

typedef int word;
typedef float real;
typedef word *memory;

#define mallocate(n)	((memory) (char *) calloc((n),sizeof(word)))
#define ballocate(n)	((char *) calloc((n),1))

/* printf("|%d*4|",(n)),getchar(),\ */

#define MAXINTEGER   0x7FFFFFFFL
#define DEFMEMSIZE   0x13C00L	/* 79K words = 316K bytes */
#define MAXMEMSIZE   0x400000L	/*  4M words =  16M bytes */
