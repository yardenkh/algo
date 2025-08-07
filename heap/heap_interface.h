#ifndef HEAP_INTERFACE_H
#define HEAP_INTERFACE_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/**
 * HEAP INTERFACE
 *
 * Common interface for all heap implementations following SOLID principles.
 * Provides different interface levels for different heap capabilities.
 */

// Forward declarations
typedef struct BinaryHeap BinaryHeap;

// Comparison function type for heap ordering
// Returns: negative if a < b, 0 if a == b, positive if a > b
typedef int (*heap_compare_fn)(const void *a, const void *b);

// Basic heap interface - operations all heaps must support
typedef struct HeapInterface
{
    void (*push)(void *heap, void *element); // Insert element
    void *(*pop)(void *heap);                // Extract root
    void *(*peek)(void *heap);               // View root
    size_t (*size)(void *heap);              // Get element count
    bool (*is_empty)(void *heap);            // Check if empty
    void (*clear)(void *heap);               // Remove all elements
    void (*free_heap)(void *heap);           // Deallocate
    bool (*is_valid)(void *heap);            // Validate heap property
} HeapInterface;

// Extended interface for heaps with additional operations
typedef struct ExtendedHeapInterface
{
    HeapInterface base; // Inherit basic operations

    void *(*replace)(void *heap, void *element);                         // Replace root element
    void (*build_from_array)(void *heap, void **elements, size_t count); // Build heap from array
    void (*merge)(void *heap1, void *heap2);                             // Merge two heaps
} ExtendedHeapInterface;

// ==================== DEFAULT COMPARISON FUNCTIONS ====================

/**
 * Integer comparison for min heap (ascending order)
 * @param a, b: Pointers to integers to compare
 * @return: Negative if a < b, 0 if equal, positive if a > b
 */
static inline int heap_int_compare_min(const void *a, const void *b)
{
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    return ia - ib;
}

/**
 * Integer comparison for max heap (descending order)
 * @param a, b: Pointers to integers to compare
 * @return: Negative if a > b (reversed for max-heap)
 */
static inline int heap_int_compare_max(const void *a, const void *b)
{
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    return ib - ia; // Reversed order
}

/**
 * String comparison for heaps
 * @param a, b: Pointers to string pointers
 * @return: strcmp result for lexicographic ordering
 */
static inline int heap_string_compare(const void *a, const void *b)
{
    const char *sa = *(const char **)a;
    const char *sb = *(const char **)b;
    return strcmp(sa, sb);
}

/**
 * Generic pointer comparison (address-based ordering)
 * @param a, b: Pointers to compare
 * @return: Comparison based on pointer addresses
 */
static inline int heap_pointer_compare(const void *a, const void *b)
{
    uintptr_t pa = (uintptr_t)a;
    uintptr_t pb = (uintptr_t)b;
    return (pa > pb) - (pa < pb);
}

// ==================== HEAP NAVIGATION MACROS ====================

// Binary heap array representation navigation
#define HEAP_PARENT(i) (((i) - 1) / 2)
#define HEAP_LEFT_CHILD(i) (2 * (i) + 1)
#define HEAP_RIGHT_CHILD(i) (2 * (i) + 2)

// D-ary heap navigation
#define DARY_PARENT(i, d) (((i) - 1) / (d))
#define DARY_FIRST_CHILD(i, d) ((d) * (i) + 1)
#define DARY_KTH_CHILD(i, d, k) ((d) * (i) + (k) + 1)

// ==================== HEAP CONFIGURATION ====================

// Default heap parameters
#define HEAP_DEFAULT_CAPACITY 16
#define HEAP_GROWTH_FACTOR 2
#define HEAP_MIN_CAPACITY 8

#endif // HEAP_INTERFACE_H