#ifndef DYNARRAY_H
#define DYNARRAY_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/**
 * DYNAMIC ARRAY IMPLEMENTATION
 * 
 * A resizable array that grows automatically as needed.
 * Provides O(1) amortized insertion and O(1) access.
 * 
 * Time Complexities:
 * - Access: O(1)
 * - Append: O(1) amortized
 * - Insert: O(n) worst case
 * - Remove: O(n) worst case
 * 
 * Space Complexity: O(n)
 */

// Configuration constants
#define DYNARRAY_DEFAULT_CAPACITY 8
#define DYNARRAY_GROWTH_FACTOR 2
#define DYNARRAY_SHRINK_THRESHOLD 4

// Dynamic array structure
typedef struct DynArray {
    void **data;     // Array of element pointers
    size_t size;     // Number of elements used
    size_t capacity; // Total allocated capacity
} DynArray;

// ==================== CORE OPERATIONS ====================

/**
 * Initialize dynamic array with specified initial capacity
 * @param arr: Array to initialize
 * @param initial_capacity: Starting capacity (0 uses default)
 * Time Complexity: O(1)
 */
static inline void dynarray_init(DynArray *arr, size_t initial_capacity) {
    arr->size = 0;
    arr->capacity = initial_capacity > 0 ? initial_capacity : DYNARRAY_DEFAULT_CAPACITY;
    arr->data = (void **)malloc(arr->capacity * sizeof(void *));
    
    if (!arr->data) {
        fprintf(stderr, "dynarray_init: allocation failed\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * Resize array capacity (internal helper)
 * @param arr: Array to resize
 * @param new_capacity: New capacity
 * Time Complexity: O(n)
 */
static inline void dynarray_resize(DynArray *arr, size_t new_capacity) {
    if (new_capacity < arr->size) return; // Cannot shrink below current size
    
    void **new_data = (void **)realloc(arr->data, new_capacity * sizeof(void *));
    if (!new_data) {
        fprintf(stderr, "dynarray_resize: reallocation failed\n");
        exit(EXIT_FAILURE);
    }
    
    arr->data = new_data;
    arr->capacity = new_capacity;
}

/**
 * Ensure capacity for at least one more element
 * @param arr: Array to check
 * Time Complexity: O(1) amortized
 */
static inline void dynarray_ensure_capacity(DynArray *arr) {
    if (arr->size >= arr->capacity) {
        dynarray_resize(arr, arr->capacity * DYNARRAY_GROWTH_FACTOR);
    }
}

/**
 * Append element to end of array
 * @param arr: Target array
 * @param element: Element to append
 * Time Complexity: O(1) amortized
 */
static inline void dynarray_push(DynArray *arr, void *element) {
    dynarray_ensure_capacity(arr);
    arr->data[arr->size++] = element;
}

/**
 * Remove and return last element
 * @param arr: Target array
 * @return: Last element, NULL if empty
 * Time Complexity: O(1)
 */
static inline void *dynarray_pop(DynArray *arr) {
    if (arr->size == 0) return NULL;
    
    void *element = arr->data[--arr->size];
    
    // Optional: shrink if array becomes much smaller than capacity
    if (arr->size > 0 && arr->size * DYNARRAY_SHRINK_THRESHOLD <= arr->capacity) {
        size_t new_capacity = arr->capacity / DYNARRAY_GROWTH_FACTOR;
        if (new_capacity >= DYNARRAY_DEFAULT_CAPACITY) {
            dynarray_resize(arr, new_capacity);
        }
    }
    
    return element;
}

/**
 * Get element at index
 * @param arr: Target array
 * @param index: Index to access
 * @return: Element at index, NULL if out of bounds
 * Time Complexity: O(1)
 */
static inline void *dynarray_get(DynArray *arr, size_t index) {
    return (index < arr->size) ? arr->data[index] : NULL;
}

/**
 * Set element at index
 * @param arr: Target array
 * @param index: Index to set
 * @param element: New element value
 * @return: true if successful, false if out of bounds
 * Time Complexity: O(1)
 */
static inline bool dynarray_set(DynArray *arr, size_t index, void *element) {
    if (index >= arr->size) return false;
    arr->data[index] = element;
    return true;
}

/**
 * Insert element at specific index
 * @param arr: Target array
 * @param index: Index for insertion
 * @param element: Element to insert
 * @return: true if successful, false if out of bounds
 * Time Complexity: O(n)
 */
static inline bool dynarray_insert(DynArray *arr, size_t index, void *element) {
    if (index > arr->size) return false;
    
    dynarray_ensure_capacity(arr);
    
    // Shift elements right
    for (size_t i = arr->size; i > index; i--) {
        arr->data[i] = arr->data[i - 1];
    }
    
    arr->data[index] = element;
    arr->size++;
    return true;
}

/**
 * Remove element at specific index
 * @param arr: Target array
 * @param index: Index to remove
 * @return: Removed element, NULL if out of bounds
 * Time Complexity: O(n)
 */
static inline void *dynarray_remove(DynArray *arr, size_t index) {
    if (index >= arr->size) return NULL;
    
    void *element = arr->data[index];
    
    // Shift elements left
    for (size_t i = index; i < arr->size - 1; i++) {
        arr->data[i] = arr->data[i + 1];
    }
    
    arr->size--;
    return element;
}

/**
 * Get current size
 * @param arr: Target array
 * @return: Number of elements
 * Time Complexity: O(1)
 */
static inline size_t dynarray_size(DynArray *arr) {
    return arr->size;
}

/**
 * Get current capacity
 * @param arr: Target array
 * @return: Total capacity
 * Time Complexity: O(1)
 */
static inline size_t dynarray_capacity(DynArray *arr) {
    return arr->capacity;
}

/**
 * Check if array is empty
 * @param arr: Target array
 * @return: true if empty
 * Time Complexity: O(1)
 */
static inline bool dynarray_is_empty(DynArray *arr) {
    return arr->size == 0;
}

/**
 * Clear all elements (reset size to 0)
 * @param arr: Target array
 * Time Complexity: O(1)
 */
static inline void dynarray_clear(DynArray *arr) {
    arr->size = 0;
}

/**
 * Free all memory
 * @param arr: Target array
 * Time Complexity: O(1)
 */
static inline void dynarray_free(DynArray *arr) {
    if (arr->data) {
        free(arr->data);
        arr->data = NULL;
    }
    arr->size = 0;
    arr->capacity = 0;
}

/**
 * Shrink capacity to match size
 * @param arr: Target array
 * Time Complexity: O(n)
 */
static inline void dynarray_shrink_to_fit(DynArray *arr) {
    if (arr->size < arr->capacity && arr->size > 0) {
        dynarray_resize(arr, arr->size);
    }
}

/**
 * Reserve minimum capacity
 * @param arr: Target array
 * @param min_capacity: Minimum capacity needed
 * Time Complexity: O(n) if resize needed
 */
static inline void dynarray_reserve(DynArray *arr, size_t min_capacity) {
    if (min_capacity > arr->capacity) {
        dynarray_resize(arr, min_capacity);
    }
}

#endif // DYNARRAY_H