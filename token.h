#ifndef TOKEN
#define TOKEN

#include <stdio.h>
#include <string.h>

#define	END	0
#define	PROGRAM	1
#define	REAL	2
#define	INT	3
#define	IF	4
#define	THEN	5
#define	END_IF	6
#define	LOOP	7
#define	UNTIL	8
#define	END_LOOP	9
#define	START	10
#define	ELSE	11
#define	LETTER	12
#define	ARITH_OP	13
#define	REL_OP	14
#define	ID	15
#define	INT_NUMBER	16
#define	NUMBER	17
#define	SEPARATION_SIGN	18
#define	COMMA	19
#define	ASSIGNMENT	20
#define	INC	21
#define	MUL	22
#define	SMALLER_THAN	23
#define	GREATER_THAN	24
#define	EQUAL_GREATER_THAN	25
#define	EQUAL_SMALLER_THAN	26
#define	NOT_EQUAL	27
#define	EQUAL	28
#define	COLON	29
#define	SEMICOLON	30
#define	LEFT_BRACKET	31 
#define	RIGHT_BRACKET	32


typedef struct {
    int type;
    char* lexeme;
    int line;
} token;

char* toString(int);
void printToken(token*, FILE*);

#endif