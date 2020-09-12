#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>
#include <time.h>
#include "tree.h"
#include "mylib.h"

#define ARRAY_LEN 10
#define NUM_WORDS 10

static void print_info(int freq, char* word) {
    printf("%-4d %s\n",freq,word);
}

/*auxillary function to populate dictionary tree with words*/
tree writedict(tree dict, char* word) {
    while(getword(word,sizeof word,stdin) != EOF) {
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
    double fill_time=0;
    double search_time=0;
    clock_t tic, toc;
    int unknown_words = 0;
    /*keep track of which cases we have had (as sometimes when we pass one command line arg others should be ignored)*/
    int case_r = 0;
    int case_c = 0;
    int case_f = 0;
    int case_o = 0;
    /*reading in command line arguments, keeping track of which ones we have been passed*/
    while ((option = getopt(argc, argv, optstring)) != EOF) {
        switch (option) {
            case 'r':
                case_r = 1;
                break;
            case 'c':
                case_c = 1;
                break; 
            case 'd':
                break;
           case 'o':
                case_o = 1;
                break;
           case 'f':
                case_f = 1;
                break;
           case 'h':
                break;
            default:
                printf("invalid command line argument");
                break;
        }
    }
    /*optind is the argument of the option array we are at. need to reset it to iterate over command line args again*/
    /* we iterate over the command line args to execute the actions they specify, now that we know all the args that 
    * we have been passed (some args are mutually exclusive, while others are fine to combine*/
    optind = 0;
    /*now executing*/
    while ((option = getopt(argc, argv, optstring)) != EOF) {
        switch (option) {
            case 'r':
                /*declare tree as bst*/
                dict = tree_new(RBT); 
                /*time insertion */
                tic = clock();
                /*read words in from standard in, insert into our dictionary tree*/ 
                dict = writedict(dict, word);
                toc = clock();
                fill_time = (toc - tic) / ((double)CLOCKS_PER_SEC);               
                /*fix root colouring to be black*/
                dict = setroot(dict);
                break;
            case 'c':
                /*c's argument is available in the global
                * variable optarg */
                filename = optarg;
                /* if -r has been specified, we will have a tree already..
                * otherwise, we will make a tree... */
                if (case_r==0) {
                    /*declare tree as bst*/
                    dict = tree_new(BST); 
                    /*time insertion*/
                    tic = clock();
                    /*read words in from standard in, insert into our dictionary tree*/ 
                    dict = writedict(dict, word);
                    toc = clock();
                    fill_time = (toc - tic) / ((double)CLOCKS_PER_SEC);                 
                }
                /*open file to be spellchecked*/
                if (NULL == (target = fopen(filename, "r"))) {
                    fprintf(stderr, "ERROR: can't find file %s\n", filename);
                    return EXIT_FAILURE;
                }
                /*now read words in and spellcheck them*/
                /*time search*/
                tic = clock();
                while(getword(word,sizeof word,target) != EOF) {
                    if (tree_search(dict,word) == 0) {
                        fprintf(stdout,"%s\n",word);
                        unknown_words++;
                    }
                }
                toc = clock();
                search_time = (toc - tic) / ((double)CLOCKS_PER_SEC);
                fprintf(stderr, "Fill time     : %f\n", fill_time);
                fprintf(stderr, "Search time   : %f\n", search_time);
                fprintf(stderr, "Unknown words = %d\n", unknown_words);
                fclose(target);
                break; 
            case 'd':
                /* this option should print the depth of the 
                * tree to stdout and not do anything else */
                /*make our tree if we haven't already*/
                /*if we have already done case_c, ignore this arg*/
                if (case_c) {
                    break;
                }
                if (case_r==0) {
                    /*declare tree as bst*/
                    dict = tree_new(BST); 
                    /*read words in from standard in, insert into our dictionary tree*/ 
                    dict = writedict(dict, word);                 
                }
                depth = tree_depth(dict); 
                fprintf(stdout,"%d\n",depth);
                break;
            case 'f':
                /*write the dot output to filename instead
                * of the default filename if -o is also given.
                * filename is stored in the global string optarg */
                /*we should only do this option if case o has also been passed, and there is no case c...*/
                if (case_o==0 || case_c==1) {
                    break;
                }
                /* just in case there isn't already a tree ...*/
                if (case_r==0) {
                    /*declare tree as bst*/
                    dict = tree_new(BST); 
                    /*read words in from standard in, insert into our dictionary tree*/ 
                    dict = writedict(dict, word);
                }
                graph = fopen(optarg,"w");
                tree_output_dot(dict, graph);
                fclose(graph);
                break;
            case 'o':
                /*output a representation of the tree in dot form to the file
                * tree-view.dot using the functions given in output-dot.txt */
                /*if we have already done case c or case f, we should break..*/
                if (case_c) {
                    break;
                } else if (case_f) {
                    break;
                }
                graph = fopen("tree-view.dot","w");
                /* just in case there isn't already a tree ...*/
                if (case_r==0) {
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
