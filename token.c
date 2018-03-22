#include "token.h"
#include "lexUtils.h"

void printToken(token* t, FILE* outputFile){
    if (t != NULL){
        fprintf(outputFile, "\tToken of type %s was found at line: %d, lexeme: \"%s\"\r\n", toString(t->type), t->line, t->lexeme);
    }
}

char* toString(int i) {
	char* tokenTypes[] = {
		"End",\
		"Program",\
		"Real",\
		"Integer",\
		"If",\
		"Then",\
		"End if",\
		"Loop",\
		"Until",\
		"End loop",\
		"Start",\
		"Else",\
		"Letter",\
		"Arithmetic operation",\
		"Real operation",\
		"Id",\
		"Integer number",\
		"Number",\
		"Separation sign",\
		"Comma",\
		"Assigment",\
		"Increment",\
		"Multiplication",\
		"Smaller than",\
		"Greater than",\
		"Equal or Greater Than",\
		"Equal or Smaller Than",\
		"Not Equal",\
		"Equal",\
		"Colon",\
		"Semicolon",\
		"Left bracket",\
		"Right bracket"
		};
	return tokenTypes[i];
}