#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tree.h"
#include "mylib.h"

/* Macros that return boolean values for if nodes are red/black */
#define IS_BLACK(x) ((NULL == (x)) || (BLACK == (x)->colour))
#define IS_RED(x) ((NULL != (x)) && (RED == (x)->colour)) 

typedef enum { RED, BLACK } tree_colour;

struct tree_node {
    char* key;
    tree_colour colour;
    tree left;
    tree right;
    int freq;
};

static tree_t tree_type; /* Either RBT or BST */

/**
 * Returns a new tree of type t and sets it to NULL.
 *
 * @param t The tree
 * @return tree A null tree.
 */
tree tree_new(tree_t t) {
    tree_type = t;
    return NULL;
}

/**
 * Returns the depth of a tree (longest path from root to leaves) recursively.
 * The base case is if the tree is null, return -1. Otherwise calculate heights
 * of left/right children while incrementing by 1 and return the largest.
 *
 * @param t Tree to find depth of.
 * @return Tree depth.
 */
int tree_depth(tree t) {
    int l_height, r_height;
    if (t == NULL) {
        return -1;
    }
    l_height = tree_depth(t->left)++;
    r_height = tree_depth(t->right)++;

    return (l_height < r_height) ? r_height : l_height;
}

/**
 * Frees the memory allocated to a tree recursively.
 * Work our way through the tree until the leaves are reached,
 * free the leaves and work back up the tree.
 *
 * @param t Tree to free.
 * @return A null tree
 */
tree tree_free(tree t) {
    if (t==NULL) {
        return t;
    }
    if (t->left != NULL) {
        t->left = tree_free(t->left);
    }
    if (t->right != NULL) {
        t->right = tree_free(t->right);
    }
    free(t->key);
    free(t);
    return t;
}

/**
 * Searches for a string in a tree recursively and returns a boolean 1 or 0
 * if the string is found or not respecitvely. 
 *
 * @param t Tree to search.
 * @param str String to find.
 * @return A boolean representing found or not.
 */
int tree_search(tree t, char* str) {
    if (t==NULL) {
        return 0;
        
    } else if (strcmp(t->key,str)==0) {
        return 1;
        
    } else if (strcmp(str,t->key) < 0) {
        return tree_search(t->left,str);
        
    } else {
        return tree_search(t->right,str);
        
    }
}

/**
 * This function takes a string as input and prints it to stdin.
 *
 * @param s string to print.
 */
void print_key(char* s) {
    printf("%s\n", s);
}

/**
 * Prints colour of the node of tree to stdin.
 *
 * @param t the tree.
 */
void print_colour(tree t) {
    if (t->colour==RED) {
        printf("red: %s\n",t->key);
    } else {
        printf("black: %s\n",t->key);
    }
}

/**
 * Sets the root of tree to black.
 *
 * @param t the tree.
 */
tree setroot_black(tree t) {
    if (t==NULL) {
        printf("Node is null");
    } else {
        t->colour = BLACK;
    }
    return t;
}





/**
 * An in-order traversal of a tree which prints the strings at each node.
 * If the tree is an RBT, it also prints the colour of the nodes.
 * 
 * @param t the tree to be traversed.
 */
void tree_inorder(tree t) {
    if (t==NULL) {
        return;
    } else {
        tree_inorder(t->left);
        if (tree_type==RBT) {
            if (t->colour==BLACK) {
                printf("black: %s\n",t->key);
            } else {
                printf("red: %s\n",t->key);
            }
        } else {
            printf("%s\n",t->key);
        } 
        tree_inorder(t->right);
    }
}

/**
 * A pre-order traversal of a tree which performs function f at each node.
 * The function parsed needs to have the same inputs specified in the header.
 *
 * An example of this would be function which prints the frequency of a word
 * at each node.
 *
 * @param t The tree to be traversed.
 * @param void f(int freq, char* word) Function to be used at the nodes.
 */
void tree_preorder(tree t, void f(int freq, char* word)) {
    if (t==NULL) {
        return;
    }
    f(t->freq,t->key);
    tree_preorder(t->left,f);
    tree_preorder(t->right,f);
}

/**
 * Rotates a tree to the left.
 * For application of function see tree_fix.
 * Specifcally only used on RBT trees.
 *
 * @param t The tree to be rotated.
 * @return Left rotated tree.
 */
tree left_rotate(tree r) {
    tree temp = r;
    r = r->right;
    temp->right = r->left;
    r->left = temp;
    return r;
}

/**
 * Rotates a tree to the right.
 * For application of function see tree_fix.
 * Specifcally only used on RBT trees.
 *
 * @param r The tree to be rotated.
 * @return Right rotated tree.
 */
tree right_rotate(tree r) {
    tree temp = r;
    r = r->left;
    temp->left = r->right;
    r->right = temp;
    return r;
}

/**
 * Fixes an RBT after an insertion is made.
 * Runs through a series of checks and performs left/right rotations
 * until the tree is a valid RBT.
 * 
 * @param r The RBT to be fixed
 * @return The fixed RBT tree.
 */
static tree tree_fix(tree r) {
    if(IS_RED(r->left) && IS_RED(r->left->left)) {
        if (IS_RED(r->right)) {
            r->colour = RED;
            r->left->colour = BLACK;
            r->right->colour = BLACK;
        } else {
            r = right_rotate(r);
            r->colour = BLACK;
            r->right->colour = RED;
        } 
    } else if (IS_RED(r->left) && IS_RED(r->left->right)) {
        if (IS_RED(r->right)) {
            r->colour = RED;
            r->left->colour = BLACK;
            r->right->colour = BLACK;
        } else {
            r->left = left_rotate(r->left);
            r = right_rotate(r);
            r->colour = BLACK;
            r->right->colour = RED;
        }
    } else if (IS_RED(r->right) && IS_RED(r->right->left)) {
        if (IS_RED(r->left)) {
            r->colour = RED;
            r->left->colour = BLACK;
            r->right->colour = BLACK; 
        } else {
            r->right = right_rotate(r->right);
            r = left_rotate(r);
            r->colour = BLACK;
            r->left->colour = RED;
        } 
    } else if (IS_RED(r->right) && IS_RED(r->right->right)) {
        if (IS_RED(r->left)) {
            r->colour = RED;
            r->left->colour = BLACK;
            r->right->colour = BLACK;
        } else {
            r = left_rotate(r);
            r->colour = BLACK;
            r->left->colour = RED;
        }
    } 
    return r;
}

/**
 * Inserts a string into a tree recursively.
 * The base case if the tree is null, create a tree and set as the string
 * to be inserted. If the string is already in the tree, increment
 * the frequency variable tied to that string. Otherwise call the function
 * recurrsively on the left/right subtrees.
 * 
 * @param t Tree to be inserted into.
 * @param str String to be inserted
 * @return The tree with inserted value/frequency updated.
 */
tree tree_insert(tree t, char* str) {

    if (t==NULL) {
        t = emalloc(sizeof *t);
        t->key = emalloc((strlen(str) + 1) * sizeof t->key[0]);
        strcpy(t->key, str);
        t->freq = 1;
        t->left = NULL;
        t->right = NULL;
        
        /* If the tree is an RBT, we colour the root node red as default */
        if (tree_type == RBT) {
            t->colour = RED;
        }
        
    } else if (strcmp(t->key,str)==0) {
        t->freq++;
        ;
    } else if (strcmp(str,t->key)<0) {
        t->left = tree_insert(t->left, str);
    } else {
        t->right = tree_insert(t->right,str);
    }
    /* If the tree is an RBT, we want to fix it so it's a valid RBT */
    if (tree_type == RBT) {
        t = tree_fix(t);
    }
    
    return t;
}

/**
 * Traverses the tree writing a DOT description about connections, and
 * possibly colours, to the given output stream.
 *
 * @param t the tree to output a DOT description of.
 * @param out the stream to write the DOT output to.
 */
static void tree_output_dot_aux(tree t, FILE *out) {
    if(t->key != NULL) {
        fprintf(out, "\"%s\"[label=\"{<f0>%s:%d|{<f1>|<f2>}}\"color=%s];\n",
                t->key, t->key, t->freq,
                (RBT == tree_type && RED == t->colour) ? "red":"black");
    }
    if(t->left != NULL) {
        tree_output_dot_aux(t->left, out);
        fprintf(out, "\"%s\":f1 -> \"%s\":f0;\n", t->key, t->left->key);
    }
    if(t->right != NULL) {
        tree_output_dot_aux(t->right, out);
        fprintf(out, "\"%s\":f2 -> \"%s\":f0;\n", t->key, t->right->key);
    }
}

/**
 * Output a DOT description of this tree to the given output stream.
 * DOT is a plain text graph description language (see www.graphviz.org).
 * You can create a viewable graph with the command
 *
 *    dot -Tpdf < graphfile.dot > graphfile.pdf
 *
 * You can also use png, ps, jpg, svg... instead of pdf
 *
 * @param t the tree to output the DOT description of.
 * @param out the stream to write the DOT description to.
 */
void tree_output_dot(tree t, FILE *out) {
    fprintf(out, "digraph tree {\nnode [shape = Mrecord, penwidth = 2];\n");
    tree_output_dot_aux(t, out);
    fprintf(out, "}\n");
}



