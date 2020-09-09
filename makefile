compile:
	gcc -W -Wall -O2 -ansi -pedantic -g tree.c mylib.c main.c -o output
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
memcheck:
	gcc -W -Wall -O2 -ansi -pedantic -g tree.c mylib.c main.c -o output
	valgrind ./output < smalldictionary.txt -r -c test.txt -f mydot.dot
	dot -Tpdf < mydot.dot > tree-view.pdf
	okular tree-view.pdf
	echo "now running sample program with same settings.."
	./sample-asgn < smalldictionary.txt -r -c test.txt
