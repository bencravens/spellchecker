checkall:
	@make -s dotcheck &> /dev/null
	@make -s ccheck &> /dev/null
	@make -s defaultcheck &> /dev/null
	@make -s depthcheck &> /dev/null
	cat cdiff.txt
	cat defaultdiff.txt
	cat depthdiff.txt
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
	diff myoutput.txt sampleoutput.txt >> cdiff.txt
defaultcheck:
	touch myoutput.txt && touch sampleoutput.txt
	rm myoutput.txt
	rm sampleoutput.txt
	#compiling our program
	gcc -W -Wall -O2 -ansi -pedantic -g tree.c mylib.c main.c -o output
	#running our program with valgrind
	valgrind ./output < smalldictionary.txt >> myoutput.txt
	#running sample program with same settings... :
	./sample-asgn < smalldictionary.txt >> sampleoutput.txt
	#difference between the two
	diff myoutput.txt sampleoutput.txt >> defaultdiff.txt
depthcheck:
	rm myoutput.txt
	rm sampleoutput.txt
	#compiling our program
	gcc -W -Wall -O2 -ansi -pedantic -g tree.c mylib.c main.c -o output
	#running our program with valgrind
	valgrind ./output < smalldictionary.txt -r -d >> myoutput.txt
	#running sample program with same settings... :
	./sample-asgn < smalldictionary.txt -r -d >> sampleoutput.txt
	#difference between the two...
	diff myoutput.txt sampleoutput.txt >> depthdiff.txt
errorcheck:
	touch myoutput.txt && touch sampleoutput.txt
	rm myoutput.txt
	rm sampleoutput.txt
	#compiling our program
	gcc -W -Wall -O2 -ansi -pedantic -g tree.c mylib.c main.c -o output
	#running our program with valgrind
	./output < smalldictionary.txt  -asbeej >> myoutput.txt
	#difference between the two
	diff myoutput.txt sampleoutput.txt

