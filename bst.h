#ifndef BST_H
#define BST_H

#include <stdint.h>
#include <stdbool.h>

typedef struct bst_node {
    uint32_t key;
    struct bst_node* left;
    struct bst_node* right;
} bst_node;

bst_node* bst_create_node(uint32_t key);
void bst_insert(bst_node** root, uint32_t key);
bool bst_search(bst_node* root, uint32_t key);
bst_node* bst_delete(bst_node* root, uint32_t key);
uint32_t bst_successor(bst_node* root, uint32_t key);
uint32_t bst_predecessor(bst_node* root, uint32_t key);
void bst_destroy(bst_node* root);

#endif