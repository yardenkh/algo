#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H

#include "heap_interface.h"
#include "../dynarray/dynarray.h"
#include <stdio.h>
#include <stdbool.h>

/**
 * BINARY HEAP IMPLEMENTATION
 *
 * Complete binary tree stored in dynamic array with heap property:
 * - Min-Heap: parent <= children
 * - Max-Heap: parent >= children
 *
 * Time Complexities:
 * - Insert: O(log n)
 * - Extract root: O(log n)
 * - Peek: O(1)
 * - Build from array: O(n)
 *
 * Space Complexity: O(n)
 *
 * Array representation:
 * - Root at index 0
 * - Parent of i: (i-1)/2
 * - Left child of i: 2*i + 1
 * - Right child of i: 2*i + 2
 */

// Binary heap structure
typedef struct BinaryHeap {
    DynArray data;                       // Underlying storage
    heap_compare_fn compare;             // Comparison function
    const ExtendedHeapInterface *vtable; // Interface for polymorphism
} BinaryHeap;

// ==================== HELPER FUNCTIONS ====================

/**
 * Swap two elements in heap
 * @param heap: Target heap
 * @param i, j: Indices to swap
 */
static inline void binary_heap_swap(BinaryHeap *heap, size_t i, size_t j) {
    void *temp = dynarray_get(&heap->data, i);
    dynarray_set(&heap->data, i, dynarray_get(&heap->data, j));
    dynarray_set(&heap->data, j, temp);
}

/**
 * Restore heap property by moving element up
 * @param heap: Target heap
 * @param index: Starting index
 */
static inline void binary_heap_heapify_up(BinaryHeap *heap, size_t index) {
    while (index > 0) {
        size_t parent_idx = HEAP_PARENT(index);
        void *current = dynarray_get(&heap->data, index);
        void *parent = dynarray_get(&heap->data, parent_idx);
        
        if (heap->compare(current, parent) >= 0) break;
        
        binary_heap_swap(heap, index, parent_idx);
        index = parent_idx;
    }
}

/**
 * Restore heap property by moving element down
 * @param heap: Target heap
 * @param index: Starting index
 */
static inline void binary_heap_heapify_down(BinaryHeap *heap, size_t index) {
    size_t size = dynarray_size(&heap->data);
    
    while (true) {
        size_t target = index;
        size_t left = HEAP_LEFT_CHILD(index);
        size_t right = HEAP_RIGHT_CHILD(index);
        
        // Find smallest/largest among parent and children
        if (left < size) {
            void *current = dynarray_get(&heap->data, target);
            void *left_data = dynarray_get(&heap->data, left);
            if (heap->compare(left_data, current) < 0) {
                target = left;
            }
        }
        
        if (right < size) {
            void *target_data = dynarray_get(&heap->data, target);
            void *right_data = dynarray_get(&heap->data, right);
            if (heap->compare(right_data, target_data) < 0) {
                target = right;
            }
        }
        
        if (target == index) break; // Heap property satisfied
        
        binary_heap_swap(heap, index, target);
        index = target;
    }
}

// ==================== CORE OPERATIONS ====================

/**
 * Initialize binary heap
 * @param heap: Heap to initialize
 * @param compare: Comparison function for ordering
 * @param initial_capacity: Starting capacity
 */
static inline void binary_heap_init(BinaryHeap *heap, heap_compare_fn compare, size_t initial_capacity) {
    size_t capacity = initial_capacity > 0 ? initial_capacity : HEAP_DEFAULT_CAPACITY;
    dynarray_init(&heap->data, capacity);
    heap->compare = compare;
    heap->vtable = NULL;
}

/**
 * Insert element into heap
 * @param heap: Target heap
 * @param element: Element to insert
 */
static inline void binary_heap_push(BinaryHeap *heap, void *element) {
    dynarray_push(&heap->data, element);
    size_t last_index = dynarray_size(&heap->data) - 1;
    binary_heap_heapify_up(heap, last_index);
}

/**
 * Extract root element
 * @param heap: Target heap
 * @return: Root element, NULL if empty
 */
static inline void *binary_heap_pop(BinaryHeap *heap) {
    size_t size = dynarray_size(&heap->data);
    if (size == 0) return NULL;
    
    void *root = dynarray_get(&heap->data, 0);
    
    if (size == 1) {
        dynarray_pop(&heap->data);
        return root;
    }
    
    // Move last element to root and restore heap property
    void *last = dynarray_pop(&heap->data);
    dynarray_set(&heap->data, 0, last);
    binary_heap_heapify_down(heap, 0);
    
    return root;
}

/**
 * View root element without removing
 * @param heap: Target heap
 * @return: Root element, NULL if empty
 */
static inline void *binary_heap_peek(BinaryHeap *heap) {
    return dynarray_size(&heap->data) > 0 ? dynarray_get(&heap->data, 0) : NULL;
}

/**
 * Replace root with new element
 * @param heap: Target heap
 * @param element: New root element
 * @return: Old root element
 */
static inline void *binary_heap_replace(BinaryHeap *heap, void *element) {
    size_t size = dynarray_size(&heap->data);
    if (size == 0) {
        binary_heap_push(heap, element);
        return NULL;
    }
    
    void *old_root = dynarray_get(&heap->data, 0);
    dynarray_set(&heap->data, 0, element);
    binary_heap_heapify_down(heap, 0);
    return old_root;
}

/**
 * Build heap from array in O(n) time
 * @param heap: Target heap
 * @param elements: Array of elements
 * @param count: Number of elements
 */
static inline void binary_heap_build_from_array(BinaryHeap *heap, void **elements, size_t count) {
    dynarray_clear(&heap->data);
    dynarray_reserve(&heap->data, count);
    
    // Copy all elements
    for (size_t i = 0; i < count; i++) {
        dynarray_push(&heap->data, elements[i]);
    }
    
    // Heapify from last parent down to root
    if (count > 1) {
        size_t last_parent = HEAP_PARENT(count - 1);
        for (size_t i = last_parent + 1; i > 0; i--) {
            binary_heap_heapify_down(heap, i - 1);
        }
    }
}

/**
 * Get current size
 * @param heap: Target heap
 * @return: Number of elements
 */
static inline size_t binary_heap_size(BinaryHeap *heap) {
    return dynarray_size(&heap->data);
}

/**
 * Check if empty
 * @param heap: Target heap
 * @return: true if empty
 */
static inline bool binary_heap_is_empty(BinaryHeap *heap) {
    return dynarray_is_empty(&heap->data);
}

/**
 * Clear all elements
 * @param heap: Target heap
 */
static inline void binary_heap_clear(BinaryHeap *heap) {
    dynarray_clear(&heap->data);
}

/**
 * Free heap memory
 * @param heap: Target heap
 */
static inline void binary_heap_free(BinaryHeap *heap) {
    dynarray_free(&heap->data);
    heap->compare = NULL;
    heap->vtable = NULL;
}

// ==================== VALIDATION AND UTILITIES ====================

/**
 * Validate heap property
 * @param heap: Target heap
 * @return: true if valid heap
 */
static inline bool binary_heap_is_valid(BinaryHeap *heap) {
    size_t size = dynarray_size(&heap->data);
    
    // Check heap property for each parent
    for (size_t i = 0; i < size / 2; i++) {
        size_t left = HEAP_LEFT_CHILD(i);
        size_t right = HEAP_RIGHT_CHILD(i);
        
        void *parent_data = dynarray_get(&heap->data, i);
        
        if (left < size) {
            void *left_data = dynarray_get(&heap->data, left);
            if (heap->compare(parent_data, left_data) > 0) {
                return false;
            }
        }
        
        if (right < size) {
            void *right_data = dynarray_get(&heap->data, right);
            if (heap->compare(parent_data, right_data) > 0) {
                return false;
            }
        }
    }
    
    return true;
}

/**
 * Merge two heaps (inefficient for binary heaps)
 * @param heap1: Destination heap
 * @param heap2: Source heap (will be emptied)
 */
static inline void binary_heap_merge(BinaryHeap *heap1, BinaryHeap *heap2) {
    while (!binary_heap_is_empty(heap2)) {
        void *element = binary_heap_pop(heap2);
        binary_heap_push(heap1, element);
    }
}

/**
 * Print heap structure for debugging
 * @param heap: Target heap
 * @param print_element: Function to print elements
 */
static inline void binary_heap_print(BinaryHeap *heap, void (*print_element)(void *)) {
    size_t size = dynarray_size(&heap->data);
    printf("Binary Heap - Size: %zu\n", size);
    
    if (size == 0) {
        printf("  (empty)\n");
        return;
    }
    
    // Print array representation
    printf("  Array: [");
    for (size_t i = 0; i < size; i++) {
        if (i > 0) printf(", ");
        if (print_element) {
            print_element(dynarray_get(&heap->data, i));
        } else {
            printf("%p", dynarray_get(&heap->data, i));
        }
    }
    printf("]\n");
    
    // Print tree levels
    size_t level = 0;
    size_t level_start = 0;
    size_t level_size = 1;
    
    while (level_start < size) {
        printf("  Level %zu: ", level);
        for (size_t i = level_start; i < level_start + level_size && i < size; i++) {
            if (print_element) {
                print_element(dynarray_get(&heap->data, i));
            } else {
                printf("%p", dynarray_get(&heap->data, i));
            }
            printf(" ");
        }
        printf("\n");
        
        level++;
        level_start += level_size;
        level_size *= 2;
    }
}

// ==================== INTERFACE IMPLEMENTATION ====================

static void binary_heap_interface_push(void *heap, void *element) {
    binary_heap_push((BinaryHeap *)heap, element);
}

static void *binary_heap_interface_pop(void *heap) {
    return binary_heap_pop((BinaryHeap *)heap);
}

static void *binary_heap_interface_peek(void *heap) {
    return binary_heap_peek((BinaryHeap *)heap);
}

static size_t binary_heap_interface_size(void *heap) {
    return binary_heap_size((BinaryHeap *)heap);
}

static bool binary_heap_interface_is_empty(void *heap) {
    return binary_heap_is_empty((BinaryHeap *)heap);
}

static void binary_heap_interface_clear(void *heap) {
    binary_heap_clear((BinaryHeap *)heap);
}

static void binary_heap_interface_free_heap(void *heap) {
    binary_heap_free((BinaryHeap *)heap);
}

static bool binary_heap_interface_is_valid(void *heap) {
    return binary_heap_is_valid((BinaryHeap *)heap);
}

static void *binary_heap_interface_replace(void *heap, void *element) {
    return binary_heap_replace((BinaryHeap *)heap, element);
}

static void binary_heap_interface_build_from_array(void *heap, void **elements, size_t count) {
    binary_heap_build_from_array((BinaryHeap *)heap, elements, count);
}

static void binary_heap_interface_merge(void *heap1, void *heap2) {
    binary_heap_merge((BinaryHeap *)heap1, (BinaryHeap *)heap2);
}

static const ExtendedHeapInterface binary_heap_vtable = {
    .base = {
        .push = binary_heap_interface_push,
        .pop = binary_heap_interface_pop,
        .peek = binary_heap_interface_peek,
        .size = binary_heap_interface_size,
        .is_empty = binary_heap_interface_is_empty,
        .clear = binary_heap_interface_clear,
        .free_heap = binary_heap_interface_free_heap,
        .is_valid = binary_heap_interface_is_valid
    },
    .replace = binary_heap_interface_replace,
    .build_from_array = binary_heap_interface_build_from_array,
    .merge = binary_heap_interface_merge
};

/**
 * Initialize heap with interface support
 * @param heap: Heap to initialize
 * @param compare: Comparison function
 * @param initial_capacity: Starting capacity
 */
static inline void binary_heap_init_with_interface(BinaryHeap *heap, heap_compare_fn compare, size_t initial_capacity) {
    binary_heap_init(heap, compare, initial_capacity);
    heap->vtable = &binary_heap_vtable;
}

#endif // BINARY_HEAP_H