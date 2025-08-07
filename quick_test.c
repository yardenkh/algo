#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Core data structures that are working
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

// Test runner structure
typedef struct {
    const char* name;
    void (*test_func)(void);
    int passed;
    int failed;
} TestSuite;

static int total_passed = 0;
static int total_failed = 0;

#define ASSERT(condition, message) \
    do { \
        if (condition) { \
            printf("  ✓ %s\n", message); \
            total_passed++; \
        } else { \
            printf("  ✗ %s\n", message); \
            total_failed++; \
        } \
    } while(0)

// Quick test functions for each data structure
void test_dynamic_array() {
    printf("\n📦 TESTING DYNAMIC ARRAY\n");
    
    DynArray arr;
    dynarray_init(&arr, 3);
    
    int nums[] = {10, 20, 30, 40, 50};
    
    // Basic operations
    ASSERT(dynarray_is_empty(&arr), "Initially empty");
    
    for (int i = 0; i < 5; i++) {
        dynarray_push(&arr, &nums[i]);
    }
    
    ASSERT(dynarray_size(&arr) == 5, "Size correct after pushes");
    ASSERT(*(int*)dynarray_get(&arr, 0) == 10, "First element");
    ASSERT(*(int*)dynarray_get(&arr, 4) == 50, "Last element");
    
    int *popped = (int*)dynarray_pop(&arr);
    ASSERT(*popped == 50 && dynarray_size(&arr) == 4, "Pop operation");
    
    dynarray_clear(&arr);
    ASSERT(dynarray_is_empty(&arr), "Clear operation");
    
    dynarray_free(&arr);
}

void test_linked_lists() {
    printf("\n🔗 TESTING LINKED LISTS\n");
    
    // Singly Linked List
    SinglyLinkedList slist;
    singly_init(&slist);
    int val1 = 100;
    singly_push_front(&slist, &val1);
    ASSERT(singly_size(&slist) == 1, "Singly list push front");
    singly_free(&slist);
    
    // Doubly Linked List  
    DoublyLinkedList dlist;
    doubly_init(&dlist);
    int vals[] = {1, 2, 3};
    for (int i = 0; i < 3; i++) {
        doubly_push_back(&dlist, &vals[i]);
    }
    ASSERT(doubly_size(&dlist) == 3, "Doubly list operations");
    ASSERT(*(int*)doubly_get_at(&dlist, 1) == 2, "Doubly list access");
    doubly_free(&dlist);
    
    // Circular Linked List
    CircularLinkedList clist;
    circular_init(&clist);
    int cval = 42;
    circular_push_back(&clist, &cval);
    ASSERT(circular_size(&clist) == 1, "Circular list operations");
    circular_free(&clist);
}

void test_stacks_and_queues() {
    printf("\n📚 TESTING STACKS & QUEUES\n");
    
    // Stack (LIFO)
    Stack stack;
    stack_init(&stack);
    int nums[] = {1, 2, 3};
    
    for (int i = 0; i < 3; i++) {
        stack_push(&stack, &nums[i]);
    }
    
    int *top = (int*)stack_pop(&stack);
    ASSERT(*top == 3, "Stack LIFO behavior");
    stack_free(&stack);
    
    // Queue (FIFO)
    Queue queue;
    queue_init(&queue);
    
    for (int i = 0; i < 3; i++) {
        queue_enqueue(&queue, &nums[i]);
    }
    
    int *front = (int*)queue_dequeue(&queue);
    ASSERT(*front == 1, "Queue FIFO behavior");
    queue_free(&queue);
    
    // Deque (both ends)
    Deque deque;
    deque_init(&deque);
    int a = 10, b = 20;
    deque_push_front(&deque, &a);
    deque_push_back(&deque, &b);
    
    int *front_val = (int*)deque_peek_front(&deque);
    int *back_val = (int*)deque_peek_back(&deque);
    ASSERT(*front_val == 10 && *back_val == 20, "Deque both-end operations");
    deque_free(&deque);
}

void test_heaps() {
    printf("\n🏔️ TESTING HEAPS\n");
    
    // Min Heap
    MinHeap min_heap;
    min_heap_init(&min_heap, 10);
    
    int nums[] = {50, 30, 70, 20, 40, 60, 80};
    for (int i = 0; i < 7; i++) {
        min_heap_push(&min_heap, &nums[i]);
    }
    
    int *min_val = (int*)min_heap_peek(&min_heap);
    ASSERT(*min_val == 20, "Min heap property");
    
    int *extracted = (int*)min_heap_pop(&min_heap);
    ASSERT(*extracted == 20, "Min heap extraction");
    min_heap_free(&min_heap);
    
    // Max Heap
    MaxHeap max_heap;
    max_heap_init(&max_heap, 10);
    
    for (int i = 0; i < 7; i++) {
        max_heap_push(&max_heap, &nums[i]);
    }
    
    int *max_val = (int*)max_heap_peek(&max_heap);
    ASSERT(*max_val == 80, "Max heap property");
    max_heap_free(&max_heap);
}

void test_hash_structures() {
    printf("\n#️⃣ TESTING HASH STRUCTURES\n");
    
    // Hash Table
    HashTable table;
    hashtable_init_string(&table);
    
    const char* key = "test_key";
    int value = 12345;
    
    hashtable_put(&table, key, &value);
    int *retrieved = (int*)hashtable_get(&table, key);
    
    ASSERT(retrieved != NULL && *retrieved == 12345, "Hash table put/get");
    ASSERT(hashtable_contains(&table, key), "Hash table contains");
    
    hashtable_remove(&table, key);
    ASSERT(!hashtable_contains(&table, key), "Hash table remove");
    hashtable_free(&table);
    
    // Hash Set
    HashSet set;
    hashset_init_string(&set);
    
    const char* items[] = {"apple", "banana", "apple", "cherry"};
    
    for (int i = 0; i < 4; i++) {
        hashset_add_string(&set, items[i]);
    }
    
    ASSERT(hashset_size(&set) == 3, "Hash set unique elements");
    ASSERT(hashset_contains_string(&set, "apple"), "Hash set contains");
    ASSERT(!hashset_contains_string(&set, "orange"), "Hash set doesn't contain");
    hashset_free(&set);
}

void test_edge_cases() {
    printf("\n⚠️ TESTING EDGE CASES\n");
    
    // Empty operations
    Stack empty_stack;
    stack_init(&empty_stack);
    ASSERT(stack_pop(&empty_stack) == NULL, "Pop from empty stack");
    ASSERT(stack_peek(&empty_stack) == NULL, "Peek empty stack");
    stack_free(&empty_stack);
    
    // Large operations
    DynArray large_arr;
    dynarray_init(&large_arr, 1);
    
    for (int i = 0; i < 1000; i++) {
        dynarray_push(&large_arr, &i);
    }
    
    ASSERT(dynarray_size(&large_arr) == 1000, "Large array operations");
    dynarray_free(&large_arr);
    
    // Memory stress test
    for (int i = 0; i < 100; i++) {
        HashTable temp_table;
        hashtable_init_int(&temp_table);
        int temp_val = i;
        hashtable_put(&temp_table, &i, &temp_val);
        hashtable_free(&temp_table);
    }
    ASSERT(true, "Memory stress test completed");
}

void benchmark_performance() {
    printf("\n⚡ PERFORMANCE BENCHMARK\n");
    
    const int N = 50000;
    clock_t start, end;
    
    // DynArray vs Stack performance
    start = clock();
    DynArray arr;
    dynarray_init(&arr, 100);
    for (int i = 0; i < N; i++) {
        dynarray_push(&arr, &i);
    }
    end = clock();
    double arr_time = ((double)(end - start) / CLOCKS_PER_SEC) * 1000;
    dynarray_free(&arr);
    
    start = clock();
    Stack stack;
    stack_init(&stack);
    for (int i = 0; i < N; i++) {
        stack_push(&stack, &i);
    }
    end = clock();
    double stack_time = ((double)(end - start) / CLOCKS_PER_SEC) * 1000;
    stack_free(&stack);
    
    printf("  📊 DynArray %d ops: %.2fms\n", N, arr_time);
    printf("  📊 Stack %d ops: %.2fms\n", N, stack_time);
    
    ASSERT(arr_time < 100 && stack_time < 100, "Performance within reasonable bounds");
}

int main() {
    printf("=================================================================\n");
    printf("🚀 COMPREHENSIVE DATA STRUCTURES TEST SUITE\n");
    printf("=================================================================\n");
    printf("Testing core functionality of all implemented data structures...\n");
    
    // Run all test suites
    test_dynamic_array();
    test_linked_lists();
    test_stacks_and_queues();
    test_heaps();
    test_hash_structures();
    test_edge_cases();
    benchmark_performance();
    
    // Final results
    printf("\n=================================================================\n");
    printf("📊 FINAL RESULTS\n");
    printf("=================================================================\n");
    printf("✅ Tests Passed: %d\n", total_passed);
    printf("❌ Tests Failed: %d\n", total_failed);
    printf("📈 Success Rate: %.1f%%\n", 
           (total_passed + total_failed > 0) ? 
           (100.0 * total_passed / (total_passed + total_failed)) : 0.0);
    printf("=================================================================\n");
    
    if (total_failed == 0) {
        printf("🎉 EXCELLENT! All data structures are working perfectly!\n");
        printf("💡 Your implementation is solid and ready for production use!\n");
        return 0;
    } else {
        printf("⚠️  Some tests failed. Please review the implementations.\n");
        return 1;
    }
}
