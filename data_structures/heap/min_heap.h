#ifndef MIN_HEAP_H
#define MIN_HEAP_H

#include "binary_heap.h"
#include <stdio.h>

/**
 * MIN-HEAP IMPLEMENTATION
 *
 * Specialized binary heap where the smallest element is always at root.
 * Root contains minimum, parent <= children property maintained.
 *
 * Use Cases:
 * - Priority queues (lowest priority first)
 * - Dijkstra's shortest path algorithm
 * - Huffman coding
 * - Event scheduling (earliest time first)
 *
 * Time Complexities: Same as binary heap - O(log n) insert/extract, O(1) peek
 */

// Min heap is just a binary heap with min comparison - no separate structure needed
typedef BinaryHeap MinHeap;

// ==================== MIN HEAP OPERATIONS ====================

/**
 * Initialize min heap for integers
 * @param heap: Heap to initialize
 * @param initial_capacity: Starting capacity
 */
static inline void min_heap_init(MinHeap *heap, size_t initial_capacity)
{
    binary_heap_init(heap, heap_int_compare_min, initial_capacity);
}

/**
 * Initialize min heap with custom comparison
 * @param heap: Heap to initialize
 * @param compare: Comparison function (should implement min-heap ordering)
 * @param initial_capacity: Starting capacity
 */
static inline void min_heap_init_with_compare(MinHeap *heap, heap_compare_fn compare, size_t initial_capacity)
{
    binary_heap_init(heap, compare, initial_capacity);
}

/**
 * Insert element into min heap
 * @param heap: Target heap
 * @param element: Element to insert
 */
static inline void min_heap_push(MinHeap *heap, void *element)
{
    binary_heap_push(heap, element);
}

/**
 * Extract minimum element
 * @param heap: Target heap
 * @return: Minimum element, NULL if empty
 */
static inline void *min_heap_pop(MinHeap *heap)
{
    return binary_heap_pop(heap);
}

/**
 * View minimum element without removing
 * @param heap: Target heap
 * @return: Minimum element, NULL if empty
 */
static inline void *min_heap_peek(MinHeap *heap)
{
    return binary_heap_peek(heap);
}

/**
 * Replace minimum with new element
 * @param heap: Target heap
 * @param new_element: New element (should be <= current min for heap property)
 * @return: Old minimum element
 */
static inline void *min_heap_replace_min(MinHeap *heap, void *new_element)
{
    return binary_heap_replace(heap, new_element);
}

/**
 * Build min heap from array in O(n) time
 * @param heap: Target heap
 * @param elements: Array of elements
 * @param count: Number of elements
 */
static inline void min_heap_build_from_array(MinHeap *heap, void **elements, size_t count)
{
    binary_heap_build_from_array(heap, elements, count);
}

/**
 * Merge two min heaps
 * @param heap1: Destination heap
 * @param heap2: Source heap (will be emptied)
 */
static inline void min_heap_merge(MinHeap *heap1, MinHeap *heap2)
{
    binary_heap_merge(heap1, heap2);
}

/**
 * Get current size
 * @param heap: Target heap
 * @return: Number of elements
 */
static inline size_t min_heap_size(MinHeap *heap)
{
    return binary_heap_size(heap);
}

/**
 * Check if empty
 * @param heap: Target heap
 * @return: true if empty
 */
static inline bool min_heap_is_empty(MinHeap *heap)
{
    return binary_heap_is_empty(heap);
}

/**
 * Clear all elements
 * @param heap: Target heap
 */
static inline void min_heap_clear(MinHeap *heap)
{
    binary_heap_clear(heap);
}

/**
 * Validate min heap property
 * @param heap: Target heap
 * @return: true if valid min heap
 */
static inline bool min_heap_is_valid(MinHeap *heap)
{
    return binary_heap_is_valid(heap);
}

/**
 * Print min heap structure
 * @param heap: Target heap
 * @param print_element: Function to print elements
 */
static inline void min_heap_print(MinHeap *heap, void (*print_element)(void *))
{
    printf("Min-");
    binary_heap_print(heap, print_element);
}

/**
 * Free heap memory
 * @param heap: Target heap
 */
static inline void min_heap_free(MinHeap *heap)
{
    binary_heap_free(heap);
}

// ==================== CONVENIENCE FUNCTIONS ====================

/**
 * Create min heap from integer array
 * @param values: Array of integer pointers
 * @param count: Number of values
 * @param initial_capacity: Starting capacity
 * @return: Initialized min heap (caller must free)
 */
static inline MinHeap *min_heap_create_from_ints(int **values, size_t count, size_t initial_capacity)
{
    MinHeap *heap = (MinHeap *)malloc(sizeof(MinHeap));
    if (!heap)
        return NULL;

    size_t capacity = initial_capacity > count ? initial_capacity : count;
    min_heap_init(heap, capacity);
    min_heap_build_from_array(heap, (void **)values, count);
    return heap;
}

/**
 * Insert integer value
 * @param heap: Target heap
 * @param value: Integer value to insert
 */
static inline void min_heap_push_int(MinHeap *heap, int *value)
{
    min_heap_push(heap, (void *)value);
}

/**
 * Extract minimum integer
 * @param heap: Target heap
 * @return: Pointer to minimum integer, NULL if empty
 */
static inline int *min_heap_pop_int(MinHeap *heap)
{
    return (int *)min_heap_pop(heap);
}

/**
 * Peek minimum integer
 * @param heap: Target heap
 * @return: Pointer to minimum integer, NULL if empty
 */
static inline int *min_heap_peek_int(MinHeap *heap)
{
    return (int *)min_heap_peek(heap);
}

// ==================== INTERFACE IMPLEMENTATION ====================

// Use binary heap interface functions - they work for min heap too
static const HeapInterface min_heap_vtable = {
    .push = binary_heap_interface_push,
    .pop = binary_heap_interface_pop,
    .peek = binary_heap_interface_peek,
    .size = binary_heap_interface_size,
    .is_empty = binary_heap_interface_is_empty,
    .clear = binary_heap_interface_clear,
    .free_heap = binary_heap_interface_free_heap,
    .is_valid = binary_heap_interface_is_valid};

/**
 * Initialize min heap with interface support
 * @param heap: Heap to initialize
 * @param initial_capacity: Starting capacity
 */
static inline void min_heap_init_with_interface(MinHeap *heap, size_t initial_capacity)
{
    min_heap_init(heap, initial_capacity);
    heap->vtable = (const ExtendedHeapInterface *)&min_heap_vtable;
}

// ==================== CONVENIENCE MACROS ====================

#define MIN_HEAP_PUSH_INT(heap, value)   \
    do                                   \
    {                                    \
        int *_val = malloc(sizeof(int)); \
        *_val = (value);                 \
        min_heap_push((heap), _val);     \
    } while (0)

#define MIN_HEAP_POP_INT(heap) ((int *)min_heap_pop(heap))
#define MIN_HEAP_PEEK_INT(heap) ((int *)min_heap_peek(heap))

#endif // MIN_HEAP_H