#ifndef DEQUE_H
#define DEQUE_H

#include "../linkedlist/doubly_linked_list.h"
#include "container_interface.h"
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * DEQUE IMPLEMENTATION - DOUBLE-ENDED QUEUE
 *
 * Double-ended queue supporting efficient operations at both ends.
 * Built on doubly linked list for O(1) front and back operations.
 *
 * Time Complexities:
 * - Push front/back: O(1)
 * - Pop front/back: O(1)
 * - Peek front/back: O(1)
 * - Random access: O(n/2) optimized
 *
 * Space Complexity: O(n)
 *
 * Use Cases:
 * - Sliding window algorithms
 * - Bidirectional BFS
 * - Palindrome checking
 * - Work-stealing queues
 * - Undo/Redo with both ends accessible
 */

// Deque structure
typedef struct Deque
{
    DoublyLinkedList list;        // Underlying storage
    const DequeInterface *vtable; // Interface for polymorphism
} Deque;

// ==================== CORE OPERATIONS ====================

/**
 * Initialize empty deque
 * @param deque: Deque to initialize
 */
static inline void deque_init(Deque *deque)
{
    doubly_init(&deque->list);
    deque->vtable = NULL;
}

/**
 * Add element to front
 * @param deque: Target deque
 * @param data: Data to add
 */
static inline void deque_push_front(Deque *deque, void *data)
{
    doubly_push_front(&deque->list, data);
}

/**
 * Add element to back
 * @param deque: Target deque
 * @param data: Data to add
 */
static inline void deque_push_back(Deque *deque, void *data)
{
    doubly_push_back(&deque->list, data);
}

/**
 * Remove element from front
 * @param deque: Target deque
 * @return: Front element, NULL if empty
 */
static inline void *deque_pop_front(Deque *deque)
{
    return doubly_pop_front(&deque->list);
}

/**
 * Remove element from back
 * @param deque: Target deque
 * @return: Back element, NULL if empty
 */
static inline void *deque_pop_back(Deque *deque)
{
    return doubly_pop_back(&deque->list);
}

/**
 * View front element without removing
 * @param deque: Target deque
 * @return: Front element, NULL if empty
 */
static inline void *deque_peek_front(Deque *deque)
{
    return doubly_get_at(&deque->list, 0);
}

/**
 * View back element without removing
 * @param deque: Target deque
 * @return: Back element, NULL if empty
 */
static inline void *deque_peek_back(Deque *deque)
{
    size_t size = doubly_size(&deque->list);
    return size > 0 ? doubly_get_at(&deque->list, size - 1) : NULL;
}

/**
 * Get current size
 * @param deque: Target deque
 * @return: Number of elements
 */
static inline size_t deque_size(Deque *deque)
{
    return doubly_size(&deque->list);
}

/**
 * Check if empty
 * @param deque: Target deque
 * @return: true if empty
 */
static inline bool deque_is_empty(Deque *deque)
{
    return doubly_is_empty(&deque->list);
}

/**
 * Clear all elements
 * @param deque: Target deque
 */
static inline void deque_clear(Deque *deque)
{
    doubly_clear(&deque->list);
}

/**
 * Free deque memory
 * @param deque: Target deque
 */
static inline void deque_free(Deque *deque)
{
    doubly_free(&deque->list);
    deque->vtable = NULL;
}

// ==================== INDEXED ACCESS OPERATIONS ====================

/**
 * Get element at index (0 = front, size-1 = back)
 * @param deque: Target deque
 * @param index: Index to access
 * @return: Element at index, NULL if out of bounds
 */
static inline void *deque_get_at(Deque *deque, size_t index)
{
    return doubly_get_at(&deque->list, index);
}

/**
 * Set element at index
 * @param deque: Target deque
 * @param index: Index to set
 * @param data: New data value
 * @return: true if successful, false if out of bounds
 */
static inline bool deque_set_at(Deque *deque, size_t index, void *data)
{
    if (index >= deque_size(deque))
        return false;
    doubly_set_at(&deque->list, index, data);
    return true;
}

/**
 * Insert element at index
 * @param deque: Target deque
 * @param index: Insertion index
 * @param data: Data to insert
 * @return: true if successful, false if out of bounds
 */
static inline bool deque_insert_at(Deque *deque, size_t index, void *data)
{
    if (index > deque_size(deque))
        return false;
    doubly_insert_at(&deque->list, index, data);
    return true;
}

/**
 * Remove element at index
 * @param deque: Target deque
 * @param index: Index to remove
 * @return: Removed element, NULL if out of bounds
 */
static inline void *deque_remove_at(Deque *deque, size_t index)
{
    return doubly_remove_at(&deque->list, index);
}

// ==================== SEARCH AND UTILITY OPERATIONS ====================

/**
 * Check if deque contains element
 * @param deque: Target deque
 * @param data: Data to search for
 * @return: true if found
 */
static inline bool deque_contains(Deque *deque, void *data)
{
    return doubly_contains(&deque->list, data);
}

/**
 * Find index of element
 * @param deque: Target deque
 * @param data: Data to find
 * @return: Index of element, SIZE_MAX if not found
 */
static inline size_t deque_find(Deque *deque, void *data)
{
    size_t size = deque_size(deque);
    for (size_t i = 0; i < size; i++)
    {
        if (deque_get_at(deque, i) == data)
        {
            return i;
        }
    }
    return SIZE_MAX;
}

/**
 * Reverse deque order
 * @param deque: Target deque
 */
static inline void deque_reverse(Deque *deque)
{
    doubly_reverse(&deque->list);
}

// ==================== ROTATION OPERATIONS ====================

/**
 * Rotate left (elements move from front to back)
 * @param deque: Target deque
 * @param steps: Number of steps to rotate
 */
static inline void deque_rotate_left(Deque *deque, size_t steps)
{
    size_t size = deque_size(deque);
    if (size <= 1 || steps == 0)
        return;

    steps = steps % size; // Optimize for steps > size
    if (steps == 0)
        return;

    // Move 'steps' elements from front to back
    for (size_t i = 0; i < steps; i++)
    {
        void *data = deque_pop_front(deque);
        deque_push_back(deque, data);
    }
}

/**
 * Rotate right (elements move from back to front)
 * @param deque: Target deque
 * @param steps: Number of steps to rotate
 */
static inline void deque_rotate_right(Deque *deque, size_t steps)
{
    size_t size = deque_size(deque);
    if (size <= 1 || steps == 0)
        return;

    steps = steps % size; // Optimize for steps > size
    if (steps == 0)
        return;

    // Move 'steps' elements from back to front
    for (size_t i = 0; i < steps; i++)
    {
        void *data = deque_pop_back(deque);
        deque_push_front(deque, data);
    }
}

// ==================== MERGE OPERATIONS ====================

/**
 * Merge another deque to back
 * @param dest: Destination deque
 * @param source: Source deque (will be emptied)
 */
static inline void deque_merge_back(Deque *dest, Deque *source)
{
    while (!deque_is_empty(source))
    {
        void *data = deque_pop_front(source);
        deque_push_back(dest, data);
    }
}

/**
 * Merge another deque to front
 * @param dest: Destination deque
 * @param source: Source deque (will be emptied)
 */
static inline void deque_merge_front(Deque *dest, Deque *source)
{
    while (!deque_is_empty(source))
    {
        void *data = deque_pop_back(source);
        deque_push_front(dest, data);
    }
}

/**
 * Copy deque
 * @param source: Source deque
 * @param dest: Destination deque (should be uninitialized)
 */
static inline void deque_copy(Deque *source, Deque *dest)
{
    deque_init(dest);
    size_t size = deque_size(source);

    // Copy all elements maintaining order
    for (size_t i = 0; i < size; i++)
    {
        void *data = deque_get_at(source, i);
        deque_push_back(dest, data);
    }
}

// ==================== UTILITY FUNCTIONS ====================

/**
 * Print deque contents (front to back)
 * @param deque: Target deque
 * @param print_element: Function to print elements
 */
static inline void deque_print(Deque *deque, void (*print_element)(void *))
{
    printf("Deque (front to back) - Size: %zu\n", deque_size(deque));

    if (deque_is_empty(deque))
    {
        printf("  (empty)\n");
        return;
    }

    printf("  Front -> ");
    size_t size = deque_size(deque);
    for (size_t i = 0; i < size; i++)
    {
        void *data = deque_get_at(deque, i);
        if (print_element)
        {
            print_element(data);
        }
        else
        {
            printf("%p", data);
        }
        if (i < size - 1)
            printf(" <-> ");
    }
    printf(" <- Back\n");
}

// ==================== INTERFACE IMPLEMENTATION ====================

// Basic container interface
static void deque_interface_push(void *container, void *data)
{
    deque_push_back((Deque *)container, data); // Default to back
}

static void *deque_interface_pop(void *container)
{
    return deque_pop_front((Deque *)container); // Default to front
}

static void *deque_interface_peek(void *container)
{
    return deque_peek_front((Deque *)container); // Default to front
}

static size_t deque_interface_size(void *container)
{
    return deque_size((Deque *)container);
}

static bool deque_interface_is_empty(void *container)
{
    return deque_is_empty((Deque *)container);
}

static void deque_interface_clear(void *container)
{
    deque_clear((Deque *)container);
}

static void deque_interface_free(void *container)
{
    deque_free((Deque *)container);
}

// Deque-specific interface
static void deque_interface_push_front(void *container, void *data)
{
    deque_push_front((Deque *)container, data);
}

static void deque_interface_push_back(void *container, void *data)
{
    deque_push_back((Deque *)container, data);
}

static void *deque_interface_pop_front(void *container)
{
    return deque_pop_front((Deque *)container);
}

static void *deque_interface_pop_back(void *container)
{
    return deque_pop_back((Deque *)container);
}

static void *deque_interface_peek_front(void *container)
{
    return deque_peek_front((Deque *)container);
}

static void *deque_interface_peek_back(void *container)
{
    return deque_peek_back((Deque *)container);
}

static const DequeInterface deque_vtable = {
    .base = {
        .push = deque_interface_push,
        .pop = deque_interface_pop,
        .peek = deque_interface_peek,
        .size = deque_interface_size,
        .is_empty = deque_interface_is_empty,
        .clear = deque_interface_clear,
        .free_container = deque_interface_free},
    .push_front = deque_interface_push_front,
    .push_back = deque_interface_push_back,
    .pop_front = deque_interface_pop_front,
    .pop_back = deque_interface_pop_back,
    .peek_front = deque_interface_peek_front,
    .peek_back = deque_interface_peek_back};

/**
 * Initialize deque with interface support
 * @param deque: Deque to initialize
 */
static inline void deque_init_with_interface(Deque *deque)
{
    deque_init(deque);
    deque->vtable = &deque_vtable;
}

// ==================== CONVENIENCE MACROS ====================

#define DEQUE_PUSH_FRONT_INT(deque, value) \
    do                                     \
    {                                      \
        int *_val = malloc(sizeof(int));   \
        *_val = (value);                   \
        deque_push_front((deque), _val);   \
    } while (0)

#define DEQUE_PUSH_BACK_INT(deque, value) \
    do                                    \
    {                                     \
        int *_val = malloc(sizeof(int));  \
        *_val = (value);                  \
        deque_push_back((deque), _val);   \
    } while (0)

#define DEQUE_POP_FRONT_INT(deque) ((int *)deque_pop_front(deque))
#define DEQUE_POP_BACK_INT(deque) ((int *)deque_pop_back(deque))
#define DEQUE_PEEK_FRONT_INT(deque) ((int *)deque_peek_front(deque))
#define DEQUE_PEEK_BACK_INT(deque) ((int *)deque_peek_back(deque))

#endif // DEQUE_H