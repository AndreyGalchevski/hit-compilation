myscanner: lex.yy.c
	gcc lex.yy.c -ll -o compiler 

lex.yy.c: scanner.l
	lex scanner.l

clean:
	rm compiler
	rm lex.yy.c