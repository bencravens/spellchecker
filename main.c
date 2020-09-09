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
    const char *optstring = "ab:c";
    char option;
    FILE* ptr;
    tree b = tree_new(RBT);
    char word[256];
    ptr = fopen("tree-view.dot","w");
    
    /*reading in command line arguments*/
    while ((option = getopt(argc, argv, optstring)) != EOF) {
        switch (option) {
            case 'a':
                printf("it's A.\n");
                break;
            case 'b':
                /*b's argument is available in the global
                * variable optarg */
                printf("it's B, argument %s\n",optarg);
                break;
            case 'c':
                printf("it's C.\n");
                break;
            default:
                printf("invalid command line argument");
                break;
        }

    }
 
    while(getword(word,256,stdin) != EOF) {
        b = tree_insert(b,word);
    }

    printf("\n\n\n\n\n-----------------------\n\n\n\n");

    tree_preorder(b,print_info);
    if(b == NULL) {
        printf("This shit null");
    }
    printf("Tree depth: %d\n", tree_depth(b));
    tree_output_dot(b, ptr);
    tree_free(b);
    fclose(ptr);
    return EXIT_SUCCESS;
}
