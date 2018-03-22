#include "arrayList.h"

void newArray(arrayList *arr)
{
    arr->index = 0;
    arr->tokens = NULL;
    arr->size = 0;
}

void deleteToken(arrayList *arr, int indexToDelete)
{
    int i;
    //If index is out of bounds do nothing
    if (indexToDelete < 0 || indexToDelete >= arr->size)
        return;

    //Shift tokens to the left starting after the token we want to delete
    for (i = indexToDelete; i < arr->size - 1; i++)
    {
        arr->tokens[i] = arr->tokens[i + 1];
    }
    //Decrement the array size by one, because the last token is the same as the one before him
    arr->size--;
    arr->tokens = realloc(arr->tokens, sizeof(token) * arr->size);
}

void addToken(arrayList *arr, token *newToken)
{
    char *newLexeme = (*newToken).lexeme;
    arr->index++;
    //If there the array is full, enlarge it
    if (arr->index > arr->size)
    {
        arr->size = (++arr->size) * 2;
        arr->tokens = realloc(arr->tokens, sizeof(token) * (arr->size));
    }
    //Allocate space for the new lexeme
    arr->tokens[arr->index - 1].lexeme = (char *)malloc(strlen(newLexeme) + 1);
    //Assign new values to the current token
    strcpy(arr->tokens[arr->index - 1].lexeme, newLexeme);
    arr->tokens[arr->index - 1].type = (*newToken).type;
    arr->tokens[arr->index - 1].line = (*newToken).line;
}