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
        printf("inserting %s\n",word);
        dict = tree_insert(dict,word);
    }

    if(dict == NULL) {
        printf("Empty dictionary..");
    }
    return dict;
}


int main(int argc, char* argv[]) {
    /*declaring constants*/
    clock_t tic, toc;
    const char* optstring = "c:df:orh";
    extern char* optarg;
    char* filename_c;
    char* filename_f;
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
    int unknown_words = 0;
    /*keep track of which cases we have had (as sometimes when we pass one command line arg others should be ignored)*/
    int case_r = 0;
    int case_c = 0;
    int case_d = 0;
    int case_o = 0;
    int case_f = 0;
    /*reading in command line arguments, keeping track of which ones we have been passed*/
    while ((option = getopt(argc, argv, optstring)) != EOF) {
        switch (option) {
            case 'r':
                case_r = 1;
                break;
            case 'c':
                case_c = 1;
                filename_c = optarg;
                break; 
            case 'd':
                case_d = 1;
                break;
           case 'o':
                case_o = 1;
                break;
           case 'f':
                case_f = 1;
                filename_f = optarg;
                break;
           case 'h':
                fprintf(stderr,"Usage: ./output [OPTION]... <STDIN>\n");
                fprintf(stderr,"\n");
                fprintf(stderr,"Perform various operations using a binary tree.  By default, words\n");
                fprintf(stderr,"are read from stdin and added to the tree, before being printed out\n");
                fprintf(stderr,"alongside their frequencies to stdout.\n");
                fprintf(stderr,"\n");
                fprintf(stderr," -c FILENAME  Check spelling of words in FILENAME using words\n");
                fprintf(stderr,"              read from stdin as the dictionary.  Print timing\n");
                fprintf(stderr,"              info & unknown words to stderr (ignore -d & -o)\n ");
                fprintf(stderr," -c FILENAME  Check spelling of words in FILENAME using words\n");
                fprintf(stderr,"              read from stdin as the dictionary.  Print timing\n");
                fprintf(stderr,"              info & unknown words to stderr (ignore -d & -o)\n");
                fprintf(stderr," -d           Only print the tree depth (ignore -o)\n");
                fprintf(stderr," -f FILENAME  Write DOT output to FILENAME (if -o given)\n");
                fprintf(stderr," -o           Output the tree in DOT form to file 'tree-view.dot'\n");
                fprintf(stderr," -r           Make the tree an RBT (the default is a BST)\n");
                fprintf(stderr,"\n");
                fprintf(stderr," -h           Print this message\n");
                exit(EXIT_FAILURE);
                break;
            default:
                fprintf(stderr,"Usage: ./output [OPTION]... <STDIN>\n");
                fprintf(stderr,"\n");
                fprintf(stderr,"Perform various operations using a binary tree.  By default, words\n");
                fprintf(stderr,"are read from stdin and added to the tree, before being printed out\n");
                fprintf(stderr,"alongside their frequencies to stdout.\n");
                fprintf(stderr,"\n");
                fprintf(stderr," -c FILENAME  Check spelling of words in FILENAME using words\n");
                fprintf(stderr,"              read from stdin as the dictionary.  Print timing\n");
                fprintf(stderr,"              info & unknown words to stderr (ignore -d & -o)\n ");
                fprintf(stderr," -c FILENAME  Check spelling of words in FILENAME using words\n");
                fprintf(stderr,"              read from stdin as the dictionary.  Print timing\n");
                fprintf(stderr,"              info & unknown words to stderr (ignore -d & -o)\n");
                fprintf(stderr," -d           Only print the tree depth (ignore -o)\n");
                fprintf(stderr," -f FILENAME  Write DOT output to FILENAME (if -o given)\n");
                fprintf(stderr," -o           Output the tree in DOT form to file 'tree-view.dot'\n");
                fprintf(stderr," -r           Make the tree an RBT (the default is a BST)\n");
                fprintf(stderr,"\n");
                fprintf(stderr," -h           Print this message\n");
                exit(EXIT_FAILURE);
                break;
        }
    }
    
    if (case_r) {
        /*start fill timer*/
        tic = clock();
        /*declare tree as rbt*/
        dict = tree_new(RBT); 
    } else {
        /* r is being called, so let's make a rbt here. */
        /*start fill timer*/
        tic = clock();
        /*declare tree as bst*/
        dict = tree_new(BST);
    } 

    /*read words in from standard in, insert into our dictionary tree*/ 
    while(getword(word,sizeof word,stdin) != EOF) {
        dict = tree_insert(dict,word);
    } 
    dict = writedict(dict, word);
    toc = clock();
    fill_time = (toc - tic) / ((double)CLOCKS_PER_SEC);               
    /*fix root colouring to be black*/
    if (case_r) {
        dict = setroot_black(dict);
    }

    if (case_c) {
        /*open file to be spellchecked*/
        if (NULL == (target = fopen(filename_c, "r"))) {
            fprintf(stderr, "ERROR: can't find file %s\n", filename_c);
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
    } else if (case_d) {
        depth = tree_depth(dict); 
        fprintf(stdout,"%d\n",depth);
    } else if (case_o) {
        if (case_f) {
            graph = fopen(filename_f,"w");
            printf("Creating dot file '%s'\n",filename_f);
        } else {
            graph = fopen("tree-view.dot","w"); 
            printf("Creating dot file 'tree-view.dot'\n");
        }
        tree_output_dot(dict, graph);
        fclose(graph);
    } else {
       tree_preorder(dict,print_info);
    }
    tree_free(dict);
    return EXIT_SUCCESS;
}
