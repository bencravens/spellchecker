/**
 * Header file for tree.c.
 * tree.c is an implementation of a tree ADT, types include BST and RBT.
 * The library contains functions like insertion, search,
 * in-order and pre-order traversals.
 */

#ifndef TREE_H_
#define TREE_H_

typedef enum tree_e { BST, RBT } tree_t;
typedef struct tree_node *tree;

extern tree      tree_new(tree_t t);
extern int       tree_depth(tree b);
extern tree      tree_free(tree t);
extern int       tree_search(tree b, char* str);
extern void      print_key(char *s);
extern void      print_colour(tree b);
extern tree      setroot_black(tree b);
extern void      tree_inorder(tree b);
extern void      tree_preorder(tree b, void f(int freq, char* str));
extern tree      left_rotate(tree b);
extern tree      right_rotate(tree b);
extern tree      tree_insert(tree b, char* str);
extern void      tree_output_dot(tree t, FILE *out);

#endif
