#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

#define UNDEFINED UINT32_MAX

bst_node* bst_create_node(uint32_t key) {
    bst_node* node = (bst_node*)malloc(sizeof(bst_node));
    if (!node) return NULL;
    
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void bst_insert(bst_node** root, uint32_t key) {
    if (!*root) {
        *root = bst_create_node(key);
        return;
    }
    
    if (key < (*root)->key)
        bst_insert(&(*root)->left, key);
    else if (key > (*root)->key)
        bst_insert(&(*root)->right, key);
}

bool bst_search(bst_node* root, uint32_t key) {
    if (!root) return false;
    
    if (root->key == key) return true;
    
    if (key < root->key)
        return bst_search(root->left, key);
    return bst_search(root->right, key);
}

static bst_node* find_min(bst_node* node) {
    if (!node) return NULL;
    while (node->left)
        node = node->left;
    return node;
}

static bst_node* find_max(bst_node* node) {
    if (!node) return NULL;
    while (node->right)
        node = node->right;
    return node;
}

bst_node* bst_delete(bst_node* root, uint32_t key) {
    if (!root) return NULL;
    
    if (key < root->key)
        root->left = bst_delete(root->left, key);
    else if (key > root->key)
        root->right = bst_delete(root->right, key);
    else {
        if (!root->left) {
            bst_node* temp = root->right;
            free(root);
            return temp;
        } else if (!root->right) {
            bst_node* temp = root->left;
            free(root);
            return temp;
        }
        
        bst_node* temp = find_min(root->right);
        root->key = temp->key;
        root->right = bst_delete(root->right, temp->key);
    }
    return root;
}

uint32_t bst_successor(bst_node* root, uint32_t key) {
    if (!root) return UNDEFINED;
    
    bst_node* succ = NULL;
    bst_node* current = root;
    
    while (current && current->key != key) {
        if (key < current->key) {
            succ = current;
            current = current->left;
        } else {
            current = current->right;
        }
    }
    
    if (!current) return UNDEFINED;
    
    if (current->right) {
        bst_node* temp = find_min(current->right);
        return temp->key;
    }
    
    return succ ? succ->key : UNDEFINED;
}

uint32_t bst_predecessor(bst_node* root, uint32_t key) {
    if (!root) return UNDEFINED;
    
    bst_node* pred = NULL;
    bst_node* current = root;
    
    while (current && current->key != key) {
        if (key > current->key) {
            pred = current;
            current = current->right;
        } else {
            current = current->left;
        }
    }
    
    if (!current) return UNDEFINED;
    
    if (current->left) {
        bst_node* temp = find_max(current->left);
        return temp->key;
    }
    
    return pred ? pred->key : UNDEFINED;
}

void bst_destroy(bst_node* root) {
    if (!root) return;
    
    bst_destroy(root->left);
    bst_destroy(root->right);
    free(root);
}