#ifndef PARSER_UTILS
#define PARSER_UTILS

#include "lexUtils.h"
#include "arrayList.h"
#include "token.h"

int definitionsFollow[3] = {2, SEMICOLON_T, BEGIN_T};
int definitionFollow[3] = {2, SEMICOLON_T, BEGIN_T};
int commandsFollow[5] = {4, SEMICOLON_T, END_T, END_WHEN_T, END_FOR_T};
int commandFollow[5] = {4, SEMICOLON_T, END_T, END_WHEN_T, END_FOR_T};
int expressionFollow[5] = {4,  REL_OP_T, SEMICOLON_T, RIGHT_PARENTHESIS_T, RIGHT_BRACKET_T}; 
int receiverFollow[3] = {1, LEFT_BRACKET_T, POINTER_T}; 
int sizeFollow[3] = {2, RIGHT_BRACKET_T, OF_T};

void match(int, arrayList *, FILE*);
void errorRecovery(int *, arrayList *);

void parseProgram(arrayList *, FILE*);

void parseBlock(arrayList *, FILE*);

void parseDefinitions(arrayList *, FILE*);
void parseDefinitions_(arrayList *, FILE*);
void parseDefinition(arrayList *, FILE*);

void parseVarDefinition(arrayList *, FILE*);
void parseVarDefinition_(arrayList *, FILE*);
void parseTypeDefinition(arrayList *, FILE*);
void parseTypeIndicator(arrayList *, FILE*);

void parseBasicType(arrayList *, FILE*);
void parseArrayType(arrayList *, FILE*);
void parsePointerType(arrayList *, FILE*);
void parsePointerType_(arrayList *, FILE*);

void parseSize(arrayList *, FILE*);

void parseCommands(arrayList *, FILE*);
void parseCommands_(arrayList *, FILE*);
void parseCommand(arrayList *, FILE*);

void parseReceiver_(arrayList *, FILE*);

void parseExpression(arrayList *, FILE*);
void parseExpression_(arrayList *, FILE*);

#endif
