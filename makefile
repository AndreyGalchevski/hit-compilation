myscanner: lex.yy.c
	gcc lex.yy.c -ll -o scannerApp 

lex.yy.c: scanner.l
	lex scanner.l

clean:
	rm scannerApp
	rm lex.yy.c