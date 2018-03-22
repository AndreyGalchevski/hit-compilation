#include "lexUtils.h"

void create_and_store_token(int type, arrayList* arr, char* string, int lineNum){
	
    token* newToken = (token*)malloc(sizeof(token));
    newToken->line = lineNum;
    newToken->type = type;
    newToken->lexeme = string;
    addToken(arr, newToken);
	free(newToken);
}

token* next_token(arrayList* arr){
   token** nextToken = (token**)malloc(sizeof(token*));

   if(fileIndex == arr->index){
        if (yylex()){
            *nextToken = &arr->tokens[fileIndex++];
        }
		else return NULL;
   }
   else{
        *nextToken = &arr->tokens[fileIndex++];
   }
   return *nextToken;
}

token* back_token(arrayList* arr){
	 
    token** prevToken = (token**)malloc(sizeof(token*));
    
    if(fileIndex >= 2){
        *prevToken = &arr->tokens[(--fileIndex) - 1];
    }

    return *prevToken;
}

void printError(char* lexeme, FILE* outputFile){    
    fprintf(outputFile, "\tThe character %s at line: %d does not begin any legal token in the language \n", lexeme, line_num);
}