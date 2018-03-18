#include "lexUtils.h"

void create_and_store_token(int type, arrayList* arr, char* string, int lineNum){
	
    token* tok = (token*)malloc(sizeof(token));
    tok->line = lineNum;
    tok->type = type;
    tok->lexeme = string;
    addToken(arr, tok);
	free(tok);
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
    fprintf(outputFile, "\tERROR IN LINE %d: \"%s\"\n", line_num, lexeme);
}