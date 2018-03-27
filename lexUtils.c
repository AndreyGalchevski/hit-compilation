#include "lexUtils.h"

void create_and_store_token(int kind, arrayList* list, char* string, int lineNum){
	
    token* newToken = (token*)malloc(sizeof(token));
    newToken->line = lineNum;
    newToken->kind = kind;
    newToken->lexeme = string;
    addToken(list, newToken);
	free(newToken);
}

token* next_token(arrayList* list){
   token** nextToken = (token**)malloc(sizeof(token*));

   if(fileIndex == list->index){
        if (yylex()){
            *nextToken = &list->tokens[fileIndex++];
        }
		else return NULL;
   }
   else{
        *nextToken = &list->tokens[fileIndex++];
   }
   return *nextToken;
}

token* back_token(arrayList* list){
	 
    token** prevToken = (token**)malloc(sizeof(token*));
    
    if(fileIndex >= 2){
        *prevToken = &list->tokens[(--fileIndex) - 1];
    }

    return *prevToken;
}

void printToken(token* t, FILE* outputFile){
    if (t != NULL){
        fprintf(outputFile, "\tToken of kind '%s' was found at line: %d, lexeme: '%s' \n", toString(t->kind), t->line, t->lexeme);
    }
}

void printError(char* lexeme, FILE* outputFile){    
    fprintf(outputFile, "\tThe character '%s' at line: %d does not begin any legal token in the language \n", lexeme, line_num);
}