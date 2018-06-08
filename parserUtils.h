#ifndef PARSER_UTILS
#define PARSER_UTILS

#include "lexUtils.h"
#include "arrayList.h"
#include "token.h"

typedef enum eErrorType{
	DUPLICATED_DECLARATION_ERROR,
	VARIABLE_NOT_DECLARED_ERROR,
	TYPE_CONSISTENCY_ERROR
}eErrorType;

token* match(int, arrayList *, FILE*);

void parseProgram(arrayList *, FILE*, FILE*);

void parseBlock(arrayList *, FILE*, FILE*);

void parseDefinitions(arrayList *, FILE*, FILE*);
void parseDefinitions_(arrayList *, FILE*, FILE*);
void parseDefinition(arrayList *, FILE*, FILE*);

void parseVarDefinition(arrayList *, FILE*, FILE*);
void parseVarDefinition_(arrayList *, FILE*, FILE*);
void parseTypeDefinition(arrayList *, FILE*, FILE*);
void parseTypeIndicator(arrayList *, FILE*, FILE*);

void parseBasicType(arrayList *, FILE*, FILE*);
void parseArrayType(arrayList *, FILE*, FILE*);
void parsePointerType(arrayList *, FILE*, FILE*);
void parsePointerType_(arrayList *, FILE*, FILE*);

void parseSize(arrayList *, FILE*, FILE*);

void parseCommands(arrayList *, FILE*, FILE*);
void parseCommands_(arrayList *, FILE*, FILE*);
void parseCommand(arrayList *, FILE*, FILE*);

void parseReceiver_(arrayList *, FILE*, FILE*);

void parseExpression(arrayList *, FILE*, FILE*);
void parseExpression_(arrayList *, FILE*, FILE*);

void error(int errorType, int line, char* lexeme, FILE* file);

#endif
