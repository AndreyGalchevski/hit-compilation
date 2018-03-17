#include "arrayList.h"

void newArray(arrayList *arr)
{
    arr->index = 0;
    arr->tokens = NULL;
    arr->size = 0;
}

void deleteToken(arrayList *arr, int index)
{
    int i;
    //Token not in our array top kek
    if (index < 0 || index >= arr->size)
        return;

    //We simply move evertything back (not really deleting)
    for (i = index; i < arr->size - 1; i++)
    {
        arr->tokens[i] = arr->tokens[i + 1];
    }
    //Since the last token is "duplicated" we don't really need arr[size]
    arr->size--;
    arr->tokens = realloc(arr->tokens, sizeof(token) * arr->size);
}

void addToken(arrayList *arr, token *val)
{
    char *lexem = (*val).lexeme;
    arr->index++;
    //If our array got overflown
    if (arr->index > arr->size)
    {
        arr->size = (++arr->size) * 2;
        arr->tokens = realloc(arr->tokens, sizeof(token) * (arr->size));
    }

    arr->tokens[arr->index - 1].lexeme = (char *)malloc(strlen(lexem) + 1);
    //Copy values to current token in our array
    strcpy(arr->tokens[arr->index - 1].lexeme, lexem);
    arr->tokens[arr->index - 1].type = (*val).type;
    arr->tokens[arr->index - 1].line = (*val).line;
}