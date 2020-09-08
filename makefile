all:
	gcc -W -Wall -O2 -ansi -pedantic -g tree.c mylib.c test.c -o output
	./output
