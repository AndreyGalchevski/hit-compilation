#ifndef TOKEN
#define TOKEN

#include <stdio.h>
#include <string.h>

#define	BLOCK_T	                0
#define	BEGIN_T	                1
#define	END_T	                2
#define	TYPE_T	                3
#define	IS_T	                4
#define	INTEGER_T	            5
#define	REAL_T	                6
#define	ARRAY_T	                7
#define	OF_T	                8
#define	WHEN_T	                9
#define	DO_T	                10
#define	DEFAULT_T	            11
#define	END_WHEN_T              12
#define	FOR_T	                13
#define	END_FOR_T	            14
#define	MALLOC_T	            15
#define	SIZE_OF_T	            16
#define	FREE_T	                17
#define	AR_OP_T	                18
#define	ADDITION_T	            19
#define	SUBTRACTION_T	        20
#define	MULTIPLICATION_T	    21
#define	DIVISION_T	            22
#define	POWER_T	                23
#define	INCREMENT_T	            24
#define	REL_OP_T	            25
#define	SMALLER_THAN_T	        26
#define	GREATER_THAN_T	        27
#define	SMALLER_EQUAL_THAN_T	28
#define	GREATER_EQUAL_THAN_T	29
#define	NOT_EQUAL_T	            30
#define	EQUAL_T	                31
#define	ASSIGNMENT_T	        32
#define	POINTER_T	            33
#define	ADDRESS_T	            34
#define	ID_T	                35
#define	INT_NUM_T	            36
#define	REAL_NUM_T	            37
#define	SEPARATION_SIGN_T       38
#define	COLON_T	                39
#define	SEMICOLON_T	            40
#define	LEFT_PARENTHESIS_T  	41 
#define	RIGHT_PARENTHESIS_T	    42
#define	LEFT_BRACKET_T	        43 
#define	RIGHT_BRACKET_T	        44
#define	EOF_T	                45
#define ERROR_TYPE_T			46

typedef struct {
    int kind;
    char* lexeme;
    int line;
} token;

char* toString(int);

#endif