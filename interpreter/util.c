/* Loglan82 Compiler&Interpreter
Copyright (C) 1981-1993 Institute of Informatics, University of Warsaw
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
#include "depend.h"
#include "genint.h"
#include "int.h"
#include "process.h"
#include "intproto.h"
#include <time.h>
/* Utility routines */

/* Compute entier (floor) */
word entier ( double x ) {
    word i;

    if ( x >= 0.0 ) {
        i = ( word ) x;
        return ( i );
    } else {
        i = ( word ) ( -x );
        i = -i;
        if ( ( double ) i <= x ) return ( i );
        else return ( i-1 );
    }
} /* end entier */

/* shift x by n bits */
word shift ( word x, word n ) {
    if ( n == 0 ) {
        return ( x );
    }
    if ( n > 0 ) {
        return ( x << n );
    } else {
        return ( ( x >> -n ) & ~ ( ~ ( word ) 0 << ( 8*sizeof ( word ) +n ) ) );
    }
} /* end shift */

/* Get ASCIIZ string from arrayof char */
char *asciiz ( virtaddr *virt ) {
    word am;
    int len, i;
    char *cp;

    if ( member ( virt, &am ) ) {
        len = M[ am ]-3;		/* length of the string */
        cp = ballocate ( len+1 );		/* allocate buffer for the string */
        if ( cp == NULL ) errsignal ( RTEMEMOV );
        for ( i = 0;  i < len;  i++ ) cp[ i ] = ( char ) M[ am+3+i ];
        cp[ len ] = '\0';		/* terminate string with 0 byte */
        return ( cp );
    } else  {
        errsignal ( RTEREFTN );		/* reference to none */
    }
} /* end asciiz */

/* Add extension to a file name */
void addext ( char *fname,char *ext ) {
    char *cp;
    cp = fname;
    while ( *cp != '\0' && *cp != '.' ) {
        cp++;
    }
    strcpy ( cp, ext );
} /* end addext */

/* Print error message and abort */
void abend ( char *msg ) {
    fprintf ( stderr, "Error: %s\n", msg );
    exit ( 8 );
} /* end abend */

/* Pseudo random number generator */
static int ranpat1 = 7, ranpat2 = 503, ranpat3 = 15661;

/* Initialize generator */
void ranset() {
    long tim;
    int i;

    time ( &tim );
    ranpat1 = tim % 30269;
    ranpat2 = tim % 30307;
    ranpat3 = tim % 30323;
} /* end ranset */

/* Produce next pseudo random number */
double prandom() {
    double r;
    int i;

    ranpat1 = 171* ( ranpat1 % 177 )- 2* ( ranpat1 / 177 );
    if ( ranpat1 < 0 ) ranpat1 += 30269;
    ranpat2 = 172* ( ranpat2 % 176 )-35* ( ranpat2 / 176 );
    if ( ranpat2 < 0 ) ranpat2 += 30307;
    ranpat3 = 170* ( ranpat3 % 178 )-63* ( ranpat3 / 178 );
    if ( ranpat3 < 0 ) ranpat3 += 30323;
    r = ranpat1/30269.0 + ranpat2/30307.0 + ranpat3/30323.0;
    i = ( int ) r;
    return ( r-i );
}

/* Copy a block of memory */
void moveblock ( char *from,char *to, word len ) {
    while ( len-- > 0 ) *to++ = *from++;
} /* end moveblock */
