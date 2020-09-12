compile:
	gcc -W -Wall -O2 -ansi -pedantic -g tree.c mylib.c main.c -o output
dotcheck: 
	gcc -W -Wall -O2 -ansi -pedantic -g tree.c mylib.c main.c -o output
	./output < smalldictionary.txt -r -o -f "output.dot"
	./sample-asgn < smalldictionary.txt -r -o -f "sample.dot"
	dot -Tpdf < output.dot > output.pdf
	dot -Tpdf < sample.dot > sample.pdf
	atril sample.pdf output.pdf
ccheck:
	rm myoutput.txt
	rm sampleoutput.txt
	#compiling our program
	gcc -W -Wall -O2 -ansi -pedantic -g tree.c mylib.c main.c -o output
	#running our program with valgrind
	valgrind ./output < smalldictionary.txt -r -c test.txt >> myoutput.txt
	#now running sample program with same settings.. : 
	./sample-asgn < smalldictionary.txt -c test.txt >> sampleoutput.txt
	#the difference between the two...
	diff myoutput.txt sampleoutput.txt 
defcheck:
	rm myoutput.txt
	rm sampleoutput.txt
	#compiling our program
	gcc -W -Wall -O2 -ansi -pedantic -g tree.c mylib.c main.c -o output
	#running our program with valgrind
	valgrind ./output < smalldictionary.txt >> myoutput.txt
	#running sample program with same settings... :
	./sample-asgn < smalldictionary.txt >> sampleoutput.txt
	diff myoutput.txt sampleoutput.txt
