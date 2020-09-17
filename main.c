#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>
#include <time.h>
#include "tree.h"
#include "mylib.h"

/**
* 
* Given information about a node in a tree, print that info. 
* passed to a traverse function (preorder or inorder) which does this
* for each node in the tree.
* @param freq The number of times the given key has been inserted into the tree.
* @param word The key.
*/
static void print_info(int freq, char* word) {
    printf("%-4d %s\n",freq,word);
}

/**
*
* Main calls on tree.c, an implementation of a tree ADT that can be
* either a BST or an RBT. Main has the functionality to use this tree ADT as
* a spellchecker. It does this by reading in a dictionary file through 
* stdin, and making a tree based on the words in the dictionary. 
* Then, when you give it another file to be spellchecked, 
* it can check using tree search whether or not the words in 
* the file it is spellchecking appear in the dictionary tree. 
* The specific behaviour of this program depends on what 
* combination of command line arguments are passed. 
*
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
*
* @param argc The number of command line arguments.
* @param argv String containing command line arguments passed, to be
* processed by getopt.
*/
int main(int argc, char* argv[]) {
    clock_t tic, toc;
    double fill_time = 0;
    double search_time = 0;
    
    const char* optstring = "c:df:orh";
    extern char* optarg;
    char option;
    
    char* filename_c = emalloc(sizeof filename_c);
    char* filename_f = emalloc(sizeof filename_f);
    int filename_size;
    FILE* graph;
    FILE* target;
    
    int depth;
    int unknown_words = 0;
    char word[256];
    tree dict = NULL;
       
    /* Keep track of which cases we have had (as sometimes when we pass one 
     * command line arg others should be ignored) */
    int case_r = 0;
    int case_c = 0;
    int case_d = 0;
    int case_o = 0;
    int case_f = 0;

    /* Reading in command line arguments, keeping track of which ones we 
     * have been passed */
    while ((option = getopt(argc, argv, optstring)) != EOF) {
        switch (option) {
            case 'r':
                case_r = 1;
                break;
            case 'c':
                case_c = 1;
                filename_size = (strlen(optarg)+1) * sizeof filename_c;
                filename_c = erealloc(filename_c,filename_size );
                strcpy(filename_c, optarg);
                break; 
            case 'd':
                case_d = 1;
                break;
           case 'o':
                case_o = 1;
                break;
           case 'f':
                case_f = 1;
                filename_size = (strlen(optarg)+1) * sizeof filename_f; 
                filename_f = erealloc(filename_f,filename_size);
                strcpy(filename_f, optarg);
                break;
           case 'h':
                fprintf(stderr,"Usage: ./output [OPTION]... <STDIN>\n");
                fprintf(stderr,"\n");
                fprintf(stderr,"Perform various operations using a");
                fprintf(stderr," binary tree. ");
                fprintf(stderr,"By default, words\n");
                fprintf(stderr,"are read from stdin and");
                fprintf(stderr," added to the tree, "); 
                fprintf(stderr, "before being printed out\n");
                fprintf(stderr,"alongside their frequencies to stdout.\n");
                fprintf(stderr,"\n");
                fprintf(stderr,"-c FILENAME  Check spelling of words in");
                fprintf(stderr, " FILENAME using words\n");
                fprintf(stderr," read from stdin as the dictionary.");
                fprintf(stderr, " Print timing\n");
                fprintf(stderr," info & unknown words to stderr ");
                fprintf(stderr, " (ignore -d & -o)\n");
                fprintf(stderr,"-d Only print the tree depth (ignore -o)\n");
                fprintf(stderr,"-f FILENAME  Write DOT output ");
                fprintf(stderr ," to FILENAME (if -o given)\n");
                fprintf(stderr,"-o Output the tree in DOT form to");
                fprintf(stderr, " file 'tree-view.dot'\n");
                fprintf(stderr,"-r Make the tree an RBT (the default is a");
                fprintf(stderr,"  BST)\n");
                fprintf(stderr,"\n");
                fprintf(stderr,"-h           Print this message\n");
                exit(EXIT_FAILURE);
                break;
            default:
                fprintf(stderr,"Usage: ./output [OPTION]... <STDIN>\n");
                fprintf(stderr,"\n");
                fprintf(stderr,"Perform various operations using a");
                fprintf(stderr," binary tree. ");
                fprintf(stderr,"By default, words\n");
                fprintf(stderr,"are read from stdin and");
                fprintf(stderr," added to the tree, "); 
                fprintf(stderr, "before being printed out\n");
                fprintf(stderr,"alongside their frequencies to stdout.\n");
                fprintf(stderr,"\n");
                fprintf(stderr,"-c FILENAME  Check spelling of words in");
                fprintf(stderr, " FILENAME using words\n");
                fprintf(stderr," read from stdin as the dictionary.");
                fprintf(stderr, " Print timing\n");
                fprintf(stderr," info & unknown words to stderr ");
                fprintf(stderr," (ignore -d & -o)\n");
                fprintf(stderr,"-d Only print the tree depth (ignore -o)\n");
                fprintf(stderr,"-f FILENAME  Write DOT output ");
                fprintf(stderr ," to FILENAME (if -o given)\n");
                fprintf(stderr,"-o Output the tree in DOT form to");
                fprintf(stderr, " file 'tree-view.dot'\n");
                fprintf(stderr,"-r Make the tree an RBT (the default is a");
                fprintf(stderr,"  BST)\n");
                fprintf(stderr,"\n");
                fprintf(stderr,"-h           Print this message\n");
                exit(EXIT_FAILURE);
                break;
        }
    }

    /* Initialise tree to RBT or BST */         
    if (case_r) {
        dict = tree_new(RBT); 
    } else {
        dict = tree_new(BST);
    } 
    tic = clock();        /* Start fill timer */
    /* Read words in from standard in, insert into our dictionary tree */ 
    while(getword(word,sizeof word,stdin) != EOF) {
        dict = tree_insert(dict,word);
    } 
    toc = clock();       /* Stop fill timer */
    fill_time = (toc - tic) / ((double)CLOCKS_PER_SEC);               
    /* If we have an RBT, fix root colour to be black */
    if (case_r) {
        dict = setroot_black(dict);
    }

    /* Now we go in order of command line argument precedence */
    if (case_c) {
        /* Open file to be spellchecked */
        if (NULL == (target = fopen(filename_c, "r"))) {
            fprintf(stderr, "ERROR: can't open file %s\n", filename_c);
            return EXIT_FAILURE;
        }
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
        
    } else if (case_d) {
        depth = tree_depth(dict); 
        fprintf(stdout,"%d\n",depth);
    } else if (case_o) {
        /* Output graph, use custom filename if f is supplied. */
        if (case_f) {
            if (NULL == (graph = fopen(filename_f, "r"))) {
                fprintf(stderr, "ERROR: can't open file %s \n", filename_f);
                return EXIT_FAILURE;
            } 
            printf("Creating dot file '%s'\n",filename_f);
        } else {
            graph = fopen("tree-view.dot","w"); 
            printf("Creating dot file 'tree-view.dot'\n");
        }
        tree_output_dot(dict, graph);
        fclose(graph);
    } else {
        /* Default case */
       tree_preorder(dict,print_info);
    }
    
    free(filename_c);
    free(filename_f);
    tree_free(dict);
    
    return EXIT_SUCCESS;
}
