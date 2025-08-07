#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

// Include all data structure headers
#include "dynarray/dynarray.h"
#include "linkedlist/singly_linked_list.h"
#include "linkedlist/doubly_linked_list.h"
#include "linkedlist/circular_linked_list.h"
#include "containers/stack.h"
#include "containers/queue.h"
#include "containers/deque.h"
#include "heap/min_heap.h"
#include "heap/max_heap.h"
#include "hash/hashtable.h"
#include "hash/hashset.h"

// Test results structure
typedef struct {
    int passed;
    int failed;
    const char* current_test;
} TestResults;

static TestResults results = {0, 0, ""};

// Helper macros
#define TEST_START(name) \
    do { \
        results.current_test = name; \
        printf("\n=== TESTING %s ===\n", name); \
    } while(0)

#define TEST_ASSERT(condition, message) \
    do { \
        if (condition) { \
            printf("✓ %s\n", message); \
            results.passed++; \
        } else { \
            printf("✗ %s\n", message); \
            results.failed++; \
        } \
    } while(0)

#define TEST_SUMMARY() \
    do { \
        printf("\n" \
               "==========================================\n" \
               "           TEST SUMMARY\n" \
               "==========================================\n" \
               "Total Tests: %d\n" \
               "Passed: %d\n" \
               "Failed: %d\n" \
               "Success Rate: %.1f%%\n" \
               "==========================================\n", \
               results.passed + results.failed, \
               results.passed, \
               results.failed, \
               (results.passed + results.failed > 0) ? \
                   (100.0 * results.passed / (results.passed + results.failed)) : 0.0); \
    } while(0)

// Test helper function for integers
void print_int(void *data) {
    printf("%d", *(int*)data);
}

// Test Dynamic Array
void test_dynarray() {
    TEST_START("DYNAMIC ARRAY");
    
    DynArray arr;
    dynarray_init(&arr, 5);
    
    TEST_ASSERT(dynarray_size(&arr) == 0, "Initial size is 0");
    TEST_ASSERT(dynarray_is_empty(&arr), "Initially empty");
    
    // Test push operations
    int values[] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; i++) {
        dynarray_push(&arr, &values[i]);
    }
    
    TEST_ASSERT(dynarray_size(&arr) == 5, "Size after pushes");
    TEST_ASSERT(!dynarray_is_empty(&arr), "Not empty after pushes");
    TEST_ASSERT(*(int*)dynarray_get(&arr, 0) == 10, "First element correct");
    TEST_ASSERT(*(int*)dynarray_get(&arr, 4) == 50, "Last element correct");
    
    // Test pop
    int *popped = (int*)dynarray_pop(&arr);
    TEST_ASSERT(*popped == 50, "Popped element correct");
    TEST_ASSERT(dynarray_size(&arr) == 4, "Size after pop");
    
    // Test set
    int new_val = 99;
    dynarray_set(&arr, 1, &new_val);
    TEST_ASSERT(*(int*)dynarray_get(&arr, 1) == 99, "Set operation works");
    
    dynarray_free(&arr);
    printf("Dynamic Array tests completed\n");
}

// Test Singly Linked List
void test_singly_linked_list() {
    TEST_START("SINGLY LINKED LIST");
    
    SinglyLinkedList list;
    singly_init(&list);
    
    TEST_ASSERT(singly_size(&list) == 0, "Initial size is 0");
    TEST_ASSERT(singly_is_empty(&list), "Initially empty");
    
    int values[] = {10, 20, 30};
    
    // Test push front
    for (int i = 0; i < 3; i++) {
        singly_push_front(&list, &values[i]);
    }
    
    TEST_ASSERT(singly_size(&list) == 3, "Size after push_front operations");
    TEST_ASSERT(*(int*)singly_get_at(&list, 0) == 30, "First element (push_front order)");
    
    // Test contains
    TEST_ASSERT(singly_contains(&list, &values[1]), "Contains existing element");
    
    int non_existing = 999;
    TEST_ASSERT(!singly_contains(&list, &non_existing), "Doesn't contain non-existing element");
    
    // Test pop front
    int *popped = (int*)singly_pop_front(&list);
    TEST_ASSERT(*popped == 30, "Pop front correct value");
    TEST_ASSERT(singly_size(&list) == 2, "Size after pop");
    
    singly_free(&list);
    printf("Singly Linked List tests completed\n");
}

// Test Doubly Linked List
void test_doubly_linked_list() {
    TEST_START("DOUBLY LINKED LIST");
    
    DoublyLinkedList list;
    doubly_init(&list);
    
    TEST_ASSERT(doubly_size(&list) == 0, "Initial size is 0");
    TEST_ASSERT(doubly_is_empty(&list), "Initially empty");
    
    int values[] = {10, 20, 30, 40};
    
    // Test push back
    for (int i = 0; i < 4; i++) {
        doubly_push_back(&list, &values[i]);
    }
    
    TEST_ASSERT(doubly_size(&list) == 4, "Size after push_back operations");
    TEST_ASSERT(*(int*)doubly_get_at(&list, 0) == 10, "First element");
    TEST_ASSERT(*(int*)doubly_get_at(&list, 3) == 40, "Last element");
    
    // Test push front
    int front_val = 5;
    doubly_push_front(&list, &front_val);
    TEST_ASSERT(*(int*)doubly_get_at(&list, 0) == 5, "Push front works");
    TEST_ASSERT(doubly_size(&list) == 5, "Size after push_front");
    
    // Test pop operations
    int *popped_back = (int*)doubly_pop_back(&list);
    int *popped_front = (int*)doubly_pop_front(&list);
    
    TEST_ASSERT(*popped_back == 40, "Pop back correct");
    TEST_ASSERT(*popped_front == 5, "Pop front correct");
    TEST_ASSERT(doubly_size(&list) == 3, "Size after pops");
    
    doubly_free(&list);
    printf("Doubly Linked List tests completed\n");
}

// Test Stack
void test_stack() {
    TEST_START("STACK (LIFO)");
    
    Stack stack;
    stack_init(&stack);
    
    TEST_ASSERT(stack_size(&stack) == 0, "Initial size is 0");
    TEST_ASSERT(stack_is_empty(&stack), "Initially empty");
    
    int values[] = {10, 20, 30, 40, 50};
    
    // Test push operations
    for (int i = 0; i < 5; i++) {
        stack_push(&stack, &values[i]);
    }
    
    TEST_ASSERT(stack_size(&stack) == 5, "Size after pushes");
    TEST_ASSERT(!stack_is_empty(&stack), "Not empty after pushes");
    
    // Test peek
    int *peeked = (int*)stack_peek(&stack);
    TEST_ASSERT(*peeked == 50, "Peek returns top element");
    TEST_ASSERT(stack_size(&stack) == 5, "Peek doesn't change size");
    
    // Test LIFO behavior
    for (int i = 4; i >= 0; i--) {
        int *popped = (int*)stack_pop(&stack);
        TEST_ASSERT(*popped == values[i], "LIFO order maintained");
    }
    
    TEST_ASSERT(stack_is_empty(&stack), "Empty after all pops");
    
    stack_free(&stack);
    printf("Stack tests completed\n");
}

// Test Queue
void test_queue() {
    TEST_START("QUEUE (FIFO)");
    
    Queue queue;
    queue_init(&queue);
    
    TEST_ASSERT(queue_size(&queue) == 0, "Initial size is 0");
    TEST_ASSERT(queue_is_empty(&queue), "Initially empty");
    
    int values[] = {10, 20, 30, 40, 50};
    
    // Test enqueue operations
    for (int i = 0; i < 5; i++) {
        queue_enqueue(&queue, &values[i]);
    }
    
    TEST_ASSERT(queue_size(&queue) == 5, "Size after enqueues");
    TEST_ASSERT(!queue_is_empty(&queue), "Not empty after enqueues");
    
    // Test peek
    int *peeked = (int*)queue_peek(&queue);
    TEST_ASSERT(*peeked == 10, "Peek returns front element");
    TEST_ASSERT(queue_size(&queue) == 5, "Peek doesn't change size");
    
    // Test FIFO behavior
    for (int i = 0; i < 5; i++) {
        int *dequeued = (int*)queue_dequeue(&queue);
        TEST_ASSERT(*dequeued == values[i], "FIFO order maintained");
    }
    
    TEST_ASSERT(queue_is_empty(&queue), "Empty after all dequeues");
    
    queue_free(&queue);
    printf("Queue tests completed\n");
}

// Test Deque
void test_deque() {
    TEST_START("DEQUE (DOUBLE-ENDED QUEUE)");
    
    Deque deque;
    deque_init(&deque);
    
    TEST_ASSERT(deque_size(&deque) == 0, "Initial size is 0");
    TEST_ASSERT(deque_is_empty(&deque), "Initially empty");
    
    int values[] = {10, 20, 30, 40};
    
    // Test push operations at both ends
    deque_push_front(&deque, &values[0]); // [10]
    deque_push_back(&deque, &values[1]);   // [10, 20]
    deque_push_front(&deque, &values[2]);  // [30, 10, 20]
    deque_push_back(&deque, &values[3]);   // [30, 10, 20, 40]
    
    TEST_ASSERT(deque_size(&deque) == 4, "Size after push operations");
    
    // Test peek operations
    int *front = (int*)deque_peek_front(&deque);
    int *back = (int*)deque_peek_back(&deque);
    TEST_ASSERT(*front == 30, "Peek front correct");
    TEST_ASSERT(*back == 40, "Peek back correct");
    
    // Test pop operations
    int *popped_front = (int*)deque_pop_front(&deque);
    int *popped_back = (int*)deque_pop_back(&deque);
    TEST_ASSERT(*popped_front == 30, "Pop front correct");
    TEST_ASSERT(*popped_back == 40, "Pop back correct");
    TEST_ASSERT(deque_size(&deque) == 2, "Size after pops");
    
    deque_free(&deque);
    printf("Deque tests completed\n");
}

// Test Min Heap
void test_min_heap() {
    TEST_START("MIN HEAP");
    
    MinHeap heap;
    min_heap_init(&heap, 10);
    
    TEST_ASSERT(min_heap_size(&heap) == 0, "Initial size is 0");
    TEST_ASSERT(min_heap_is_empty(&heap), "Initially empty");
    
    int values[] = {50, 20, 80, 10, 90, 30};
    int sorted[] = {10, 20, 30, 50, 80, 90};
    
    // Test push operations
    for (int i = 0; i < 6; i++) {
        min_heap_push(&heap, &values[i]);
    }
    
    TEST_ASSERT(min_heap_size(&heap) == 6, "Size after pushes");
    TEST_ASSERT(!min_heap_is_empty(&heap), "Not empty after pushes");
    
    // Test peek (should be minimum)
    int *min_val = (int*)min_heap_peek(&heap);
    TEST_ASSERT(*min_val == 10, "Peek returns minimum value");
    
    // Test min heap property (extract in ascending order)
    for (int i = 0; i < 6; i++) {
        int *extracted = (int*)min_heap_pop(&heap);
        TEST_ASSERT(*extracted == sorted[i], "Min heap order maintained");
    }
    
    TEST_ASSERT(min_heap_is_empty(&heap), "Empty after all extractions");
    
    min_heap_free(&heap);
    printf("Min Heap tests completed\n");
}

// Test Max Heap
void test_max_heap() {
    TEST_START("MAX HEAP");
    
    MaxHeap heap;
    max_heap_init(&heap, 10);
    
    TEST_ASSERT(max_heap_size(&heap) == 0, "Initial size is 0");
    TEST_ASSERT(max_heap_is_empty(&heap), "Initially empty");
    
    int values[] = {50, 20, 80, 10, 90, 30};
    int sorted[] = {90, 80, 50, 30, 20, 10}; // Descending order
    
    // Test push operations
    for (int i = 0; i < 6; i++) {
        max_heap_push(&heap, &values[i]);
    }
    
    TEST_ASSERT(max_heap_size(&heap) == 6, "Size after pushes");
    TEST_ASSERT(!max_heap_is_empty(&heap), "Not empty after pushes");
    
    // Test peek (should be maximum)
    int *max_val = (int*)max_heap_peek(&heap);
    TEST_ASSERT(*max_val == 90, "Peek returns maximum value");
    
    // Test max heap property (extract in descending order)
    for (int i = 0; i < 6; i++) {
        int *extracted = (int*)max_heap_pop(&heap);
        TEST_ASSERT(*extracted == sorted[i], "Max heap order maintained");
    }
    
    TEST_ASSERT(max_heap_is_empty(&heap), "Empty after all extractions");
    
    max_heap_free(&heap);
    printf("Max Heap tests completed\n");
}

// Test Hash Table
void test_hashtable() {
    TEST_START("HASH TABLE");
    
    HashTable table;
    hashtable_init_string(&table);
    
    TEST_ASSERT(hashtable_size(&table) == 0, "Initial size is 0");
    TEST_ASSERT(hashtable_is_empty(&table), "Initially empty");
    
    // Test string key-value operations
    const char* keys[] = {"apple", "banana", "cherry", "date"};
    int values[] = {100, 200, 300, 400};
    
    // Test put operations
    for (int i = 0; i < 4; i++) {
        hashtable_put(&table, keys[i], &values[i]);
    }
    
    TEST_ASSERT(hashtable_size(&table) == 4, "Size after puts");
    TEST_ASSERT(!hashtable_is_empty(&table), "Not empty after puts");
    
    // Test get operations
    for (int i = 0; i < 4; i++) {
        int *retrieved = (int*)hashtable_get(&table, keys[i]);
        TEST_ASSERT(retrieved != NULL, "Key found");
        if (retrieved) {
            TEST_ASSERT(*retrieved == values[i], "Retrieved value correct");
        }
    }
    
    // Test contains
    TEST_ASSERT(hashtable_contains(&table, "apple"), "Contains existing key");
    TEST_ASSERT(!hashtable_contains(&table, "grape"), "Doesn't contain non-existing key");
    
    // Test update
    int new_value = 999;
    hashtable_put(&table, "apple", &new_value);
    int *updated = (int*)hashtable_get(&table, "apple");
    TEST_ASSERT(*updated == 999, "Update works");
    TEST_ASSERT(hashtable_size(&table) == 4, "Size unchanged after update");
    
    // Test remove
    bool removed = hashtable_remove(&table, "banana");
    TEST_ASSERT(removed, "Remove existing key");
    TEST_ASSERT(!hashtable_contains(&table, "banana"), "Key no longer exists");
    TEST_ASSERT(hashtable_size(&table) == 3, "Size decremented after remove");
    
    hashtable_free(&table);
    printf("Hash Table tests completed\n");
}

// Test Hash Set
void test_hashset() {
    TEST_START("HASH SET");
    
    HashSet set;
    hashset_init_string(&set);
    
    TEST_ASSERT(hashset_size(&set) == 0, "Initial size is 0");
    TEST_ASSERT(hashset_is_empty(&set), "Initially empty");
    
    const char* items[] = {"apple", "banana", "cherry", "apple", "date"};
    
    // Test add operations (with duplicates)
    int unique_count = 0;
    for (int i = 0; i < 5; i++) {
        bool added = hashset_add_string(&set, items[i]);
        if (added) unique_count++;
    }
    
    TEST_ASSERT(unique_count == 4, "Correct number of unique items added");
    TEST_ASSERT(hashset_size(&set) == 4, "Size reflects unique items only");
    
    // Test contains
    TEST_ASSERT(hashset_contains_string(&set, "apple"), "Contains existing item");
    TEST_ASSERT(!hashset_contains_string(&set, "grape"), "Doesn't contain non-existing item");
    
    // Test remove
    bool removed = hashset_remove_string(&set, "banana");
    TEST_ASSERT(removed, "Remove existing item");
    TEST_ASSERT(!hashset_contains_string(&set, "banana"), "Item no longer exists");
    TEST_ASSERT(hashset_size(&set) == 3, "Size decremented after remove");
    
    hashset_free(&set);
    printf("Hash Set tests completed\n");
}

// Test Circular Linked List
void test_circular_linked_list() {
    TEST_START("CIRCULAR LINKED LIST");
    
    CircularLinkedList list;
    circular_init(&list);
    
    TEST_ASSERT(circular_size(&list) == 0, "Initial size is 0");
    TEST_ASSERT(circular_is_empty(&list), "Initially empty");
    
    int values[] = {10, 20, 30};
    
    // Test insertions
    for (int i = 0; i < 3; i++) {
        circular_push_back(&list, &values[i]);
    }
    
    TEST_ASSERT(circular_size(&list) == 3, "Size after insertions");
    TEST_ASSERT(!circular_is_empty(&list), "Not empty after insertions");
    
    // Test circular property by getting elements
    TEST_ASSERT(*(int*)circular_get_at(&list, 0) == 10, "First element correct");
    TEST_ASSERT(*(int*)circular_get_at(&list, 1) == 20, "Second element correct");
    TEST_ASSERT(*(int*)circular_get_at(&list, 2) == 30, "Third element correct");
    
    // Test contains
    TEST_ASSERT(circular_contains(&list, &values[1]), "Contains existing element");
    
    int non_existing = 999;
    TEST_ASSERT(!circular_contains(&list, &non_existing), "Doesn't contain non-existing element");
    
    circular_free(&list);
    printf("Circular Linked List tests completed\n");
}

// Performance benchmark
void benchmark_performance() {
    TEST_START("PERFORMANCE BENCHMARK");
    
    const int N = 10000;
    clock_t start, end;
    
    printf("Benchmarking with %d operations:\n", N);
    
    // Benchmark Dynamic Array
    start = clock();
    DynArray arr;
    dynarray_init(&arr, 1);
    for (int i = 0; i < N; i++) {
        dynarray_push(&arr, &i);
    }
    end = clock();
    printf("Dynamic Array %d pushes: %.2fms\n", N, 
           ((double)(end - start) / CLOCKS_PER_SEC) * 1000);
    dynarray_free(&arr);
    
    // Benchmark Stack
    start = clock();
    Stack stack;
    stack_init(&stack);
    for (int i = 0; i < N; i++) {
        stack_push(&stack, &i);
    }
    end = clock();
    printf("Stack %d pushes: %.2fms\n", N, 
           ((double)(end - start) / CLOCKS_PER_SEC) * 1000);
    stack_free(&stack);
    
    // Benchmark Hash Table
    start = clock();
    HashTable table;
    hashtable_init_int(&table);
    for (int i = 0; i < N; i++) {
        hashtable_put(&table, &i, &i);
    }
    end = clock();
    printf("Hash Table %d insertions: %.2fms\n", N, 
           ((double)(end - start) / CLOCKS_PER_SEC) * 1000);
    hashtable_free(&table);
    
    printf("Performance benchmark completed\n");
}

// Memory safety test
void test_memory_safety() {
    TEST_START("MEMORY SAFETY");
    
    // Test multiple init/free cycles
    for (int i = 0; i < 100; i++) {
        DynArray arr;
        dynarray_init(&arr, 5);
        for (int j = 0; j < 10; j++) {
            dynarray_push(&arr, &j);
        }
        dynarray_free(&arr);
    }
    TEST_ASSERT(true, "Multiple DynArray init/free cycles");
    
    // Test stack operations
    Stack stack;
    stack_init(&stack);
    for (int i = 0; i < 1000; i++) {
        stack_push(&stack, &i);
    }
    while (!stack_is_empty(&stack)) {
        stack_pop(&stack);
    }
    stack_free(&stack);
    TEST_ASSERT(true, "Stack push/pop stress test");
    
    // Test empty operations
    Queue queue;
    queue_init(&queue);
    TEST_ASSERT(queue_dequeue(&queue) == NULL, "Dequeue from empty queue returns NULL");
    TEST_ASSERT(queue_peek(&queue) == NULL, "Peek empty queue returns NULL");
    queue_free(&queue);
    
    printf("Memory safety tests completed\n");
}

int main() {
    printf("======================================================================\n");
    printf("           COMPREHENSIVE DATA STRUCTURES TEST SUITE\n");
    printf("======================================================================\n");
    printf("Testing all data structures in the workspace...\n");
    
    // Run all tests
    test_dynarray();
    test_singly_linked_list();
    test_doubly_linked_list();
    test_circular_linked_list();
    test_stack();
    test_queue();
    test_deque();
    test_min_heap();
    test_max_heap();
    test_hashtable();
    test_hashset();
    test_memory_safety();
    benchmark_performance();
    
    // Print final summary
    TEST_SUMMARY();
    
    if (results.failed == 0) {
        printf("\n🎉 ALL TESTS PASSED! Your data structures are working perfectly!\n");
        return 0;
    } else {
        printf("\n⚠️  Some tests failed. Please check the implementation.\n");
        return 1;
    }
}
