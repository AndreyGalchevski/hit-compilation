#include "arrayList.h"

void createNewArray(arrayList *list)
{
    list->index = 0;
    list->tokens = NULL;
    list->size = 0;
}

void deleteToken(arrayList *list, int indexToDelete)
{
    int i;
    //If index is out of bounds do nothing
    if (indexToDelete < 0 || indexToDelete >= list->size)
        return;

    //Shift tokens to the left starting after the token we want to delete
    for (i = indexToDelete; i < list->size - 1; i++)
    {
        list->tokens[i] = list->tokens[i + 1];
    }
    //Decrement the array size by one, because the last token is the same as the one before him
    list->size--;
    list->tokens = realloc(list->tokens, sizeof(token) * list->size);
}

void addToken(arrayList *list, token *newToken)
{
    char *newLexeme = (*newToken).lexeme;
    list->index++;
    //If there the array is full, enlarge it
    if (list->index > list->size)
    {
        list->size = (++list->size) * 2;
        list->tokens = realloc(list->tokens, sizeof(token) * (list->size));
    }
    //Allocate space for the new lexeme
    list->tokens[list->index - 1].lexeme = (char *)malloc(strlen(newLexeme) + 1);
    //Assign new values to the current token
    strcpy(list->tokens[list->index - 1].lexeme, newLexeme);
    list->tokens[list->index - 1].kind = (*newToken).kind;
    list->tokens[list->index - 1].line = (*newToken).line;
}