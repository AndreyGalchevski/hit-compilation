#ifndef PARSER_UTILS
#define PARSER_UTILS

#include "lexUtils.h"
#include "arrayList.h"
#include "token.h"

void match(int, arrayList *, FILE*);

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
