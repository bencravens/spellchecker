#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "tree.h"
#include "mylib.h"

#define ARRAY_LEN 10
#define NUM_WORDS 10

static void print_info(int freq, char* word) {
    printf("%-4d %s\n",freq,word);
}

/*auxillary function to populate dictionary tree with words*/
tree writedict(tree dict, char word[256]) {
    while(getword(word,256,stdin) != EOF) {
        dict = tree_insert(dict,word);
    }

    if(dict == NULL) {
        printf("Empty dictionary..");
    }
    return dict;
}


int main(int argc, char* argv[]) {
    /*declaring constants*/
    const char *optstring = "c:df:orh";
    extern char* optarg;
    char* filename;
    char option;
    /*file pointer for graph output*/
    FILE* graph;
    /*file pointer for .txt file to be spellchecked*/
    FILE* target;
    /*tree depth*/
    int depth;
    char word[256];
    tree dict = NULL;
    
    /*reading in command line arguments*/
    while ((option = getopt(argc, argv, optstring)) != EOF) {
        switch (option) {
            case 'r':
                /*declare tree as bst*/
                dict = tree_new(RBT); 
                /*read words in from standard in, insert into our dictionary tree*/ 
                dict = writedict(dict, word);                 
                /*fix root colouring to be black*/
                dict = setroot(dict);
                break;
            case 'c':
                /*c's argument is available in the global
                * variable optarg */
                filename = optarg;
                /* if -r has been specified, we will have a tree already..
                * otherwise, we will make a tree... */
                if (dict == NULL) {
                    /*declare tree as bst*/
                    dict = tree_new(BST); 
                    /*read words in from standard in, insert into our dictionary tree*/ 
                    dict = writedict(dict, word);                 
                }
                /*open file to be spellchecked*/
                if (NULL == (target = fopen(filename, "r"))) {
                    fprintf(stderr, "ERROR: can't find file %s\n", filename);
                    return EXIT_FAILURE;
                }
                /*now read words in and spellcheck them*/
                while (1 == fscanf(target,"%s",word)) {
                    if (tree_search(dict,word) == 0) {
                        fprintf(stdout,"%s\n",word);
                    }
                }
                fclose(target);
                break; 
            case 'd':
                /* this option should print the depth of the 
                * tree to stdout and not do anything else */
                /*declare tree as bst*/
                dict = tree_new(BST); 
                /*read words in from standard in, insert into our dictionary tree*/ 
                dict = writedict(dict, word);                 
                depth = tree_depth(dict); 
                fprintf(stdout,"%d\n",depth);
                break;
            case 'f':
                /*write the dot output to filename instead
                * of the default filename if -o is also given.
                * filename is stored in the global string optarg */
                graph = fopen(optarg,"w");
                if (dict==NULL) {
                    /*declare tree as bst*/
                    dict = tree_new(BST); 
                    /*read words in from standard in, insert into our dictionary tree*/ 
                    dict = writedict(dict, word);                 
                }
                tree_output_dot(dict,graph); 
                fclose(graph);
                break;
            case 'o':
                /*output a representation of the tree in dot form to the file
                * tree-view.dot using the functions given in output-dot.txt */
                graph = fopen("tree-view.dot","w");
                /* just in case there isn't already a tree ...*/
                if (dict==NULL) {
                    /*declare tree as bst*/
                    dict = tree_new(BST); 
                    /*read words in from standard in, insert into our dictionary tree*/ 
                    dict = writedict(dict, word);
                }
                tree_output_dot(dict, graph);
                fclose(graph);
                break;
           case 'h':
                /* print a help message describing how to use the program */
                printf("this is a spellchecking program based on an implementation of a tree abstract data type, which supports both red black trees and binary search trees. \n");
                printf("it accepts the following command line arguments:\n");
                printf("-c filename: Check the spelling of words in filename using words read from stdin as the dictionary. Print all unknown words to stdout. Print timing information and unknown word count to stderr. WHen this option is given -d and -o are ignored.\n");
                printf("-d: print the depth of the tree to stdout\n");
                printf("-f filename: write the graph output to filename instead of the default tree-view.dot\n");
                printf("-o: output a representation of the tree in dot form to the file tree-view.dot\n");
                printf("-r: make the tree a red-black tree instead of a binary search tree.\n");
                printf("-h: print help message\n");
                break;
            default:
                printf("invalid command line argument");
                break;
        }

    }

    tree_free(dict);
    return EXIT_SUCCESS;
}
