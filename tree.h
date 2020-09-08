#ifndef TREE_H_
#define TREE_H_

typedef enum tree_e { BST, RBT } tree_t;
typedef struct tree_node *tree;

extern tree      tree_delete(tree b, char* str);
extern tree      tree_free(tree b);
extern void     tree_inorder(tree b);
extern tree      tree_insert(tree b, char* str);
extern tree      tree_new();
extern void     tree_preorder(tree b);
extern int      tree_search(tree b, char* str);
extern void     print_colour(tree b);
extern tree      right_rotate(tree b);
extern tree      left_rotate(tree b);
extern int      tree_depth(tree b);
extern void    tree_output_dot(tree t, FILE *out);


#endif
