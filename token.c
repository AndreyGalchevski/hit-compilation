#include "token.h"
#include "lexUtils.h"

void printToken(token* t, FILE* outputFile){
    if (t != NULL){
        fprintf(outputFile, "\t-In line: %d, appears a token of type: %s. Token is: \"%s\"\r\n", t->line, toString(t->type), t->lexeme);
    }
}

char* toString(int i) {
	char* map[]={"End", "Program", "Real", "Integer", "If", "Then",\
				"End if", "Loop", "Until", "End loop", "Start", "Else",\
				"Letter", "Arithmetic operation", "Real operation",\
				"Id", "Integer number", "Number", "Separation sign", "Comma",\
				"Assigment", "Increment", "Multiplication", "Smaller than",\
				"Greater than", "Equal or Greater Than", "Equal or Smaller Than",\
				"Not Equal", "Equal", "Colon", "Semicolon", "Left bracket", "Right bracket"};
	return map[i];
}