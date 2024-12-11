#ifndef VEB_H
#define VEB_H

#include <stdint.h>
#include <stdbool.h>

typedef struct veb_node {
    uint32_t universe_size;
    uint32_t min;
    uint32_t max;
    struct veb_node* summary;
    struct veb_node** cluster;
    bool min_set;
    bool max_set;
} veb_tree;

veb_tree* veb_create(uint32_t universe_size);
void veb_insert(veb_tree* V, uint32_t x);
bool veb_member(veb_tree* V, uint32_t x);
void veb_delete(veb_tree* V, uint32_t x);
uint32_t veb_successor(veb_tree* V, uint32_t x);
uint32_t veb_predecessor(veb_tree* V, uint32_t x);
void veb_destroy(veb_tree* V);

#endif
