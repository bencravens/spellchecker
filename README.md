# spellchecker
 A spellchecker written from scratch in vanilla C. Uses an abstract "tree" data structure to compare words in a document with a given dictionary file.

Main calls on tree.c, an implementation of a tree ADT that can be either a BST or an RBT. Main has the functionality to use this tree ADT as a spellchecker. It does this by reading in a dictionary file through stdin, and making a tree based on the words in the dictionary. Then, when you give it another file to be spellchecked, it can check using tree search whether or not the words in the file it is spellchecking appear in the dictionary tree. The specific behaviour of this program depends on what combination of command line arguments are passed. 

*   -h prints a help message detailing the program.
*
*   -r initializes the tree as a red black tree.
*
*   -c filename checks the spelling in file "filename", 
*      and outputs timing information to stderr.
*
*   -d outputs the depth of the dictionary tree.
*
*   -o outputs a graph representation of the dictionary tree in the .dot format
*
*   -f filename is an optional addition to the -o that redirects the .dot 
*   output to the filename "filename".
