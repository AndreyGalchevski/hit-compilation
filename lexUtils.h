#ifndef LEX_UTILS
#define LEX_UTILS

#include "arrayList.h"

int line_num = 1;
int fileIndex = 0;

void create_and_store_token(int, arrayList*, char*, int);
token* next_token(arrayList*);
token* back_token(arrayList*);
void printToken(token*, FILE*);
void printError(char*, FILE*);

#endif