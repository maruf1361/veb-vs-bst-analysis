#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "veb.h"

#define UNDEFINED UINT32_MAX

static uint32_t high(uint32_t x, uint32_t universe_size) {
    uint32_t shift = (uint32_t)ceil(log2(universe_size)) / 2;
    return x >> shift;
}

static uint32_t low(uint32_t x, uint32_t universe_size) {
    uint32_t shift = (uint32_t)ceil(log2(universe_size)) / 2;
    uint32_t mask = (1 << shift) - 1;
    return x & mask;
}

static uint32_t index(uint32_t high, uint32_t low, uint32_t universe_size) {
    uint32_t shift = (uint32_t)ceil(log2(universe_size)) / 2;
    return (high << shift) + low;
}

veb_tree* veb_create(uint32_t universe_size) {
    if (universe_size <= 1) return NULL;
    
    veb_tree* V = (veb_tree*)malloc(sizeof(veb_tree));
    if (!V) return NULL;
    
    V->universe_size = universe_size;
    V->min = UNDEFINED;
    V->max = UNDEFINED;
    V->min_set = false;
    V->max_set = false;
    
    uint32_t upper_sqrt = (uint32_t)ceil(sqrt(universe_size));
    uint32_t lower_sqrt = (uint32_t)floor(sqrt(universe_size));
    
    if (universe_size <= 2) {
        V->summary = NULL;
        V->cluster = NULL;
    } else {
        V->summary = veb_create(upper_sqrt);
        V->cluster = (veb_tree**)malloc(upper_sqrt * sizeof(veb_tree*));
        if (!V->cluster) {
            free(V);
            return NULL;
        }
        for (uint32_t i = 0; i < upper_sqrt; i++) {
            V->cluster[i] = veb_create(lower_sqrt);
        }
    }
    
    return V;
}

void veb_destroy(veb_tree* V) {
    if (!V) return;
    
    if (V->universe_size > 2) {
        uint32_t upper_sqrt = (uint32_t)ceil(sqrt(V->universe_size));
        veb_destroy(V->summary);
        for (uint32_t i = 0; i < upper_sqrt; i++) {
            veb_destroy(V->cluster[i]);
        }
        free(V->cluster);
    }
    free(V);
}

void veb_insert(veb_tree* V, uint32_t x) {
    if (!V) return;
    
    if (!V->min_set) {
        V->min = x;
        V->max = x;
        V->min_set = true;
        V->max_set = true;
        return;
    }
    
    if (x < V->min) {
        uint32_t temp = x;
        x = V->min;
        V->min = temp;
    }
    
    if (V->universe_size > 2) {
        uint32_t high_x = high(x, V->universe_size);
        uint32_t low_x = low(x, V->universe_size);
        
        if (!V->cluster[high_x]->min_set) {
            veb_insert(V->summary, high_x);
        }
        veb_insert(V->cluster[high_x], low_x);
    }
    
    if (x > V->max) {
        V->max = x;
        V->max_set = true;
    }
}

bool veb_member(veb_tree* V, uint32_t x) {
    if (!V || !V->min_set) return false;
    if (x == V->min || x == V->max) return true;
    if (V->universe_size <= 2) return false;
    
    return veb_member(V->cluster[high(x, V->universe_size)],
                     low(x, V->universe_size));
}

uint32_t veb_successor(veb_tree* V, uint32_t x) {
    if (!V || !V->min_set) return UNDEFINED;
    if (V->universe_size <= 2) {
        if (x == 0 && V->max == 1) return 1;
        return UNDEFINED;
    }
    
    if (!V->min_set || x > V->max) return UNDEFINED;
    if (x < V->min) return V->min;
    
    uint32_t high_x = high(x, V->universe_size);
    uint32_t low_x = low(x, V->universe_size);
    uint32_t max_low = V->cluster[high_x] ? V->cluster[high_x]->max : UNDEFINED;
    
    if (max_low != UNDEFINED && low_x < max_low) {
        uint32_t offset = veb_successor(V->cluster[high_x], low_x);
        return index(high_x, offset, V->universe_size);
    }
    
    uint32_t succ_cluster = veb_successor(V->summary, high_x);
    if (succ_cluster == UNDEFINED) return UNDEFINED;
    
    uint32_t offset = V->cluster[succ_cluster]->min;
    return index(succ_cluster, offset, V->universe_size);
}

uint32_t veb_predecessor(veb_tree* V, uint32_t x) {
    if (!V || !V->min_set) return UNDEFINED;
    if (V->universe_size <= 2) {
        if (x == 1 && V->min == 0) return 0;
        return UNDEFINED;
    }
    
    if (!V->min_set || x < V->min) return UNDEFINED;
    if (x > V->max) return V->max;
    
    uint32_t high_x = high(x, V->universe_size);
    uint32_t low_x = low(x, V->universe_size);
    uint32_t min_low = V->cluster[high_x] ? V->cluster[high_x]->min : UNDEFINED;
    
    if (min_low != UNDEFINED && low_x > min_low) {
        uint32_t offset = veb_predecessor(V->cluster[high_x], low_x);
        return index(high_x, offset, V->universe_size);
    }
    
    uint32_t pred_cluster = veb_predecessor(V->summary, high_x);
    if (pred_cluster == UNDEFINED) {
        if (V->min < x) return V->min;
        return UNDEFINED;
    }
    
    uint32_t offset = V->cluster[pred_cluster]->max;
    return index(pred_cluster, offset, V->universe_size);
}

void veb_delete(veb_tree* V, uint32_t x) {
    if (!V || !V->min_set) return;
    
    if (V->min == V->max) {
        V->min_set = false;
        V->max_set = false;
        return;
    }
    
    if (V->universe_size == 2) {
        if (x == 0) {
            V->min = 1;
        } else {
            V->min = 0;
        }
        V->max = V->min;
        return;
    }
    
    if (x == V->min) {
        uint32_t first_cluster = V->summary->min;
        x = index(first_cluster, V->cluster[first_cluster]->min, V->universe_size);
        V->min = x;
    }
    
    uint32_t high_x = high(x, V->universe_size);
    uint32_t low_x = low(x, V->universe_size);
    
    veb_delete(V->cluster[high_x], low_x);
    
    if (!V->cluster[high_x]->min_set) {
        veb_delete(V->summary, high_x);
        if (x == V->max) {
            uint32_t summary_max = V->summary->max;
            if (!V->summary->max_set) {
                V->max = V->min;
            } else {
                V->max = index(summary_max, V->cluster[summary_max]->max, V->universe_size);
            }
        }
    } else if (x == V->max) {
        V->max = index(high_x, V->cluster[high_x]->max, V->universe_size);
    }
}
