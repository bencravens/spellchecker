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

/*global variable that determines tree type*/
static tree_t tree_type;
int max_depth = 0;

void print_key(char* s) {
    printf("%s\n", s);
}

tree setroot(tree b) {
    if (b==NULL) {
        printf("Node is null");
    } else {
        b->colour = BLACK;
    }
    return b;
}

void print_colour(tree b) {
    if (b->colour==RED) {
        printf("red: %s\n",b->key);
    } else {
        printf("black: %s\n",b->key);
    }
}

/*return a tree node pointer*/
struct tree_node* talloc(void) {
    return (struct tree_node * ) emalloc(sizeof(struct tree_node));
}

tree tree_new(tree_t t) {
    tree_type = t;
    return NULL;
}

void tree_inorder(tree b) {
    if (b==NULL) {
        return;
    } else {
        tree_inorder(b->left);
        if (tree_type==RBT) {
            if (b->colour==BLACK) {
                printf("black: %s\n",b->key);
            } else {
                printf("red: %s\n",b->key);
            }
        } else {
            printf("%s\n",b->key);
        } 
        tree_inorder(b->right);
    }
}

void tree_preorder(tree b, void f(int freq, char* word)) {
    if (b==NULL) {
        return;
    }
    f(b->freq,b->key);
    tree_preorder(b->left,f);
    tree_preorder(b->right,f);
}

tree left_rotate(tree b) {
    /*keep track of original*/
    tree temp = b;
    /*change the root to point to its right child*/
    b = b->right;
    /*make the right child of temp (original root) point to
     * left child of the new root. */
    temp->right = b->left;
    /*now make the left child of the new root point to temp (old root)*/
    b->left = temp;
    return b;
}

tree right_rotate(tree b) {
    /*keep track of original*/
    tree temp = b;
    /*change the root to point to its left child*/
    b = b->left;
    /*make the left child of temp (original root) point to
     * right child of the new root. */
    temp->left = b->right;
    /*now make the right child of the new root point to temp (old root)*/
    b->right = temp;
    return b;
}


static tree tree_fix(tree R) {
    if(IS_RED(R->left) && IS_RED(R->left->left)) {
        if (IS_RED(R->right)) {
            R->colour = RED;
            R->left->colour = BLACK;
            R->right->colour = BLACK;
        } else {
            R = right_rotate(R);
            R->colour = BLACK;
            R->right->colour = RED;
        } 
    } else if (IS_RED(R->left) && IS_RED(R->left->right)) {
        if (IS_RED(R->right)) {
            R->colour = RED;
            R->left->colour = BLACK;
            R->right->colour = BLACK;
        } else {
            R->left = left_rotate(R->left);
            R = right_rotate(R);
            R->colour = BLACK;
            R->right->colour = RED;
        }
    } else if (IS_RED(R->right) && IS_RED(R->right->left)) {
        if (IS_RED(R->left)) {
            R->colour = RED;
            R->left->colour = BLACK;
            R->right->colour = BLACK; 
        } else {
            R->right = right_rotate(R->right);
            R = left_rotate(R);
            R->colour = BLACK;
            R->left->colour = RED;
        } 
    } else if (IS_RED(R->right) && IS_RED(R->right->right)) {
        if (IS_RED(R->left)) {
            R->colour = RED;
            R->left->colour = BLACK;
            R->right->colour = BLACK;
        } else {
            R = left_rotate(R);
            R->colour = BLACK;
            R->left->colour = RED;
        }
    } 
    return R;
}

tree tree_insert(tree b, char* str) {
    int s;
    if (b==NULL) {
        b = emalloc(sizeof *b);
        s = (strlen(str)+1) * sizeof str[0];
        b->key = emalloc(s);
        strcpy(b->key, str);
        b->freq = 1;
        b->left = NULL;
        b->right = NULL;
        if (tree_type == RBT) {
            b->colour = RED; /*node is red by default*/
        }
    } else if (strcmp(b->key,str)==0) {
        b->freq++;
        ;
    } else if (strcmp(str,b->key)<0) {
        b->left = tree_insert(b->left, str);
    } else {
        b->right = tree_insert(b->right,str);
    }
    if (tree_type == RBT) {
        b = tree_fix(b);
    }
    return b;
}

int tree_search(tree b, char* str) {
    if (b==NULL) {
        /*empty tree, so string can't be found*/
        return 0;
    } else if (strcmp(b->key,str)==0) {
        /*found the string, return true*/
        return 1;
    } else if (strcmp(str,b->key) < 0) {
        /*node too big, search left subtree*/
        return tree_search(b->left,str);
    } else {
        /*node too small, search right subtree*/
        return tree_search(b->right,str);
    }
}


tree tree_free(tree b) {
    if (b==NULL) {
        return b;
    }
    if (b->left != NULL) {
        b->left = tree_free(b->left);
    }
    if (b->right != NULL) {
        b->right = tree_free(b->right);
    }
    free(b->key);
    free(b);
    return b;
}

/* Input tree b and returns an int representing longest path between the root
   and furthest leaf node.*/
int tree_depth(tree b) {
    int l_height, r_height;
    if (b == NULL) {
        return 0;
    }
    l_height = tree_depth(b->left);
    r_height = tree_depth(b->right);
    
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



