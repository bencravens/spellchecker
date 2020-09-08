#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "mylib.h"

#define ARRAY_LEN 10
#define NUM_WORDS 10

int main(void) {
    /*now test BST*/
    tree b = tree_new(BST); /*start with empty tree*/
    char word[256];
    while(getword(word,256,stdin) != EOF) {
        b = tree_insert(b,word);
    }

    printf("\n\n\n\n\n-----------------------\n\n\n\n");

    tree_preorder(b);
    if(b == NULL) {
        printf("This shit null");
    }
    printf("Tree depth: %d\n", tree_depth(b));
    tree_output_dot(b, stdin);
    tree_free(b);
 

    return EXIT_SUCCESS;
}
