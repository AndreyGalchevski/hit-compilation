#include "parserUtils.h"

void match(int tokenKind, arrayList *array, FILE *file) {
    token *token = next_token(array);
    if (token->kind != tokenKind)
    {
        fprintf(file, "Expected token '%s' at line: %d, Actual token '%s', lexeme: '%s' \n",
            toString(tokenKind), token->line, toString(token->kind), token->lexeme);
    }
    else
    {
        if (token->kind == EOF_T)
        {
            back_token(array);
        }
    }
}

void errorRecovery(int *followArray, arrayList *array) {
    int size = followArray[0];
    token *token = back_token(array);
    int i;
    while (1)
    {
        token = next_token(array);
        for (i = 1; i <= size; i++)
        {
            if (token->kind == followArray[i] || token->kind == EOF_T)
            {
                back_token(array);
                return;
            }
        }
    }
}
/*
PROGRAM -> BLOCK
*/
void parseProgram(arrayList *array, FILE *file) {
    fprintf(file, "{PROGRAM -> BLOCK}\n");
    parseBlock(array, file);
}

/*
BLOCK -> block DEFINITIONS; begin COMMANDS; end
*/
void parseBlock(arrayList *array, FILE *file) {
    fprintf(file, "{BLOCK -> block DEFINITIONS; begin COMMANDS; end}\n");
    match(BLOCK_T, array, file);
    parseDefinitions(array, file);
    match(BEGIN_T, array, file);
    parseCommands(array, file);    
    match(END_T, array, file);    
}

/*
DEFINITIONS -> DEFINITION DEFINITIONS`
*/
void parseDefinitions(arrayList *array, FILE *file) {
    fprintf(file, "{DEFINITIONS -> DEFINITION DEFINITIONS`}\n");
    parseDefinition(array, file);
    parseDefinitions_(array, file);
}

/*
DEFINITIONS` -> ;DEFINITION DEFINITIONS` | epsilon
*/
void parseDefinitions_(arrayList *array, FILE *file) {
    token *token;
    fprintf(file, "{DEFINITIONS` -> ;DEFINITION DEFINITIONS` | epsilon}\n");
    token = next_token(array);

    switch(token->kind) {
        case SEMICOLON_T:
            parseDefinitions(array, file);
        default:
            back_token(array);
    }
}

/*
DEFINITIONS -> VAR_DEFINITION | TYPE_DEFINITION
*/
void parseDefinition(arrayList *array, FILE *file) {
    token *token;
    token = next_token(array);
    fprintf(file, "{DEFINITIONS -> VAR_DEFINITION | TYPE_DEFINITION}\n");
    switch(token->kind) {
        case ID_T:
            parseVarDefinition(array, file);
        case TYPE_T:
            parseTypeDefinition(array, file);            
    }   
}

/*
VAR_DEFINITION -> id: VAR_DEFINITION`
*/
void parseVarDefinition(arrayList *array, FILE *file) {
    fprintf(file, "{VAR_DEFINITION -> id: VAR_DEFINITION`}\n");
    match(ID_T, array, file);
    match(COLON_T, array, file);
    parseVarDefinition_(array, file);
}

/*
VAR_DEFINITION` -> BASIC_TYPE | type_name
*/
void parseVarDefinition_(arrayList *array, FILE *file) {   
    token *token;
    token = next_token(array);
    fprintf(file, "{VAR_DEFINITION` -> BASIC_TYPE | type_name}\n");

    switch(token->kind) {
        case INTEGER_T:
        case REAL_T:
            parseBasicType(array, file);
        case ID_T:
            match(ID_T, array, file);
    }
}

/*
type type_name is TYPE_INDICATOR
*/
void parseTypeDefinition_(arrayList *array, FILE *file) {
    fprintf(file, "{type type_name is TYPE_INDICATOR}\n");
    match(TYPE_T, array, file);
    match(ID_T, array, file);
    match(IS_T, array, file);
    parseTypeIndicator(array, file);
}

/*
TYPE_INDICATOR -> BASIC_TYPE | ARRAY_TYPE | POINTER_TYPE
*/
void parseTypeIndicator(arrayList *array, FILE *file) {   
    token *token;
    token = next_token(array);
    fprintf(file, "{TYPE_INDICATOR -> BASIC_TYPE | ARRAY_TYPE | POINTER_TYPE}\n");

    switch(token->kind) {
        case INTEGER_T:
        case REAL_T:
            parseBasicType(array, file);
        case ARRAY_T:
            parseArrayType(array, file);
        case POINTER_T:
            parsePointerType(array, file);
        default:
            back_token(array);
    }
}

/*
BASIC_TYPE -> integer | real
*/
void parseBasicType(arrayList *array, FILE *file) {   
    token *token;
    token = next_token(array);
    fprintf(file, "{BASIC_TYPE -> integer | real}\n");

    switch(token->kind) {
        case INTEGER_T:
        case REAL_T:
            break;
        default:
            back_token(array);
    }
}

/*
ARRAY_TYPE -> array[SIZE] of BASIC_TYPE
*/
void parseArrayType(arrayList *array, FILE *file) {   
    fprintf(file, "{ARRAY_TYPE -> array[SIZE] of BASIC_TYPE}\n");
    match(ARRAY_T, array, file);
    match(LEFT_BRACKET_T, array, file);
    parseSize(array, file);
    match(RIGHT_BRACKET_T, array, file);
    match(OF_T, array, file);
    parseBasicType(array, file);
}

/*
POINTER_TYPE -> ^POINTER_TYPE`
*/
void parsePointerType(arrayList *array, FILE *file){
    fprintf(file, "{POINTER_TYPE -> ^POINTER_TYPE`}\n");
    match(POINTER_T, array, file);
    parsePointerType_(array, file);
}

/*
POINTER_TYPE` -> BASIC_TYPE | type_name
*/
void parsePointerType_(arrayList *array, FILE *file) {   
    token *token;
    token = next_token(array);
    fprintf(file, "{POINTER_TYPE` -> BASIC_TYPE | type_name}\n");

    switch(token->kind) {
        case INTEGER_T:
        case REAL_T:
            parseBasicType(array, file);
        case ID_T:
            break;
        default:
            back_token(array);
    }
}

/*
SIZE -> int_num
*/
void parseSize(arrayList *array, FILE *file) {   
    fprintf(file, "{SIZE -> int_num}\n");

    match(INT_NUM_T, array, file);
}

/*
COMMANDS -> COMMANDS`
*/
void parseCommands(arrayList *array, FILE *file) {   
    fprintf(file, "{COMMANDS -> COMMANDS`}\n");

    parseCommands_(array, file);
}

/*
COMMANDS` -> ;COMMAND COMMANDS` | epsilon
*/
void parseCommands_(arrayList *array, FILE *file) {
    token *token;
    token = next_token(array);
    fprintf(file, "{COMMANDS` -> ;COMMAND COMMANDS` | epsilon}\n");

    switch(token->kind) {
        case SEMICOLON_T:
            parseCommand(array, file);
            parseCommands_(array, file);
        default:
            back_token(array);
    }
}

/*
COMMAND -> RECEIVER = EXPRESSION
COMMAND -> when (EXPRESSION rel_op EXPRESSION) do
COMMAND -> for (id = EXPRESSION; id rel_op EXPRESSION; id++) COMMANDS; end_for
COMMAND -> id = malloc(size_of(type_name))
COMMAND -> free(id)
COMMAND -> BLOCK
*/
void parseCommand(arrayList *array, FILE *file) {
    token *token;
    token = next_token(array);

    switch(token->kind) {
        // case RECEIVER:
        //     fprintf(file, "{COMMAND -> RECEIVER = EXPRESSION}\n");
        //     //todo
        case WHEN_T:
            fprintf(file, "{COMMAND -> when (EXPRESSION rel_op EXPRESSION) do}\n");
            match(LEFT_PARENTHESIS_T, array, file);
            parseExpression(array, file);
            match(REL_OP_T, array, file);
            parseExpression(array, file);
            match(RIGHT_PARENTHESIS_T, array, file);            
            match(DO_T, array, file);
            parseCommands(array, file);
            match(SEMICOLON_T, array, file);
            match(DEFAULT_T, array, file);
            parseCommands(array, file);
            match(SEMICOLON_T, array, file);            
            match(END_WHEN_T, array, file);
            break;
        case FOR_T:
            fprintf(file, "{COMMAND -> for (id = EXPRESSION; id rel_op EXPRESSION; id++) COMMANDS; end_for}\n");
            match(LEFT_PARENTHESIS_T, array, file);
            match(ID_T, array, file);                                    
            match(ASSIGNMENT_T, array, file);
            parseExpression(array, file);
            match(SEMICOLON_T, array, file);
            match(ID_T, array, file);                    
            match(REL_OP_T, array, file);
            parseExpression(array, file);
            match(SEMICOLON_T, array, file);
            match(ID_T, array, file);                    
            match(INCREMENT_T, array, file);
            match(RIGHT_PARENTHESIS_T, array, file);
            parseCommands(array, file);
            match(SEMICOLON_T, array, file);            
            match(END_FOR_T, array, file);
            break;
        case ID_T:
            fprintf(file, "{COMMAND -> id = malloc(size_of(type_name))}\n");
            match(ASSIGNMENT_T, array, file);
            match(MALLOC_T, array, file);
            match(LEFT_PARENTHESIS_T, array, file);
            match(SIZE_OF_T, array, file);         
            match(LEFT_PARENTHESIS_T, array, file);
            match(ID_T, array, file);
            match(RIGHT_PARENTHESIS_T, array, file);
            match(RIGHT_PARENTHESIS_T, array, file);                                                               
            break;
        case FREE_T:
            fprintf(file, "{COMMAND -> free(id)}\n");
            match(LEFT_PARENTHESIS_T, array, file);
            match(ID_T, array, file);
            match(RIGHT_PARENTHESIS_T, array, file);
            break;
        case BLOCK_T:
            fprintf(file, "{COMMAND -> BLOCK}\n");        
            parseBlock(array, file);
            break;
        default:
            back_token(array);
    }
}

/*
RECEIVER -> id RECEIVER`
*/
void parseReceiver(arrayList *array, FILE *file) {
    fprintf(file, "{RECEIVER -> id RECEIVER`}\n");
    match(ID_T, array, file);
    parseReceiver_(array, file);
}

/*
RECEIVER` -> [EXPRESSION]
RECEIVER` -> ^
RECEIVER` -> epsilon
*/
void parseReceiver_(arrayList *array, FILE *file) {
    token *token;
    token = next_token(array);

    switch(token->kind) {
        case LEFT_BRACKET_T:
            fprintf(file, "{RECEIVER` -> [EXPRESSION]}\n");
            parseExpression(array, file);
            match(RIGHT_BRACKET_T, array, file);
            break;
        case POINTER_T:
            fprintf(file, "{RECEIVER` -> ^}\n");        
            break;
        default:
            fprintf(file, "{RECEIVER` -> epsilon}");
            back_token(array);     
    }
}

/*
EXPRESSION -> int_num
EXPRESSION -> real_num
EXPRESSION -> &id
EXPRESSION -> size_of(type_name)
EXPRESSION -> id EXPRESSION`
*/
void parseExpression(arrayList *array, FILE *file) {
    token *token;
    token = next_token(array);

    switch(token->kind) {
        case INT_NUM_T:
            fprintf(file, "{EXPRESSION -> int_num}\n");
            break;
        case REAL_NUM_T:
            fprintf(file, "{EXPRESSION -> real_num}\n");        
            break;
        case ADDRESS_T:
            fprintf(file, "{EXPRESSION -> &id}\n");
            match(ID_T, array, file);        
            break;
        case SIZE_OF_T:
            fprintf(file, "{EXPRESSION -> size_of(type_name)}\n");
            match(LEFT_PARENTHESIS_T, array, file);
            match(ID_T, array, file);
            match(RIGHT_PARENTHESIS_T, array, file);            
        case ID_T:
            fprintf(file, "{EXPRESSION -> id EXPRESSION`}\n");            
            parseExpression_(array, file);
            break;
        default:
            back_token(array);     
    }
}

/*
EXPRESSION` -> [EXPRESSION]
EXPRESSION` -> ^
EXPRESSION` -> ar_op EXPRESSION
EXPRESSION -> epsilon
*/
void parseExpression_(arrayList *array, FILE *file) {
    token *token;
    token = next_token(array);

        switch(token->kind) {
            case LEFT_BRACKET_T:
                fprintf(file, "{EXPRESSION` -> [EXPRESSION]}\n");
                parseExpression(array, file);
                match(RIGHT_BRACKET_T, array, file);      
                break;
            case POINTER_T:
                fprintf(file, "{EXPRESSION` -> ^}\n");
                break;                          
            case AR_OP_T:
                fprintf(file, "{EXPRESSION` -> ar_op EXPRESSION}\n");
                parseExpression(array, file);
                break;
            default:
                fprintf(file, "{EXPRESSION -> epsilon}\n");        
                back_token(array);   
        }
}