# 242assignment
## TODO: 
- [x] Create a tree ADT which combines rbt and bst functionality
- [x] Add a tree_depth() function which should return the length of the longest path between the root node and the furthest leaf node.
- [x] Add a frequency field to the struct tree_node and update the frequency whenever a duplicate node is added to the tree
- [x] Add the two output_dot graph printing functions which are found at /home/cshome/coursework/242/asgn-files/ on the COSC network
- [x] Fix the rbt code so that the root of the tree is always black
- [x] Add getopt code to main.c so that command line arguments can be passed to main function
- [x] Read words from stdin into our main.c 
- [x] Put the words into a tree. This tree represents our "dictionary" we are using for spellcheck
- [x] Read every word from filename.txt using getword. IF the word exists in our dictionary tree, then it is a valid word, in which case do nothing. ELSE, it is a misspelled word, so print it to stdout
- [ ] When finished running through filename.txt, print the timing information and unknown word count to stderr like this: https://imgur.com/MB35ceI.png
- [ ] add the following command line arguments to the script: (c, d, f, o, r, h) https://imgur.com/BZG1Ain.png
- [x] c (spellcheck)
- [ ] d (print depth of tree)
- [ ] f (dot output to custom filename)
- [ ] o (dot output)
- [ ] r (initialize tree as rbt)
- [ ] h (help message)

To compile and run the program, use "make", the linux command.
