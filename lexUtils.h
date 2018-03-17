#ifndef LEX_UTILS
#define LEX_UTILS

#include "arrayList.h"

int line_num = 1;
int fileIndex = 0;

token* next_token(arrayList*);
void create_and_store_token(int, arrayList*, char*, int);
void printError(char*, FILE*);
void errorHandle(char*);

#endif