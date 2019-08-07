#include "../include/binary_tree.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

node_t  *allocnode()
{
    node_t *node;

    node = (node_t *)malloc(sizeof(node_t));
    node->key = NULL;
    node->data = NULL;
    node->left = NULL;
    node->right = NULL;
    return (node);
}

node_t  *insert(node_t *root, char *key, void *data)
{
    if (root == NULL)
    {
        root = allocnode();
        root->key = key;
        root->data = data;
    }
    else
    {
        if (strcmp(key, root->key) <= 0)
            root->left = insert(root->left, key, data);
        else
            root->right = insert(root->right, key, data);
    }
    return (root);
}

void    print_node(node_t *node)
{
    if (node == NULL)
        return ;
    print_node(node->left);
    print_node(node->left);
    printf("%s\n", node->key);

}

void    visit_tree(node_t *node, void (*fp)(node_t *root))
{
        if (node == NULL)
            return ;
        visit_tree(node->left, fp);
        visit_tree(node->right, fp);
        fp(node);
}

void    destroy_tree(node_t *node, void (*fdestroy)(node_t *root))
{
    if (node == NULL)
        return ;
    destroy_tree(node->left, fdestroy);
    destroy_tree(node->right, fdestroy);

    fdestroy(node);
}

