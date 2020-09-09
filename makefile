c:
	gcc -W -Wall -O2 -ansi -pedantic -g tree.c mylib.c main.c -o output
	./output < dictionary.txt -c "test.txt"
dot: 
	dot -Tpdf < tree-view.dot > tree-view.pdf
	okular tree-view.pdf
d:
	gcc -W -Wall -O2 -ansi -pedantic -g tree.c mylib.c main.c -o output
	./output < dictionary.txt -d
f:
	gcc -W -Wall -O2 -ansi -pedantic -g tree.c mylib.c main.c -o output
	./output < dictionary.txt -f "customdot.dot"
o:
	gcc -W -Wall -O2 -ansi -pedantic -g tree.c mylib.c main.c -o output
	./output < dictionary.txt -o
r:
	gcc -W -Wall -O2 -ansi -pedantic -g tree.c mylib.c main.c -o output
	./output < dictionary.txt -r
h:
	gcc -W -Wall -O2 -ansi -pedantic -g tree.c mylib.c main.c -o output
	./output < dictionary.txt -h
