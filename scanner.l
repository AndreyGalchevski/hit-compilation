%{
#include "arrayList.c"
#include "token.c"
#include "lexUtils.c"

FILE* lexOutputFile;

int EndOF = 0;
arrayList arr;

%}

%option noyywrap
digit           [0-9]
letter          [a-zA-Z]
start           "start"
end             "end"
program         "program"
real            "real"
integer         "integer"
if              "if"
else            "else"
end_if          "end_if"
then            "then"
loop            "loop"
end_loop        "end_loop"
until           "until"

equal_greater_than   ">="
equal_smaller_than   "<="
not_equal            "!="
equal                "=="
increment            "+"
multiplication       "*"
smaller_than         "<"
greater_than         ">"


id			{letter}({letter}|{digit}|(("_")({letter}|{digit})))*
integer_number		({digit})+
floating	((({integer_number})"."({integer_number}))|({integer_number}))

comma           ","
space           " "
assignment      "="
left_bracket	"["
right_bracket	"]"
colon			":"
semicolon		";"

%x comment1 comment2

%%
{assignment}            {create_and_store_token(ASSIGNMENT, &arr, yytext, line_num); return 1;}
{integer}               {create_and_store_token(INT, &arr, yytext, line_num); return 1;}
{start}                 {create_and_store_token(START, &arr, yytext, line_num); return 1;}
{end}                   {create_and_store_token(END, &arr, yytext, line_num); return 1;}
{program}               {create_and_store_token(PROGRAM, &arr, yytext, line_num); return 1;}
{real}                  {create_and_store_token(REAL, &arr, yytext, line_num); return 1;}
{loop}                  {create_and_store_token(LOOP, &arr, yytext, line_num); return 1;}
{end_loop}              {create_and_store_token(END_LOOP, &arr, yytext, line_num); return 1;}
{until}                 {create_and_store_token(UNTIL, &arr, yytext, line_num); return 1;}
{if}                    {create_and_store_token(IF, &arr, yytext, line_num); return 1;}
{else}                  {create_and_store_token(ELSE, &arr, yytext, line_num); return 1;}
{then}                  {create_and_store_token(THEN, &arr, yytext, line_num); return 1;}
{end_if}                {create_and_store_token(END_IF, &arr, yytext, line_num); return 1;}
{increment}             {create_and_store_token(INC, &arr, yytext, line_num); return 1;}
{multiplication}        {create_and_store_token(MUL, &arr, yytext, line_num); return 1;}
{smaller_than}          {create_and_store_token(SMALLER_THAN, &arr, yytext, line_num); return 1;}
{greater_than}          {create_and_store_token(GREATER_THAN, &arr, yytext, line_num); return 1;}
{equal_greater_than}    {create_and_store_token(EQUAL_GREATER_THAN, &arr, yytext, line_num); return 1;}
{equal_smaller_than}    {create_and_store_token(EQUAL_SMALLER_THAN, &arr, yytext, line_num); return 1;}
{equal}                 {create_and_store_token(EQUAL, &arr, yytext, line_num); return 1;}
{not_equal}             {create_and_store_token(NOT_EQUAL, &arr, yytext, line_num); return 1;}
{left_bracket}          {create_and_store_token(LEFT_BRACKET, &arr, yytext, line_num); return 1;} 
{right_bracket}         {create_and_store_token(RIGHT_BRACKET, &arr, yytext, line_num); return 1;} 
{comma}                 {create_and_store_token(COMMA, &arr, yytext, line_num); return 1;}
{id}                    {create_and_store_token(ID, &arr, yytext, line_num); return 1;} 
{floating}              {create_and_store_token(NUMBER, &arr, yytext, line_num); return 1;} 
{colon}                 {create_and_store_token(COLON, &arr, yytext, line_num); return 1;} 
{semicolon}             {create_and_store_token(SEMICOLON, &arr, yytext, line_num); return 1;} 

\n                      {line_num++;}
<<EOF>>               {EndOF = 1; return 0;}
{space}    
\r	
\t	
.                       {printError(yytext, lexOutputFile);}
            
"/*" BEGIN(comment1);
<comment1>\n    line_num++;
<comment1>[^*/\n]* 
<comment1>"*" 
<comment1>"*/" BEGIN(0);

"//" BEGIN(comment2);
<comment2>[^\n]*
<comment2>\n {line_num++; BEGIN(0);}

%%

int main(int argc, char *argv[]){
    int testFileNumber = 1;

    const char* testFile1 = "/home/andrey/Documents/test1.txt";
    const char* testFile2 = "/home/andrey/Documents/test2.txt";

    const char* lexOutput1 = "/home/andrey/Documents/test1_lex.txt";
    const char* lexOutput2 = "/home/andrey/Documents/test2_lex.txt";

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
        printf("There is no test file in /home/andrey/Documents");
    }

    createNewArray(&arr);
    
    while(!EndOF){
         printToken(next_token(&arr), lexOutputFile);
    }

    fclose(lexOutputFile);

    return 1;
}