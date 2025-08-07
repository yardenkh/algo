#ifndef MAX_HEAP_H
#define MAX_HEAP_H

#include "binary_heap.h"
#include <stdio.h>

/**
 * MAX-HEAP IMPLEMENTATION
 *
 * Specialized binary heap where the largest element is always at root.
 * Root contains maximum, parent >= children property maintained.
 *
 * Use Cases:
 * - Priority queues (highest priority first)
 * - Job scheduling (highest priority tasks first)
 * - Finding k largest elements
 * - Maximum flow algorithms
 *
 * Time Complexities: Same as binary heap - O(log n) insert/extract, O(1) peek
 */

// Max heap is just a binary heap with max comparison - no separate structure needed
typedef BinaryHeap MaxHeap;

// ==================== MAX HEAP OPERATIONS ====================

/**
 * Initialize max heap for integers
 * @param heap: Heap to initialize
 * @param initial_capacity: Starting capacity
 */
static inline void max_heap_init(MaxHeap *heap, size_t initial_capacity)
{
    binary_heap_init(heap, heap_int_compare_max, initial_capacity);
}

/**
 * Initialize max heap with custom comparison
 * @param heap: Heap to initialize
 * @param compare: Comparison function (should implement max-heap ordering)
 * @param initial_capacity: Starting capacity
 */
static inline void max_heap_init_with_compare(MaxHeap *heap, heap_compare_fn compare, size_t initial_capacity)
{
    binary_heap_init(heap, compare, initial_capacity);
}

/**
 * Insert element into max heap
 * @param heap: Target heap
 * @param element: Element to insert
 */
static inline void max_heap_push(MaxHeap *heap, void *element)
{
    binary_heap_push(heap, element);
}

/**
 * Extract maximum element
 * @param heap: Target heap
 * @return: Maximum element, NULL if empty
 */
static inline void *max_heap_pop(MaxHeap *heap)
{
    return binary_heap_pop(heap);
}

/**
 * View maximum element without removing
 * @param heap: Target heap
 * @return: Maximum element, NULL if empty
 */
static inline void *max_heap_peek(MaxHeap *heap)
{
    return binary_heap_peek(heap);
}

/**
 * Replace maximum with new element
 * @param heap: Target heap
 * @param new_element: New element (should be >= current max for heap property)
 * @return: Old maximum element
 */
static inline void *max_heap_replace_max(MaxHeap *heap, void *new_element)
{
    return binary_heap_replace(heap, new_element);
}

/**
 * Build max heap from array in O(n) time
 * @param heap: Target heap
 * @param elements: Array of elements
 * @param count: Number of elements
 */
static inline void max_heap_build_from_array(MaxHeap *heap, void **elements, size_t count)
{
    binary_heap_build_from_array(heap, elements, count);
}

/**
 * Merge two max heaps
 * @param heap1: Destination heap
 * @param heap2: Source heap (will be emptied)
 */
static inline void max_heap_merge(MaxHeap *heap1, MaxHeap *heap2)
{
    binary_heap_merge(heap1, heap2);
}

/**
 * Get current size
 * @param heap: Target heap
 * @return: Number of elements
 */
static inline size_t max_heap_size(MaxHeap *heap)
{
    return binary_heap_size(heap);
}

/**
 * Check if empty
 * @param heap: Target heap
 * @return: true if empty
 */
static inline bool max_heap_is_empty(MaxHeap *heap)
{
    return binary_heap_is_empty(heap);
}

/**
 * Clear all elements
 * @param heap: Target heap
 */
static inline void max_heap_clear(MaxHeap *heap)
{
    binary_heap_clear(heap);
}

/**
 * Validate max heap property
 * @param heap: Target heap
 * @return: true if valid max heap
 */
static inline bool max_heap_is_valid(MaxHeap *heap)
{
    return binary_heap_is_valid(heap);
}

/**
 * Print max heap structure
 * @param heap: Target heap
 * @param print_element: Function to print elements
 */
static inline void max_heap_print(MaxHeap *heap, void (*print_element)(void *))
{
    printf("Max-");
    binary_heap_print(heap, print_element);
}

/**
 * Free heap memory
 * @param heap: Target heap
 */
static inline void max_heap_free(MaxHeap *heap)
{
    binary_heap_free(heap);
}

// ==================== CONVENIENCE FUNCTIONS ====================

/**
 * Create max heap from integer array
 * @param values: Array of integer pointers
 * @param count: Number of values
 * @param initial_capacity: Starting capacity
 * @return: Initialized max heap (caller must free)
 */
static inline MaxHeap *max_heap_create_from_ints(int **values, size_t count, size_t initial_capacity)
{
    MaxHeap *heap = (MaxHeap *)malloc(sizeof(MaxHeap));
    if (!heap)
        return NULL;

    size_t capacity = initial_capacity > count ? initial_capacity : count;
    max_heap_init(heap, capacity);
    max_heap_build_from_array(heap, (void **)values, count);
    return heap;
}

/**
 * Insert integer value
 * @param heap: Target heap
 * @param value: Integer value to insert
 */
static inline void max_heap_push_int(MaxHeap *heap, int *value)
{
    max_heap_push(heap, (void *)value);
}

/**
 * Extract maximum integer
 * @param heap: Target heap
 * @return: Pointer to maximum integer, NULL if empty
 */
static inline int *max_heap_pop_int(MaxHeap *heap)
{
    return (int *)max_heap_pop(heap);
}

/**
 * Peek maximum integer
 * @param heap: Target heap
 * @return: Pointer to maximum integer, NULL if empty
 */
static inline int *max_heap_peek_int(MaxHeap *heap)
{
    return (int *)max_heap_peek(heap);
}

// ==================== INTERFACE IMPLEMENTATION ====================

// Use binary heap interface functions - they work for max heap too
static const HeapInterface max_heap_vtable = {
    .push = binary_heap_interface_push,
    .pop = binary_heap_interface_pop,
    .peek = binary_heap_interface_peek,
    .size = binary_heap_interface_size,
    .is_empty = binary_heap_interface_is_empty,
    .clear = binary_heap_interface_clear,
    .free_heap = binary_heap_interface_free_heap,
    .is_valid = binary_heap_interface_is_valid};

/**
 * Initialize max heap with interface support
 * @param heap: Heap to initialize
 * @param initial_capacity: Starting capacity
 */
static inline void max_heap_init_with_interface(MaxHeap *heap, size_t initial_capacity)
{
    max_heap_init(heap, initial_capacity);
    heap->vtable = (const ExtendedHeapInterface *)&max_heap_vtable;
}

// ==================== CONVENIENCE MACROS ====================

#define MAX_HEAP_PUSH_INT(heap, value)   \
    do                                   \
    {                                    \
        int *_val = malloc(sizeof(int)); \
        *_val = (value);                 \
        max_heap_push((heap), _val);     \
    } while (0)

#define MAX_HEAP_POP_INT(heap) ((int *)max_heap_pop(heap))
#define MAX_HEAP_PEEK_INT(heap) ((int *)max_heap_peek(heap))

#endif // MAX_HEAP_H