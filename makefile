all:
	gcc -W -Wall -O2 -ansi -pedantic -g tree.c mylib.c main.c -o output
	./output < dictionary.txt -c "test.txt"
dot: 
	dot -Tpdf < tree-view.dot > tree-view.pdf
	okular tree-view.pdf
	
