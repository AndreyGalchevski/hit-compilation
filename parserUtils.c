#include "parserUtils.h"
#include "symbolTable.c"

typedef struct Var {
	int datatype;
	char* lexeme;
}Var;

Var BASIC_TYPE;
Var DEFINITION;
Var DEFINITION_B;
Var EXPRESSION;
Var EXPRESSION_B;
Var ID_LIST;

TableEntry* id_entry;

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
		make_table();
    parseDefinitions(array, file);
    match(BEGIN_T, array, file);
    parseCommands(array, file);
    match(END_T, array, file);
		pop_table();
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
    token = next_token(array);

    switch(token->kind) {
        case SEMICOLON_T:
            fprintf(file, "{DEFINITIONS` -> ;DEFINITION DEFINITIONS`}\n");            
            parseDefinition(array, file);        
            parseDefinitions_(array, file);
            break;
        default:
            fprintf(file, "{DEFINITIONS` -> epsilon}\n");                        
            back_token(array);
            break;
    }
}

/*
DEFINITION -> VAR_DEFINITION | TYPE_DEFINITION
*/
void parseDefinition(arrayList *array, FILE *file) {
    token *token;
    token = next_token(array);
    switch(token->kind) {
        case ID_T:
            fprintf(file, "{DEFINITION -> VAR_DEFINITION}\n");
						//insert the new id name and receive a pointer to its location
						id_entry = insert(token->lexeme);
						//in case the returned pointer is empty because this id was already declared
						if(id_entry == NULL){ 
							error(DUPLICATED_DECLARATION_ERROR, token->line, token->lexeme, file); 
						}  
            parseVarDefinition(array, file);
            break;
        case TYPE_T:
            fprintf(file, "{DEFINITION -> TYPE_DEFINITION}\n");            
            parseTypeDefinition(array, file);
            break;
        default:
            back_token(array);
            break;
    }   
}

/*
VAR_DEFINITION -> id: VAR_DEFINITION`
*/
void parseVarDefinition(arrayList *array, FILE *file) {
    fprintf(file, "{VAR_DEFINITION -> id: VAR_DEFINITION`}\n");
    match(COLON_T, array, file);
    parseVarDefinition_(array, file);
}

/*
VAR_DEFINITION` -> BASIC_TYPE | type_name
*/
void parseVarDefinition_(arrayList *array, FILE *file) {
		int data_type;
    token *token;
    token = next_token(array);

    switch(token->kind) {
        case INTEGER_T:
            fprintf(file, "{VAR_DEFINITION` -> BASIC_TYPE}\n");                        
            fprintf(file, "{BASIC_TYPE -> integer}\n");
            break;
        case REAL_T:
            fprintf(file, "{VAR_DEFINITION` -> BASIC_TYPE}\n");                        
            fprintf(file, "{BASIC_TYPE -> real}\n");
            break;
        case ID_T:
            fprintf(file, "{VAR_DEFINITION` -> type_name}\n");
						//insert the new id name and receive a pointer to its location
						id_entry = insert(token->lexeme);
						//in case the returned pointer is empty because this id was already declared
						if(id_entry == NULL){ 
							error(DUPLICATED_DECLARATION_ERROR, token->line, token->lexeme, file); 
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
void parseTypeDefinition(arrayList *array, FILE *file) {
    fprintf(file, "{TYPE_DEFINITION -> type type_name is TYPE_INDICATOR}\n");
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

    switch(token->kind) {
        case INTEGER_T:
            fprintf(file, "{TYPE_INDICATOR -> BASIC_TYPE}\n");              
            fprintf(file, "{BASIC_TYPE -> integer}\n");
            break;            
        case REAL_T:
            fprintf(file, "{TYPE_INDICATOR -> BASIC_TYPE}\n");                          
            fprintf(file, "{BASIC_TYPE -> real}\n");
            break;
        case ARRAY_T:
            fprintf(file, "{TYPE_INDICATOR -> ARRAY_TYPE}\n");                          
            parseArrayType(array, file);
            break;
        case POINTER_T:
            fprintf(file, "{TYPE_INDICATOR -> POINTER_TYPE}\n");            
            parsePointerType(array, file);
            break;
        default:
            back_token(array);
            break;
    }
}

/*
BASIC_TYPE -> integer | real
*/
void parseBasicType(arrayList *array, FILE *file) {   
    token *token;
    token = next_token(array);

    switch(token->kind) {
        case INTEGER_T:
            fprintf(file, "{BASIC_TYPE -> integer}\n");
						BASIC_TYPE.datatype = INTEGER_T;
            break;
        case REAL_T:
            fprintf(file, "{BASIC_TYPE -> real}\n");
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
void parseArrayType(arrayList *array, FILE *file) {   
    fprintf(file, "{ARRAY_TYPE -> array[SIZE] of BASIC_TYPE}\n");
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
    parsePointerType_(array, file);
}

/*
POINTER_TYPE` -> BASIC_TYPE | type_name
*/
void parsePointerType_(arrayList *array, FILE *file) {   
    token *token;
    token = next_token(array);

    switch(token->kind) {
        case INTEGER_T:
            fprintf(file, "{POINTER_TYPE` -> BASIC_TYPE}\n");            
            fprintf(file, "{BASIC_TYPE -> integer}\n");
            break;            
        case REAL_T:
            fprintf(file, "{POINTER_TYPE` -> BASIC_TYPE}\n");                    
            fprintf(file, "{BASIC_TYPE -> real}\n");
            break;
        case ID_T:
            fprintf(file, "{POINTER_TYPE` -> type_name}\n");
            break;
        default:
            back_token(array);
            break;
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
COMMANDS -> COMMAND COMMANDS`
*/
void parseCommands(arrayList *array, FILE *file) {   
    fprintf(file, "{COMMANDS -> COMMAND COMMANDS`}\n");
    
    parseCommand(array, file);
    parseCommands_(array, file);
}

/*
COMMANDS` -> ;COMMANDS | epsilon
*/
void parseCommands_(arrayList *array, FILE *file) {
    token *token;
    token = next_token(array);

    switch(token->kind) {
        case SEMICOLON_T:
            fprintf(file, "{COMMANDS` -> ;COMMANDS}\n");            
            parseCommands(array, file);
            break;
        default:
            fprintf(file, "{COMMANDS` -> epsilon}\n");
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
void parseCommand(arrayList *array, FILE *file) {
    token *token;
    token = next_token(array);

    switch(token->kind) {
        case WHEN_T:
            fprintf(file, "{COMMAND -> when (EXPRESSION rel_op EXPRESSION) do COMMANDS; default COMMANDS; end_when}\n");
            match(LEFT_PARENTHESIS_T, array, file);
            parseExpression(array, file);
            match(REL_OP_T, array, file);
            parseExpression(array, file);
            match(RIGHT_PARENTHESIS_T, array, file);            
            match(DO_T, array, file);
            parseCommands(array, file);
            match(DEFAULT_T, array, file);
            parseCommands(array, file);
            match(END_WHEN_T, array, file);
            break;
        case FOR_T:
            fprintf(file, "{COMMAND -> for (id = EXPRESSION; id rel_op EXPRESSION; id++) COMMANDS; end_for}\n");
            match(LEFT_PARENTHESIS_T, array, file);
            match(ID_T, array, file);
						//insert the new id name and receive a pointer to its location
						id_entry = insert(token->lexeme);
						//in case the returned pointer is empty because this id was already declared
						if(id_entry == NULL){ 
							error(DUPLICATED_DECLARATION_ERROR, token->line, token->lexeme, file); 
						}  
            match(ASSIGNMENT_T, array, file);
            parseExpression(array, file);
            match(SEMICOLON_T, array, file);
            match(ID_T, array, file);
						id_entry = find(token->lexeme);						                   
						//VARIABLE_NOT_DECLARED_ERROR
						if (id_entry == NULL) { 
							error(VARIABLE_NOT_DECLARED_ERROR, token->line, token->lexeme, file); 
						}                          
            match(REL_OP_T, array, file);
            parseExpression(array, file);
            match(SEMICOLON_T, array, file);
            match(ID_T, array, file);
						id_entry = find(token->lexeme);						                   
						//VARIABLE_NOT_DECLARED_ERROR
						if (id_entry == NULL) { 
							error(VARIABLE_NOT_DECLARED_ERROR, token->line, token->lexeme, file); 
						}                   
            match(INCREMENT_T, array, file);
            match(RIGHT_PARENTHESIS_T, array, file);
            parseCommands(array, file);
            match(END_FOR_T, array, file);
            break;
        case ID_T:
						//insert the new id name and receive a pointer to its location
						id_entry = insert(token->lexeme);
						//in case the returned pointer is empty because this id was already declared
						if(id_entry == NULL){ 
							error(DUPLICATED_DECLARATION_ERROR, token->line, token->lexeme, file); 
						}
            token = next_token(array); // now token is either '=' or epsilon | '[' | ^
            if (token->kind == ASSIGNMENT_T) {
                token = next_token(array);
                if (token->kind == MALLOC_T) {
                    fprintf(file, "{COMMAND -> id = malloc(size_of(type_name))}\n");
                    match(LEFT_PARENTHESIS_T, array, file);
                    match(SIZE_OF_T, array, file);         
                    match(LEFT_PARENTHESIS_T, array, file);
                    match(ID_T, array, file);
										id_entry = find(token->lexeme);						                   
										//VARIABLE_NOT_DECLARED_ERROR
										if (id_entry == NULL) { 
											error(VARIABLE_NOT_DECLARED_ERROR, token->line, token->lexeme, file); 
										}  
                    match(RIGHT_PARENTHESIS_T, array, file);
                    match(RIGHT_PARENTHESIS_T, array, file);                                                               
                }
                else {
                    back_token(array);
                    fprintf(file, "{COMMAND -> RECEIVER = EXPRESSION}\n");
                    fprintf(file, "{RECEIVER` -> epsilon}\n");                    
                    parseExpression(array, file);
                }
            }
            else {
                back_token(array);                
                parseReceiver_(array, file);
                match(ASSIGNMENT_T, array, file);
                parseExpression(array, file);
            }
            break;
        case FREE_T:
            fprintf(file, "{COMMAND -> free(id)}\n");
            match(LEFT_PARENTHESIS_T, array, file);
            match(ID_T, array, file);
						id_entry = find(token->lexeme);						                   
						//VARIABLE_NOT_DECLARED_ERROR
						if (id_entry == NULL) { 
							error(VARIABLE_NOT_DECLARED_ERROR, token->line, token->lexeme, file); 
						}  
            match(RIGHT_PARENTHESIS_T, array, file);
            break;
        case BLOCK_T:
            fprintf(file, "{COMMAND -> BLOCK}\n");
            back_token(array);        
            parseBlock(array, file);
            break;
        default:
            back_token(array);
            break;
    }
}

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
            fprintf(file, "{RECEIVER` -> epsilon}\n");
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
						id_entry = find(token->lexeme);						                   
						//VARIABLE_NOT_DECLARED_ERROR
						if (id_entry == NULL) { 
							error(VARIABLE_NOT_DECLARED_ERROR, token->line, token->lexeme, file); 
						}      
            break;
        case SIZE_OF_T:
            fprintf(file, "{EXPRESSION -> size_of(type_name)}\n");
            match(LEFT_PARENTHESIS_T, array, file);
            match(ID_T, array, file);
						id_entry = find(token->lexeme);						                   
						//VARIABLE_NOT_DECLARED_ERROR
						if (id_entry == NULL) { 
							error(VARIABLE_NOT_DECLARED_ERROR, token->line, token->lexeme, file); 
						}  
            match(RIGHT_PARENTHESIS_T, array, file);    
            break;        
        case ID_T:
						id_entry = find(token->lexeme);
						//VARIABLE_NOT_DECLARED_ERROR
						if (id_entry == NULL) { 
							error(VARIABLE_NOT_DECLARED_ERROR, token->line, token->lexeme, file); 
						}
            fprintf(file, "{EXPRESSION -> id EXPRESSION`}\n");
						//TYPE_CONSISTENCY_ERROR - case integer is on the left and real is on the right of the assignment
						if(id_entry){
							if ( is_integer(id_entry) && EXPRESSION.datatype == REAL_NUM_T ){ 
								error(TYPE_CONSISTENCY_ERROR, token->line, token->lexeme, file);
							}
						}          
            parseExpression_(array, file);
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
                break;  
        }
}


void error(int errorType, int line, char* lexeme, FILE *file){
	fprintf(file,"\n---Semantic Error: detected in line %d, lexeme '%s'---", line, lexeme);
	switch(errorType){
		case DUPLICATED_DECLARATION_ERROR:
			fprintf(file,"\n---duplicated declaration of the same name within same scope is forbidden)---");
			break;
		case VARIABLE_NOT_DECLARED_ERROR:
			fprintf(file,"\n---variable is used without being declared---");
			break;
		case ASSIGNMENT_TO_CONSTANT_ERROR:
			fprintf(file,"\n---assignments to constants are forbidden---");
			break;
		case EXCEPTION_NAME_REFERENCED_ERROR:
			fprintf(file,"\n---exception name can be referenced only in a command raise; all other references to exceptions are illegal---");
			break;
		case TYPE_CONSISTENCY_ERROR:
			fprintf(file,"\n---left side integer and right side is real is forbidden---");
			break;
		case NON_EXCEPTION_IN_RAISE_ERROR:
			fprintf(file,"\n---referencing a non-exception object in command raise is forbidden---");
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