#ifndef QUEUE_H
#define QUEUE_H

#include "../linkedlist/doubly_linked_list.h"
#include "container_interface.h"
#include <stdio.h>
#include <stdbool.h>

/**
 * QUEUE IMPLEMENTATION - FIFO CONTAINER
 *
 * First-In-First-Out data structure using doubly linked list.
 * Provides O(1) operations at both ends without the complexity of circular buffers.
 *
 * Time Complexities:
 * - Enqueue: O(1)
 * - Dequeue: O(1)
 * - Peek: O(1)
 * - Size: O(1)
 *
 * Space Complexity: O(n)
 *
 * Use Cases:
 * - Task scheduling
 * - BFS algorithms
 * - Producer-consumer patterns
 * - Request processing
 * - Print job management
 */

// Queue structure
typedef struct Queue
{
    DoublyLinkedList list;            // Underlying storage
    const ContainerInterface *vtable; // Interface for polymorphism
} Queue;

// ==================== CORE OPERATIONS ====================

/**
 * Initialize empty queue
 * @param queue: Queue to initialize
 */
static inline void queue_init(Queue *queue)
{
    doubly_init(&queue->list);
    queue->vtable = NULL;
}

/**
 * Add element to rear of queue (enqueue)
 * @param queue: Target queue
 * @param data: Data to add
 */
static inline void queue_enqueue(Queue *queue, void *data)
{
    doubly_push_back(&queue->list, data);
}

/**
 * Remove element from front of queue (dequeue)
 * @param queue: Target queue
 * @return: Front element, NULL if empty
 */
static inline void *queue_dequeue(Queue *queue)
{
    return doubly_pop_front(&queue->list);
}

/**
 * View front element without removing (peek)
 * @param queue: Target queue
 * @return: Front element, NULL if empty
 */
static inline void *queue_peek(Queue *queue)
{
    return doubly_get_at(&queue->list, 0);
}

/**
 * View rear element without removing
 * @param queue: Target queue
 * @return: Rear element, NULL if empty
 */
static inline void *queue_peek_rear(Queue *queue)
{
    size_t size = doubly_size(&queue->list);
    return size > 0 ? doubly_get_at(&queue->list, size - 1) : NULL;
}

/**
 * Get current size
 * @param queue: Target queue
 * @return: Number of elements
 */
static inline size_t queue_size(Queue *queue)
{
    return doubly_size(&queue->list);
}

/**
 * Check if empty
 * @param queue: Target queue
 * @return: true if empty
 */
static inline bool queue_is_empty(Queue *queue)
{
    return doubly_is_empty(&queue->list);
}

/**
 * Clear all elements
 * @param queue: Target queue
 */
static inline void queue_clear(Queue *queue)
{
    doubly_clear(&queue->list);
}

/**
 * Free queue memory
 * @param queue: Target queue
 */
static inline void queue_free(Queue *queue)
{
    doubly_free(&queue->list);
    queue->vtable = NULL;
}

// ==================== ADVANCED OPERATIONS ====================

/**
 * Get element at position from front (0 = front, 1 = second, etc.)
 * @param queue: Target queue
 * @param index: Index from front
 * @return: Element at index, NULL if out of bounds
 */
static inline void *queue_get_at(Queue *queue, size_t index)
{
    return doubly_get_at(&queue->list, index);
}

/**
 * Check if queue contains element
 * @param queue: Target queue
 * @param data: Data to search for
 * @return: true if found
 */
static inline bool queue_contains(Queue *queue, void *data)
{
    return doubly_contains(&queue->list, data);
}

/**
 * Copy queue
 * @param source: Source queue
 * @param dest: Destination queue (should be uninitialized)
 */
static inline void queue_copy(Queue *source, Queue *dest)
{
    queue_init(dest);
    size_t size = queue_size(source);

    // Copy all elements maintaining FIFO order
    for (size_t i = 0; i < size; i++)
    {
        void *data = queue_get_at(source, i);
        queue_enqueue(dest, data);
    }
}

/**
 * Reverse queue order (front becomes rear, rear becomes front)
 * @param queue: Target queue
 */
static inline void queue_reverse(Queue *queue)
{
    doubly_reverse(&queue->list);
}

// ==================== UTILITY FUNCTIONS ====================

/**
 * Print queue contents (front to rear)
 * @param queue: Target queue
 * @param print_element: Function to print elements
 */
static inline void queue_print(Queue *queue, void (*print_element)(void *))
{
    printf("Queue (front to rear) - Size: %zu\n", queue_size(queue));

    if (queue_is_empty(queue))
    {
        printf("  (empty)\n");
        return;
    }

    printf("  Front -> ");
    size_t size = queue_size(queue);
    for (size_t i = 0; i < size; i++)
    {
        void *data = queue_get_at(queue, i);
        if (print_element)
        {
            print_element(data);
        }
        else
        {
            printf("%p", data);
        }
        if (i < size - 1)
            printf(" -> ");
    }
    printf(" <- Rear\n");
}

// ==================== INTERFACE IMPLEMENTATION ====================

static void queue_interface_push(void *container, void *data)
{
    queue_enqueue((Queue *)container, data);
}

static void *queue_interface_pop(void *container)
{
    return queue_dequeue((Queue *)container);
}

static void *queue_interface_peek(void *container)
{
    return queue_peek((Queue *)container);
}

static size_t queue_interface_size(void *container)
{
    return queue_size((Queue *)container);
}

static bool queue_interface_is_empty(void *container)
{
    return queue_is_empty((Queue *)container);
}

static void queue_interface_clear(void *container)
{
    queue_clear((Queue *)container);
}

static void queue_interface_free(void *container)
{
    queue_free((Queue *)container);
}

static const ContainerInterface queue_vtable = {
    .push = queue_interface_push,
    .pop = queue_interface_pop,
    .peek = queue_interface_peek,
    .size = queue_interface_size,
    .is_empty = queue_interface_is_empty,
    .clear = queue_interface_clear,
    .free_container = queue_interface_free};

/**
 * Initialize queue with interface support
 * @param queue: Queue to initialize
 */
static inline void queue_init_with_interface(Queue *queue)
{
    queue_init(queue);
    queue->vtable = &queue_vtable;
}

// ==================== CONVENIENCE MACROS ====================

#define QUEUE_ENQUEUE_INT(queue, value)  \
    do                                   \
    {                                    \
        int *_val = malloc(sizeof(int)); \
        *_val = (value);                 \
        queue_enqueue((queue), _val);    \
    } while (0)

#define QUEUE_DEQUEUE_INT(queue) ((int *)queue_dequeue(queue))
#define QUEUE_PEEK_INT(queue) ((int *)queue_peek(queue))

#endif // QUEUE_H