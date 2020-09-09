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
    tree dict = tree_new(RBT);
    char word[256];
    graph = fopen("tree-view.dot","w");
       
 
    /*reading in command line arguments*/
    while ((option = getopt(argc, argv, optstring)) != EOF) {
        switch (option) {
            case 'c':
                /*c's argument is available in the global
                * variable optarg */
                filename = optarg;
                break;
            case 'd':
                /* this option should print the depth of the 
                * tree to stdout and not do anything else */
                break;
            case 'f':
                /*write the dot output to filename instead
                * of the default filename if -o is also given.
                * filename is stored in the global string optarg */
                break;
            case 'o':
                /*output a representation of the tree in dot form to the file
                * tree-view.dot using the functions given in output-dot.txt */
                break;
            case 'r':
                /*make the tree an rbt instead of default bst*/
                break;
            case 'h':
                /* print a help message describing how to use the program */
                break;
            default:
                printf("invalid command line argument");
                break;
        }

    }

    /*read words in from standard in, insert into our dictionary tree*/ 
    while(getword(word,256,stdin) != EOF) {
        dict = tree_insert(dict,word);
    }

    /*traverse tree*/
    printf("printing dictionary. \n");
    tree_preorder(dict,print_info);
    printf("\n");
    if(dict == NULL) {
        printf("Empty dictionary..");
    }

    /*open file to be spellchecked*/
    if (NULL == (target = fopen(filename, "r"))) {
        fprintf(stderr, "ERROR: can't find file %s\n", filename);
        return EXIT_FAILURE;
    }
    /*now read words in and spellcheck them*/
    printf("printing misspelled words...\n");
    while (1 == fscanf(target,"%s",word)) {
        if (tree_search(dict,word) == 0) {
            fprintf(stderr,"%s\n",word);
        }
    } 
    /*output graph, free memory*/
    tree_output_dot(dict, graph);
    tree_free(dict);
    fclose(graph);
    fclose(target);
    return EXIT_SUCCESS;
}
