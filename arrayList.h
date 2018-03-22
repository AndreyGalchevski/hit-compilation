#ifndef ARRAY_LIST
#define ARRAY_LIST

#include <stdlib.h>
#include "token.h"

typedef struct {
    int index;
	token* tokens;
    int size;
} arrayList;

void createNewArray(arrayList*);
void deleteToken(arrayList*, int);
void addToken(arrayList*, token*);

#endif