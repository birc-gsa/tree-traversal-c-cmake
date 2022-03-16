#include "gsa.h"
#include <assert.h>

tree new_tree(int val, tree left, tree right)
{
    tree t = malloc(sizeof *t);
    assert(t);
    *t = (struct node){.val = val, .left = left, .right = right};
    return t;
}

// Using recursion here because I'm lazy, but you really
// should use a traversal that doesn't...
void free_tree(tree t)
{
    if (!t)
        return;

    free_tree(t->left);
    free_tree(t->right);
    free(t);
}
