all: compiler_test 

compiler_test:
	cc -o compiler_test compiler.c tokenize.c linkedlist.c symboltable.c find_symbols.c


clean:
	rm compiler_test

