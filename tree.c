#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tree.h"
#include "mylib.h"

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

static tree_t tree_type;

/**
 * This function takes a string as input and prints it to stdin.
 *
 * @param s string to print
 */
void print_key(char* s) {
    printf("%s\n", s);
}

/**
 * Sets the root of tree
 *
 * @param s string to print
 */
tree setroot_black(tree t) {
    if (t==NULL) {
        printf("Node is null");
    } else {
        t->colour = BLACK;
    }
    return t;
}

void print_colour(tree t) {
    if (t->colour==RED) {
        printf("red: %s\n",t->key);
    } else {
        printf("black: %s\n",t->key);
    }
}



tree tree_new(tree_t t) {
    tree_type = t;
    return NULL;
}

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

void tree_preorder(tree t, void f(int freq, char* word)) {
    if (t==NULL) {
        return;
    }
    f(t->freq,t->key);
    tree_preorder(t->left,f);
    tree_preorder(t->right,f);
}

tree left_rotate(tree t) {
    /*keep track of original*/
    tree temp = t;
    /*change the root to point to its right child*/
    t = t->right;
    /*make the right child of temp (original root) point to
     * left child of the new root. */
    temp->right = t->left;
    /*now make the left child of the new root point to temp (old root)*/
    t->left = temp;
    return t;
}

tree right_rotate(tree r) {
    /*keep track of original*/
    tree temp = r;
    /*change the root to point to its left child*/
    r = r->left;
    /*make the left child of temp (original root) point to
     * right child of the new root. */
    temp->left = r->right;
    /*now make the right child of the new root point to temp (old root)*/
    r->right = temp;
    return r;
}


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

tree tree_insert(tree t, char* str) {

    if (t==NULL) {
        t = emalloc(sizeof *t);
        t->key = emalloc((strlen(str) + 1) * sizeof t->key[0]);
        strcpy(t->key, str);
        t->freq = 1;
        t->left = NULL;
        t->right = NULL;
        if (tree_type == RBT) {
            t->colour = RED; /*node is red by default*/
        }
    } else if (strcmp(t->key,str)==0) {
        t->freq++;
        ;
    } else if (strcmp(str,t->key)<0) {
        t->left = tree_insert(t->left, str);
    } else {
        t->right = tree_insert(t->right,str);
    }
    if (tree_type == RBT) {
        t = tree_fix(t);
    }
    return t;
}

int tree_search(tree t, char* str) {
    if (t==NULL) {
        /*empty tree, so string can't be found*/
        return 0;
    } else if (strcmp(t->key,str)==0) {
        /*found the string, return true*/
        return 1;
    } else if (strcmp(str,t->key) < 0) {
        /*node too big, search left subtree*/
        return tree_search(t->left,str);
    } else {
        /*node too small, search right subtree*/
        return tree_search(t->right,str);
    }
}


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

/* Input tree b and returns an int representing longest path between the root
   and furthest leaf node.*/
int tree_depth(tree t) {
    int l_height, r_height;
    if (t == NULL) {
        return -1;
    }
    l_height = tree_depth(t->left);
    r_height = tree_depth(t->right);

    return (l_height < r_height) ? (r_height + 1) : (l_height + 1);
    
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



