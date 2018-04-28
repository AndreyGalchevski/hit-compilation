#include "token.h"
#include "lexUtils.h"

char* toString(int i) {
	char* tokenKinds[] = {
		"Block",\
		"Begin",\
		"End",\
		"Type",\
		"Is",\
		"Integer",\
		"Real",\
		"Array",\
		"Of",\
		"When",\
		"Do",\
		"Default",\
		"End when",\
		"For",\
		"End for",\
		"Malloc",\
		"Size of",\
		"Free",\
		"Arithmetic operation",\
		"Addition",\
		"Subtraction",\
		"Multiplication",\
		"Division",\
		"Power",\
		"Increment",\
		"Comparison operation",\
		"Smaller than",\
		"Greater than",\
		"Smaller or equal than",\
		"Greater or equal than",\
		"Not equal",\
		"Equal",\
		"Assigment",\
		"Pointer operator",\
		"Address operator",\
		"Id",\
		"Integer number",\
		"Real number",\
		"Separation sign",\
		"Colon",\
		"Semicolon",\
		"Left parenthesis",\
		"Right parenthesis",\
		"Left bracket",\
		"Right bracket",\
		"End Of File"
		};
	return tokenKinds[i];
}