#include "parserUtils.h"
#include "symbolTable.c"

typedef struct Var {
	int datatype;
	char* lexeme;
}Var;

Var BASIC_TYPE;
Var ARRAY_TYPE;
Var POINTER_TYPE;
Var EXPRESSION;

TableEntry* id_entry;

void match(int tokenKind, arrayList *array, FILE *file) {
    token *token = next_token(array);
    if (token->kind != tokenKind) {
        fprintf(file, "Expected token '%s' at line: %d, Actual token '%s', lexeme: '%s' \n",
        toString(tokenKind), token->line, toString(token->kind), token->lexeme);
    }
    else {
        if (token->kind == EOF_T) {
            back_token(array);
        }
    }
}

/*
PROGRAM -> BLOCK
*/
void parseProgram(arrayList *array, FILE *syntacticOut, FILE *semanticOut) {
    fprintf(syntacticOut, "{PROGRAM -> BLOCK}\n");
    parseBlock(array, syntacticOut, semanticOut);
}

/*
BLOCK -> block DEFINITIONS; begin COMMANDS; end
*/
void parseBlock(arrayList *array, FILE *syntacticOut, FILE *semanticOut) {
    fprintf(syntacticOut, "{BLOCK -> block DEFINITIONS; begin COMMANDS; end}\n");
    match(BLOCK_T, array, syntacticOut);
	make_table();
    parseDefinitions(array, syntacticOut, semanticOut);
    match(BEGIN_T, array, syntacticOut);
    parseCommands(array, syntacticOut, semanticOut);
    match(END_T, array, syntacticOut);
	pop_table();
}

/*
DEFINITIONS -> DEFINITION DEFINITIONS`
*/
void parseDefinitions(arrayList *array, FILE *syntacticOut, FILE *semanticOut) {
    fprintf(syntacticOut, "{DEFINITIONS -> DEFINITION DEFINITIONS`}\n");
    parseDefinition(array, syntacticOut, semanticOut);
    parseDefinitions_(array, syntacticOut, semanticOut);
}

/*
DEFINITIONS` -> ;DEFINITION DEFINITIONS` | epsilon
*/
void parseDefinitions_(arrayList *array, FILE *syntacticOut, FILE *semanticOut) {
    token *token;
    token = next_token(array);

    switch(token->kind) {
        case SEMICOLON_T:
            fprintf(syntacticOut, "{DEFINITIONS` -> ;DEFINITION DEFINITIONS`}\n");            
            parseDefinition(array, syntacticOut, semanticOut);        
            parseDefinitions_(array, syntacticOut, semanticOut);
            break;
        default:
            fprintf(syntacticOut, "{DEFINITIONS` -> epsilon}\n");                        
            back_token(array);
            break;
    }
}

/*
DEFINITION -> VAR_DEFINITION | TYPE_DEFINITION
*/
void parseDefinition(arrayList *array, FILE *syntacticOut, FILE *semanticOut) {
    token *token;
    token = next_token(array);
    switch(token->kind) {
        case ID_T:
            fprintf(syntacticOut, "{DEFINITION -> VAR_DEFINITION}\n");
			id_entry = insert(token->lexeme);
			if(id_entry == NULL) { 
				error(DUPLICATED_DECLARATION_ERROR, token->line, token->lexeme, semanticOut); 
			}  
            parseVarDefinition(array, syntacticOut, semanticOut);
            break;
        case TYPE_T:
            fprintf(syntacticOut, "{DEFINITION -> TYPE_DEFINITION}\n");            
            parseTypeDefinition(array, syntacticOut, semanticOut);
            break;
        default:
            back_token(array);
            break;
    }   
}

/*
VAR_DEFINITION -> id: VAR_DEFINITION`
*/
void parseVarDefinition(arrayList *array, FILE *syntacticOut, FILE *semanticOut) {
    fprintf(syntacticOut, "{VAR_DEFINITION -> id: VAR_DEFINITION`}\n");
	match(COLON_T, array, syntacticOut);
    parseVarDefinition_(array, syntacticOut, semanticOut);
}

/*
VAR_DEFINITION` -> BASIC_TYPE | type_name
*/
void parseVarDefinition_(arrayList *array, FILE *syntacticOut, FILE *semanticOut) {
	int data_type;
    token *token;
    token = next_token(array);

    switch(token->kind) {
        case INTEGER_T:
            fprintf(syntacticOut, "{VAR_DEFINITION` -> BASIC_TYPE}\n");                        
            fprintf(syntacticOut, "{BASIC_TYPE -> integer}\n");
            break;
        case REAL_T:
            fprintf(syntacticOut, "{VAR_DEFINITION` -> BASIC_TYPE}\n");                        
            fprintf(syntacticOut, "{BASIC_TYPE -> real}\n");
            break;
        case ID_T:
            fprintf(syntacticOut, "{VAR_DEFINITION` -> type_name}\n");
			id_entry = insert(token->lexeme);
			if(id_entry == NULL) { 
				error(DUPLICATED_DECLARATION_ERROR, token->line, token->lexeme, semanticOut); 
			}            
            break;
        default:
            back_token(array);
            break;
    }
}

/*
type type_name is TYPE_INDICATOR
*/
void parseTypeDefinition(arrayList *array, FILE *syntacticOut, FILE *semanticOut) {
    fprintf(syntacticOut, "{TYPE_DEFINITION -> type type_name is TYPE_INDICATOR}\n");
    match(ID_T, array, syntacticOut);
    match(IS_T, array, syntacticOut);
    parseTypeIndicator(array, syntacticOut, semanticOut);
}

/*
TYPE_INDICATOR -> BASIC_TYPE | ARRAY_TYPE | POINTER_TYPE
*/
void parseTypeIndicator(arrayList *array, FILE *syntacticOut, FILE *semanticOut) {   
    token *token;
    token = next_token(array);

    switch(token->kind) {
        case INTEGER_T:
            fprintf(syntacticOut, "{TYPE_INDICATOR -> BASIC_TYPE}\n");              
            fprintf(syntacticOut, "{BASIC_TYPE -> integer}\n");
            break;            
        case REAL_T:
            fprintf(syntacticOut, "{TYPE_INDICATOR -> BASIC_TYPE}\n");                          
            fprintf(syntacticOut, "{BASIC_TYPE -> real}\n");
            break;
        case ARRAY_T:
            fprintf(syntacticOut, "{TYPE_INDICATOR -> ARRAY_TYPE}\n");                          
            parseArrayType(array, syntacticOut, semanticOut);
            break;
        case POINTER_T:
            fprintf(syntacticOut, "{TYPE_INDICATOR -> POINTER_TYPE}\n");            
            parsePointerType(array, syntacticOut, semanticOut);
            break;
        default:
            back_token(array);
            break;
    }
}

/*
BASIC_TYPE -> integer | real
*/
void parseBasicType(arrayList *array, FILE *syntacticOut, FILE *semanticOut) {   
    token *token;
    token = next_token(array);

    switch(token->kind) {
        case INTEGER_T:
            fprintf(syntacticOut, "{BASIC_TYPE -> integer}\n");
			BASIC_TYPE.datatype = INTEGER_T;
            break;
        case REAL_T:
            fprintf(syntacticOut, "{BASIC_TYPE -> real}\n");
			BASIC_TYPE.datatype = REAL_T;         
            break;
        default:
            back_token(array);
            break;
    }
}

/*
ARRAY_TYPE -> array[SIZE] of BASIC_TYPE
*/
void parseArrayType(arrayList *array, FILE *syntacticOut, FILE *semanticOut) {   
    fprintf(syntacticOut, "{ARRAY_TYPE -> array[SIZE] of BASIC_TYPE}\n");
	ARRAY_TYPE.datatype = ARRAY_T;
    match(LEFT_BRACKET_T, array, syntacticOut);
    parseSize(array, syntacticOut, semanticOut);
    match(RIGHT_BRACKET_T, array, syntacticOut);
    match(OF_T, array, syntacticOut);
    parseBasicType(array, syntacticOut, semanticOut);
}

/*
POINTER_TYPE -> ^POINTER_TYPE`
*/
void parsePointerType(arrayList *array, FILE *syntacticOut, FILE *semanticOut){
    fprintf(syntacticOut, "{POINTER_TYPE -> ^POINTER_TYPE`}\n");

	POINTER_TYPE.datatype = POINTER_T;
    parsePointerType_(array, syntacticOut, semanticOut);
}

/*
POINTER_TYPE` -> BASIC_TYPE | type_name
*/
void parsePointerType_(arrayList *array, FILE *syntacticOut, FILE *semanticOut) {   
    token *token;
    token = next_token(array);

    switch(token->kind) {
        case INTEGER_T:
            fprintf(syntacticOut, "{POINTER_TYPE` -> BASIC_TYPE}\n");            
            fprintf(syntacticOut, "{BASIC_TYPE -> integer}\n");
            break;            
        case REAL_T:
            fprintf(syntacticOut, "{POINTER_TYPE` -> BASIC_TYPE}\n");                    
            fprintf(syntacticOut, "{BASIC_TYPE -> real}\n");
            break;
        case ID_T:
            fprintf(syntacticOut, "{POINTER_TYPE` -> type_name}\n");
            break;
        default:
            back_token(array);
            break;
    }
}

/*
SIZE -> int_num
*/
void parseSize(arrayList *array, FILE *syntacticOut, FILE *semanticOut) {   
    fprintf(syntacticOut, "{SIZE -> int_num}\n");

    match(INT_NUM_T, array, syntacticOut);
}

/*
COMMANDS -> COMMAND COMMANDS`
*/
void parseCommands(arrayList *array, FILE *syntacticOut, FILE *semanticOut) {   
    fprintf(syntacticOut, "{COMMANDS -> COMMAND COMMANDS`}\n");
    
    parseCommand(array, syntacticOut, semanticOut);
    parseCommands_(array, syntacticOut, semanticOut);
}

/*
COMMANDS` -> ;COMMANDS | epsilon
*/
void parseCommands_(arrayList *array, FILE *syntacticOut, FILE *semanticOut) {
    token *token;
    token = next_token(array);

    switch(token->kind) {
        case SEMICOLON_T:
            fprintf(syntacticOut, "{COMMANDS` -> ;COMMANDS}\n");            
            parseCommands(array, syntacticOut, semanticOut);
            break;
        default:
            fprintf(syntacticOut, "{COMMANDS` -> epsilon}\n");
            back_token(array);
            break;
    }
}

/*
COMMAND -> RECEIVER = EXPRESSION
COMMAND -> when (EXPRESSION rel_op EXPRESSION) do COMMANDS; default COMMANDS; end_when
COMMAND -> for (id = EXPRESSION; id rel_op EXPRESSION; id++) COMMANDS; end_for
COMMAND -> id = malloc(size_of(type_name))
COMMAND -> free(id)
COMMAND -> BLOCK
*/
void parseCommand(arrayList *array, FILE *syntacticOut, FILE *semanticOut) {
    token *token;
    token = next_token(array);

    switch(token->kind) {
        case WHEN_T:
            fprintf(syntacticOut, "{COMMAND -> when (EXPRESSION rel_op EXPRESSION) do COMMANDS; default COMMANDS; end_when}\n");
            match(LEFT_PARENTHESIS_T, array, syntacticOut);
            parseExpression(array, syntacticOut, semanticOut);
            match(REL_OP_T, array, syntacticOut);
            parseExpression(array, syntacticOut, semanticOut);
            match(RIGHT_PARENTHESIS_T, array, syntacticOut);            
            match(DO_T, array, syntacticOut);
            parseCommands(array, syntacticOut, semanticOut);
            match(DEFAULT_T, array, syntacticOut);
            parseCommands(array, syntacticOut, semanticOut);
            match(END_WHEN_T, array, syntacticOut);
            break;
        case FOR_T:
            fprintf(syntacticOut, "{COMMAND -> for (id = EXPRESSION; id rel_op EXPRESSION; id++) COMMANDS; end_for}\n");
            match(LEFT_PARENTHESIS_T, array, syntacticOut);
            match(ID_T, array, syntacticOut);
			id_entry = insert(token->lexeme);
			if(id_entry == NULL){ 
				error(DUPLICATED_DECLARATION_ERROR, token->line, token->lexeme, semanticOut); 
			}  
            match(ASSIGNMENT_T, array, syntacticOut);
            parseExpression(array, syntacticOut, semanticOut);
            match(SEMICOLON_T, array, syntacticOut);
            match(ID_T, array, syntacticOut);
			id_entry = find(token->lexeme);						                   
			if (id_entry == NULL) { 
				error(VARIABLE_NOT_DECLARED_ERROR, token->line, token->lexeme, semanticOut); 
			}                          
            match(REL_OP_T, array, syntacticOut);
            parseExpression(array, syntacticOut, semanticOut);
            match(SEMICOLON_T, array, syntacticOut);
            match(ID_T, array, syntacticOut);
			id_entry = find(token->lexeme);						                   
			if (id_entry == NULL) { 
				error(VARIABLE_NOT_DECLARED_ERROR, token->line, token->lexeme, semanticOut); 
			}                   
            match(INCREMENT_T, array, syntacticOut);
            match(RIGHT_PARENTHESIS_T, array, syntacticOut);
            parseCommands(array, syntacticOut, semanticOut);
            match(END_FOR_T, array, syntacticOut);
            break;
        case ID_T:
			id_entry = find(token->lexeme);
			if (id_entry == NULL) { 
				error(VARIABLE_NOT_DECLARED_ERROR, token->line, token->lexeme, semanticOut); 
			}
            token = next_token(array); // now token is either '=' or epsilon | '[' | ^
            if (token->kind == ASSIGNMENT_T) {
                token = next_token(array);
                if (token->kind == MALLOC_T) {
                    fprintf(syntacticOut, "{COMMAND -> id = malloc(size_of(type_name))}\n");
                    match(LEFT_PARENTHESIS_T, array, syntacticOut);
                    match(SIZE_OF_T, array, syntacticOut);         
                    match(LEFT_PARENTHESIS_T, array, syntacticOut);
                    match(ID_T, array, syntacticOut);
					id_entry = find(token->lexeme);						                   
					if (id_entry == NULL) { 
						error(VARIABLE_NOT_DECLARED_ERROR, token->line, token->lexeme, semanticOut); 
					}  
                    match(RIGHT_PARENTHESIS_T, array, syntacticOut);
                    match(RIGHT_PARENTHESIS_T, array, syntacticOut);                                                               
                }
                else {
                    back_token(array);
                    fprintf(syntacticOut, "{COMMAND -> RECEIVER = EXPRESSION}\n");
                    fprintf(syntacticOut, "{RECEIVER` -> epsilon}\n");                    
                    parseExpression(array, syntacticOut, semanticOut);
                }
            }
            else {
                back_token(array);                
                parseReceiver_(array, syntacticOut, semanticOut);
                match(ASSIGNMENT_T, array, syntacticOut);
                parseExpression(array, syntacticOut, semanticOut);
            }
            break;
        case FREE_T:
            fprintf(syntacticOut, "{COMMAND -> free(id)}\n");
            match(LEFT_PARENTHESIS_T, array, syntacticOut);
            match(ID_T, array, syntacticOut);
			id_entry = find(token->lexeme);						                   
			if (id_entry == NULL) { 
				error(VARIABLE_NOT_DECLARED_ERROR, token->line, token->lexeme, semanticOut); 
			}  
            match(RIGHT_PARENTHESIS_T, array, syntacticOut);
            break;
        case BLOCK_T:
            fprintf(syntacticOut, "{COMMAND -> BLOCK}\n");
            back_token(array);        
            parseBlock(array, syntacticOut, semanticOut);
            break;
        default:
            back_token(array);
            break;
    }
}

void parseReceiver_(arrayList *array, FILE *syntacticOut, FILE *semanticOut) {
    token *token;
    token = next_token(array);

    switch(token->kind) {
        case LEFT_BRACKET_T:
            fprintf(syntacticOut, "{RECEIVER` -> [EXPRESSION]}\n");
            parseExpression(array, syntacticOut, semanticOut);
            match(RIGHT_BRACKET_T, array, syntacticOut);
            break;
        case POINTER_T:
            fprintf(syntacticOut, "{RECEIVER` -> ^}\n");        
            break;
        default:
            fprintf(syntacticOut, "{RECEIVER` -> epsilon}\n");
            back_token(array);
            break;     
    }
}

/*
EXPRESSION -> int_num
EXPRESSION -> real_num
EXPRESSION -> &id
EXPRESSION -> size_of(type_name)
EXPRESSION -> id EXPRESSION`
*/
void parseExpression(arrayList *array, FILE *syntacticOut, FILE *semanticOut) {
    token *token;
    token = next_token(array);

    switch(token->kind) {
        case INT_NUM_T:
            fprintf(syntacticOut, "{EXPRESSION -> int_num}\n");
            break;
        case REAL_NUM_T:
            fprintf(syntacticOut, "{EXPRESSION -> real_num}\n");        
            break;
        case ADDRESS_T:
            fprintf(syntacticOut, "{EXPRESSION -> &id}\n");
            match(ID_T, array, syntacticOut);    
			id_entry = find(token->lexeme);						                   
			if (id_entry == NULL) { 
				error(VARIABLE_NOT_DECLARED_ERROR, token->line, token->lexeme, semanticOut); 
			}      
            break;
        case SIZE_OF_T:
            fprintf(syntacticOut, "{EXPRESSION -> size_of(type_name)}\n");
            match(LEFT_PARENTHESIS_T, array, syntacticOut);
            match(ID_T, array, syntacticOut);
			id_entry = find(token->lexeme);						                   
			if (id_entry == NULL) { 
				error(VARIABLE_NOT_DECLARED_ERROR, token->line, token->lexeme, semanticOut); 
			}  
            match(RIGHT_PARENTHESIS_T, array, syntacticOut);    
            break;        
        case ID_T:
			id_entry = find(token->lexeme);
			if (id_entry == NULL) { 
				error(VARIABLE_NOT_DECLARED_ERROR, token->line, token->lexeme, semanticOut); 
			}
            fprintf(syntacticOut, "{EXPRESSION -> id EXPRESSION`}\n");
			if(id_entry){
				if ( is_integer(id_entry) && EXPRESSION.datatype == REAL_NUM_T ){ 
					error(TYPE_CONSISTENCY_ERROR, token->line, token->lexeme, semanticOut);
				}
			}          
            parseExpression_(array, syntacticOut, semanticOut);
            break;
        default:
            back_token(array);
            break;     
    }
}

/*
EXPRESSION` -> [EXPRESSION]
EXPRESSION` -> ^
EXPRESSION` -> ar_op EXPRESSION
EXPRESSION -> epsilon
*/
void parseExpression_(arrayList *array, FILE *syntacticOut, FILE *semanticOut) {
    token *token;
    token = next_token(array);

	switch(token->kind) {
		case LEFT_BRACKET_T:
			fprintf(syntacticOut, "{EXPRESSION` -> [EXPRESSION]}\n");
			parseExpression(array, syntacticOut, semanticOut);
			match(RIGHT_BRACKET_T, array, syntacticOut);      
			break;
		case POINTER_T:
			fprintf(syntacticOut, "{EXPRESSION` -> ^}\n");
			break;                          
		case AR_OP_T:
			fprintf(syntacticOut, "{EXPRESSION` -> ar_op EXPRESSION}\n");
			parseExpression(array, syntacticOut, semanticOut);
			break;
		default:
			fprintf(syntacticOut, "{EXPRESSION -> epsilon}\n");        
			back_token(array); 
			break;  
	}
}


void error(int errorType, int line, char* lexeme, FILE *file){
	fprintf(file,"---Semantic Error: detected in line %d, lexeme '%s'---\n", line, lexeme);
	switch(errorType){
		case DUPLICATED_DECLARATION_ERROR:
			fprintf(file,"---duplicated declaration of the same name within same scope is forbidden)---\n");
			break;
		case VARIABLE_NOT_DECLARED_ERROR:
			fprintf(file,"---variable is used without being declared---\n");
			break;
		case TYPE_CONSISTENCY_ERROR:
			fprintf(file,"---left side integer and right side is real is forbidden---\n");
			break;
		default:
			break;
	}
}

int isRealNumber(char* number){
	char* c = strchr(number, '.');
	if (c)
		return 1;
	else
		return 0;
}