#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "veb.h"
#include "bst.h"

#define UNIVERSE_SIZE 65536  // 2^16
#define NUM_OPERATIONS 10000
#define NUM_TESTS 5

typedef struct {
    double insert_time;
    double search_time;
    double successor_time;
    double predecessor_time;
    double delete_time;
} benchmark_results;

benchmark_results benchmark_veb() {
    benchmark_results results = {0};
    veb_tree* V = veb_create(UNIVERSE_SIZE);
    clock_t start, end;
    
    // Array to store inserted numbers for later operations
    uint32_t* numbers = (uint32_t*)malloc(NUM_OPERATIONS * sizeof(uint32_t));
    
    // Insertion benchmark
    start = clock();
    for (int i = 0; i < NUM_OPERATIONS; i++) {
        numbers[i] = rand() % UNIVERSE_SIZE;
        veb_insert(V, numbers[i]);
    }
    end = clock();
    results.insert_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    // Search benchmark
    start = clock();
    for (int i = 0; i < NUM_OPERATIONS; i++) {
        veb_member(V, numbers[i]);
    }
    end = clock();
    results.search_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    // Successor benchmark
    start = clock();
    for (int i = 0; i < NUM_OPERATIONS; i++) {
        veb_successor(V, numbers[i]);
    }
    end = clock();
    results.successor_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    // Predecessor benchmark
    start = clock();
    for (int i = 0; i < NUM_OPERATIONS; i++) {
        veb_predecessor(V, numbers[i]);
    }
    end = clock();
    results.predecessor_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    // Delete benchmark
    start = clock();
    for (int i = 0; i < NUM_OPERATIONS; i++) {
        veb_delete(V, numbers[i]);
    }
    end = clock();
    results.delete_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    free(numbers);
    veb_destroy(V);
    return results;
}

benchmark_results benchmark_bst() {
    benchmark_results results = {0};
    bst_node* root = NULL;
    clock_t start, end;
    
    // Array to store inserted numbers for later operations
    uint32_t* numbers = (uint32_t*)malloc(NUM_OPERATIONS * sizeof(uint32_t));
    
    // Insertion benchmark
    start = clock();
    for (int i = 0; i < NUM_OPERATIONS; i++) {
        numbers[i] = rand() % UNIVERSE_SIZE;
        bst_insert(&root, numbers[i]);
    }
    end = clock();
    results.insert_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    // Search benchmark
    start = clock();
    for (int i = 0; i < NUM_OPERATIONS; i++) {
        bst_search(root, numbers[i]);
    }
    end = clock();
    results.search_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    // Successor benchmark
    start = clock();
    for (int i = 0; i < NUM_OPERATIONS; i++) {
        bst_successor(root, numbers[i]);
    }
    end = clock();
    results.successor_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    // Predecessor benchmark
    start = clock();
    for (int i = 0; i < NUM_OPERATIONS; i++) {
        bst_predecessor(root, numbers[i]);
    }
    end = clock();
    results.predecessor_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    // Delete benchmark
    start = clock();
    for (int i = 0; i < NUM_OPERATIONS; i++) {
        root = bst_delete(root, numbers[i]);
    }
    end = clock();
    results.delete_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    free(numbers);
    bst_destroy(root);
    return results;
}

void print_average_results(benchmark_results* results, int num_tests, const char* structure_name) {
    double avg_insert = 0, avg_search = 0, avg_succ = 0, avg_pred = 0, avg_delete = 0;
    
    for (int i = 0; i < num_tests; i++) {
        avg_insert += results[i].insert_time;
        avg_search += results[i].search_time;
        avg_succ += results[i].successor_time;
        avg_pred += results[i].predecessor_time;
        avg_delete += results[i].delete_time;
    }
    
    printf("\n%s Average Performance (%d tests):\n", structure_name, num_tests);
    printf("Insert Time:      %.6f seconds\n", avg_insert / num_tests);
    printf("Search Time:      %.6f seconds\n", avg_search / num_tests);
    printf("Successor Time:   %.6f seconds\n", avg_succ / num_tests);
    printf("Predecessor Time: %.6f seconds\n", avg_pred / num_tests);
    printf("Delete Time:      %.6f seconds\n", avg_delete / num_tests);
}

int main() {
    srand(time(NULL));
    
    benchmark_results veb_results[NUM_TESTS];
    benchmark_results bst_results[NUM_TESTS];
    
    printf("Starting benchmarks with:\n");
    printf("Universe Size: %d\n", UNIVERSE_SIZE);
    printf("Operations per test: %d\n", NUM_OPERATIONS);
    printf("Number of tests: %d\n", NUM_TESTS);
    
    for (int i = 0; i < NUM_TESTS; i++) {
        printf("\nRunning test %d of %d...\n", i + 1, NUM_TESTS);
        veb_results[i] = benchmark_veb();
        bst_results[i] = benchmark_bst();
    }
    
    print_average_results(veb_results, NUM_TESTS, "Van Emde Boas Tree");
    print_average_results(bst_results, NUM_TESTS, "Binary Search Tree");
    
    return 0;
}

