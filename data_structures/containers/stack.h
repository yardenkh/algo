#ifndef STACK_H
#define STACK_H

#include "../dynarray/dynarray.h"
#include "container_interface.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

/**
 * STACK IMPLEMENTATION - LIFO CONTAINER
 *
 * Last-In-First-Out data structure using dynamic array for optimal performance.
 * Provides excellent cache locality and amortized O(1) operations.
 *
 * Time Complexities:
 * - Push: O(1) amortized
 * - Pop: O(1)
 * - Peek: O(1)
 * - Size: O(1)
 *
 * Space Complexity: O(n)
 *
 * Use Cases:
 * - Expression evaluation
 * - Function call management
 * - DFS algorithms
 * - Backtracking
 * - Undo/Redo operations
 */

// Configuration
#define STACK_DEFAULT_CAPACITY 16

// Stack structure
typedef struct Stack
{
    DynArray array;                   // Underlying storage
    const ContainerInterface *vtable; // Interface for polymorphism
} Stack;

// ==================== CORE OPERATIONS ====================

/**
 * Initialize empty stack
 * @param stack: Stack to initialize
 */
static inline void stack_init(Stack *stack)
{
    dynarray_init(&stack->array, STACK_DEFAULT_CAPACITY);
    stack->vtable = NULL;
}

/**
 * Initialize with custom capacity
 * @param stack: Stack to initialize
 * @param initial_capacity: Starting capacity
 */
static inline void stack_init_with_capacity(Stack *stack, size_t initial_capacity)
{
    dynarray_init(&stack->array, initial_capacity);
    stack->vtable = NULL;
}

/**
 * Push element onto stack
 * @param stack: Target stack
 * @param data: Data to push
 */
static inline void stack_push(Stack *stack, void *data)
{
    dynarray_push(&stack->array, data);
}

/**
 * Pop element from stack
 * @param stack: Target stack
 * @return: Top element, NULL if empty
 */
static inline void *stack_pop(Stack *stack)
{
    return dynarray_pop(&stack->array);
}

/**
 * Peek at top element without removing
 * @param stack: Target stack
 * @return: Top element, NULL if empty
 */
static inline void *stack_peek(Stack *stack)
{
    size_t size = dynarray_size(&stack->array);
    return size > 0 ? dynarray_get(&stack->array, size - 1) : NULL;
}

/**
 * Get current size
 * @param stack: Target stack
 * @return: Number of elements
 */
static inline size_t stack_size(Stack *stack)
{
    return dynarray_size(&stack->array);
}

/**
 * Check if empty
 * @param stack: Target stack
 * @return: true if empty
 */
static inline bool stack_is_empty(Stack *stack)
{
    return dynarray_is_empty(&stack->array);
}

/**
 * Clear all elements
 * @param stack: Target stack
 */
static inline void stack_clear(Stack *stack)
{
    dynarray_clear(&stack->array);
}

/**
 * Free stack memory
 * @param stack: Target stack
 */
static inline void stack_free(Stack *stack)
{
    dynarray_free(&stack->array);
    stack->vtable = NULL;
}

// ==================== ADVANCED OPERATIONS ====================

/**
 * Get element at depth from top (0 = top, 1 = second from top, etc.)
 * @param stack: Target stack
 * @param depth: Depth from top
 * @return: Element at depth, NULL if out of bounds
 */
static inline void *stack_get_at_depth(Stack *stack, size_t depth)
{
    size_t size = stack_size(stack);
    return (depth < size) ? dynarray_get(&stack->array, size - 1 - depth) : NULL;
}

/**
 * Check if stack contains element
 * @param stack: Target stack
 * @param data: Data to search for
 * @return: true if found
 */
static inline bool stack_contains(Stack *stack, void *data)
{
    size_t size = stack_size(stack);
    for (size_t i = 0; i < size; i++)
    {
        if (dynarray_get(&stack->array, i) == data)
        {
            return true;
        }
    }
    return false;
}

/**
 * Find depth of element (distance from top)
 * @param stack: Target stack
 * @param data: Data to find
 * @return: Depth from top, SIZE_MAX if not found
 */
static inline size_t stack_find_depth(Stack *stack, void *data)
{
    size_t size = stack_size(stack);
    for (size_t depth = 0; depth < size; depth++)
    {
        size_t array_index = size - 1 - depth;
        if (dynarray_get(&stack->array, array_index) == data)
        {
            return depth;
        }
    }
    return SIZE_MAX; // Not found
}

/**
 * Get current capacity
 * @param stack: Target stack
 * @return: Current capacity
 */
static inline size_t stack_capacity(Stack *stack)
{
    return dynarray_capacity(&stack->array);
}

/**
 * Reserve capacity to avoid reallocations
 * @param stack: Target stack
 * @param min_capacity: Minimum capacity needed
 */
static inline void stack_reserve(Stack *stack, size_t min_capacity)
{
    dynarray_reserve(&stack->array, min_capacity);
}

/**
 * Copy stack
 * @param source: Source stack
 * @param dest: Destination stack (should be uninitialized)
 */
static inline void stack_copy(Stack *source, Stack *dest)
{
    stack_init_with_capacity(dest, stack_capacity(source));
    size_t size = stack_size(source);

    // Copy elements maintaining order (bottom to top)
    for (size_t i = 0; i < size; i++)
    {
        dynarray_push(&dest->array, dynarray_get(&source->array, i));
    }
}

// ==================== UTILITY FUNCTIONS ====================

/**
 * Print stack contents (bottom to top)
 * @param stack: Target stack
 * @param print_element: Function to print elements
 */
static inline void stack_print(Stack *stack, void (*print_element)(void *))
{
    printf("Stack (bottom to top) - Size: %zu, Capacity: %zu\n",
           stack_size(stack), stack_capacity(stack));

    if (stack_is_empty(stack))
    {
        printf("  (empty)\n");
        return;
    }

    printf("  Bottom: ");
    size_t size = stack_size(stack);
    for (size_t i = 0; i < size; i++)
    {
        if (print_element)
        {
            print_element(dynarray_get(&stack->array, i));
        }
        else
        {
            printf("%p", dynarray_get(&stack->array, i));
        }
        if (i < size - 1)
            printf(" -> ");
    }
    printf(" <- Top\n");
}

/**
 * Print stack contents (top to bottom)
 * @param stack: Target stack
 * @param print_element: Function to print elements
 */
static inline void stack_print_reverse(Stack *stack, void (*print_element)(void *))
{
    printf("Stack (top to bottom) - Size: %zu, Capacity: %zu\n",
           stack_size(stack), stack_capacity(stack));

    if (stack_is_empty(stack))
    {
        printf("  (empty)\n");
        return;
    }

    printf("  Top: ");
    size_t size = stack_size(stack);
    for (size_t i = size; i > 0; i--)
    {
        if (print_element)
        {
            print_element(dynarray_get(&stack->array, i - 1));
        }
        else
        {
            printf("%p", dynarray_get(&stack->array, i - 1));
        }
        if (i > 1)
            printf(" -> ");
    }
    printf(" <- Bottom\n");
}

// ==================== INTERFACE IMPLEMENTATION ====================

static void stack_interface_push(void *container, void *data)
{
    stack_push((Stack *)container, data);
}

static void *stack_interface_pop(void *container)
{
    return stack_pop((Stack *)container);
}

static void *stack_interface_peek(void *container)
{
    return stack_peek((Stack *)container);
}

static size_t stack_interface_size(void *container)
{
    return stack_size((Stack *)container);
}

static bool stack_interface_is_empty(void *container)
{
    return stack_is_empty((Stack *)container);
}

static void stack_interface_clear(void *container)
{
    stack_clear((Stack *)container);
}

static void stack_interface_free(void *container)
{
    stack_free((Stack *)container);
}

static const ContainerInterface stack_vtable = {
    .push = stack_interface_push,
    .pop = stack_interface_pop,
    .peek = stack_interface_peek,
    .size = stack_interface_size,
    .is_empty = stack_interface_is_empty,
    .clear = stack_interface_clear,
    .free_container = stack_interface_free};

/**
 * Initialize stack with interface support
 * @param stack: Stack to initialize
 */
static inline void stack_init_with_interface(Stack *stack)
{
    stack_init(stack);
    stack->vtable = &stack_vtable;
}

// ==================== CONVENIENCE MACROS ====================

#define STACK_PUSH_INT(stack, value)     \
    do                                   \
    {                                    \
        int *_val = malloc(sizeof(int)); \
        *_val = (value);                 \
        stack_push((stack), _val);       \
    } while (0)

#define STACK_POP_INT(stack) ((int *)stack_pop(stack))
#define STACK_PEEK_INT(stack) ((int *)stack_peek(stack))

#endif // STACK_H