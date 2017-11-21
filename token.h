#include <stdio.h>
#include "boolean.h"
#include <string.h>

#ifndef TOKEN_H
#define TOKEN_H

typedef struct {
	char symbol[3]; 
	char string[20];
	int line;
} Token;

#define Symbol(P) (P).symbol
#define String(P) (P).string
#define Line(P) (P).line


void assign_symbol(Token *T);
/*I.S. Token sembarang yang dikenali stringnya */
/*F.S. Symbol token akan diassign sesuai string token tersebut*/

#endif