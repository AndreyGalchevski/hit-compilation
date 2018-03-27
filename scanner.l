%{
#include "arrayList.c"
#include "token.c"
#include "lexUtils.c"

FILE* lexOutputFile;

int endOfFile = 0;
arrayList tokenList;

%}

%option noyywrap
digit           [0-9]
letter          [a-zA-Z]

block           "block"
begin           "begin"
end             "end"
type            "type"
is              "is"
integer         "integer"
real            "real"
array           "array"
of              "of"
when            "when"
do              "do"
default         "default"
end_when        "end_when"
for             "for"
end_for         "end_for"
malloc          "malloc"
size_of         "size_of"
free            "free"

pointer              "^"
address              "&"

addition            "+"
subtraction         "-"
multiplication      "*"
division            "/"
power               "**"
increment           "++"

smaller_than         "<"
greater_than         ">"
smaller_equal_than   "<="
greater_equal_than   ">="
not_equal            "!="
equal                "=="
assignment           "="

id			        {letter}({letter}|{digit}|(("_")({letter}|{digit})))*

integer_number		({digit})+
real_number	        ((({integer_number})"."({integer_number}))|({integer_number}))

colon			    ":"
semicolon		    ";"
left_parenthesis    "("
right_parenthesis   ")"
left_bracket	    "["
right_bracket	    "]"

new_line                "\n"
space                   " "
tab                     "\t"
carriage_return         "\r"

%%
{block}                 {create_and_store_token(BLOCK_T, &tokenList, yytext, line_num); return 1;}
{begin}                 {create_and_store_token(BEGIN_T, &tokenList, yytext, line_num); return 1;}
{end}                   {create_and_store_token(END_T, &tokenList, yytext, line_num); return 1;}
{type}                  {create_and_store_token(TYPE_T, &tokenList, yytext, line_num); return 1;}
{is}                    {create_and_store_token(IS_T, &tokenList, yytext, line_num); return 1;}
{integer}               {create_and_store_token(INTEGER_T, &tokenList, yytext, line_num); return 1;}
{real}                  {create_and_store_token(REAL_T, &tokenList, yytext, line_num); return 1;}
{array}                 {create_and_store_token(ARRAY_T, &tokenList, yytext, line_num); return 1;}
{of}                    {create_and_store_token(OF_T, &tokenList, yytext, line_num); return 1;}
{when}                  {create_and_store_token(WHEN_T, &tokenList, yytext, line_num); return 1;}
{do}                    {create_and_store_token(DO_T, &tokenList, yytext, line_num); return 1;}
{default}               {create_and_store_token(DEFAULT_T, &tokenList, yytext, line_num); return 1;}
{end_when}              {create_and_store_token(END_WHEN_T, &tokenList, yytext, line_num); return 1;}
{for}                   {create_and_store_token(FOR_T, &tokenList, yytext, line_num); return 1;}
{end_for}               {create_and_store_token(END_FOR_T, &tokenList, yytext, line_num); return 1;}
{malloc}                {create_and_store_token(MALLOC_T, &tokenList, yytext, line_num); return 1;}
{size_of}               {create_and_store_token(SIZE_OF_T, &tokenList, yytext, line_num); return 1;}
{free}                  {create_and_store_token(FREE_T, &tokenList, yytext, line_num); return 1;}

{addition}              {create_and_store_token(ADDITION_T, &tokenList, yytext, line_num); return 1;}
{subtraction}           {create_and_store_token(SUBTRACTION_T, &tokenList, yytext, line_num); return 1;}
{multiplication}        {create_and_store_token(MULTIPLICATION_T, &tokenList, yytext, line_num); return 1;}
{division}              {create_and_store_token(DIVISION_T, &tokenList, yytext, line_num); return 1;}
{power}                 {create_and_store_token(POWER_T, &tokenList, yytext, line_num); return 1;}
{increment}             {create_and_store_token(INCREMENT_T, &tokenList, yytext, line_num); return 1;}

{smaller_than}          {create_and_store_token(SMALLER_THAN_T, &tokenList, yytext, line_num); return 1;}
{greater_than}          {create_and_store_token(GREATER_THAN_T, &tokenList, yytext, line_num); return 1;}
{smaller_equal_than}    {create_and_store_token(SMALLER_EQUAL_THAN_T, &tokenList, yytext, line_num); return 1;}
{greater_equal_than}    {create_and_store_token(GREATER_EQUAL_THAN_T, &tokenList, yytext, line_num); return 1;}
{not_equal}             {create_and_store_token(NOT_EQUAL_T, &tokenList, yytext, line_num); return 1;}
{equal}                 {create_and_store_token(EQUAL_T, &tokenList, yytext, line_num); return 1;}
{assignment}            {create_and_store_token(ASSIGNMENT_T, &tokenList, yytext, line_num); return 1;}

{pointer}               {create_and_store_token(POINTER_T, &tokenList, yytext, line_num); return 1;}
{address}               {create_and_store_token(ADDRESS_T, &tokenList, yytext, line_num); return 1;}

{id}                    {create_and_store_token(ID_T, &tokenList, yytext, line_num); return 1;} 

{integer_number}        {create_and_store_token(INT_NUM_T, &tokenList, yytext, line_num); return 1;}
{real_number}           {create_and_store_token(REAL_NUM_T, &tokenList, yytext, line_num); return 1;}

{colon}                 {create_and_store_token(COLON_T, &tokenList, yytext, line_num); return 1;} 
{semicolon}             {create_and_store_token(SEMICOLON_T, &tokenList, yytext, line_num); return 1;} 
{left_parenthesis}      {create_and_store_token(LEFT_PARENTHESIS_T, &tokenList, yytext, line_num); return 1;} 
{right_parenthesis}     {create_and_store_token(RIGHT_PARENTHESIS_T, &tokenList, yytext, line_num); return 1;} 
{left_bracket}          {create_and_store_token(LEFT_BRACKET_T, &tokenList, yytext, line_num); return 1;} 
{right_bracket}         {create_and_store_token(RIGHT_BRACKET_T, &tokenList, yytext, line_num); return 1;} 

<<EOF>>                 {endOfFile = 1; return 0;}

{new_line}              {line_num++;}
{space}    
{tab}	
{carriage_return}
"--".*                  	
.                       {printError(yytext, lexOutputFile);}

%%

int main(int argc, char *argv[]){
    int testFileNumber = 1;

    const char* testFile1 = "/home/andrey/Documents/test1.txt";
    const char* testFile2 = "/home/andrey/Documents/test2.txt";

    const char* lexOutput1 = "/home/andrey/Documents/test1_328796511_300221280_lex.txt";
    const char* lexOutput2 = "/home/andrey/Documents/test2_328796511_300221280_lex.txt";

    printf("Enter test file number to run: 1 or 2\n");
    scanf("%d", &testFileNumber);

    if (testFileNumber !=1 && testFileNumber != 2) {
        printf("Please enter valid number: 1 or 2\n");
        return 0;
    }

    if (testFileNumber == 1) {
        yyin = fopen(testFile1, "r");
        lexOutputFile = fopen(lexOutput1, "w");
    }
    else if (testFileNumber == 2) {
        yyin = fopen(testFile2, "r");
        lexOutputFile = fopen(lexOutput2, "w");        
    }

    if (yyin == NULL) {
        printf("There is no test file in the specified directory");
    }

    createNewArray(&tokenList);
    
    while(!endOfFile){
         printToken(next_token(&tokenList), lexOutputFile);
    }

    fclose(lexOutputFile);

    return 1;
}