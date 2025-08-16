#ifndef CIRCULAR_LINKED_LIST_H
#define CIRCULAR_LINKED_LIST_H

#include "list_interface.h"
#include <stdlib.h>
#include <stdbool.h>

/**
 * CIRCULAR LINKED LIST IMPLEMENTATION
 *
 * A linked list where the last node points back to the first node, forming a circle.
 * Useful for round-robin scheduling, circular buffers, and cyclic data access.
 *
 * Time Complexities:
 * - Push front/back: O(1) with tail pointer
 * - Pop front/back: O(1) with tail pointer
 * - Rotate: O(k) where k is number of steps
 * - Search: O(n)
 *
 * Space Complexity: O(n) where n is number of elements
 *
 * Key Properties:
 * - Always circular (even with one element, it points to itself)
 * - No NULL pointers in a non-empty list
 * - Supports efficient rotation operations
 */

// Node structure for circular linked list
// Each node contains data and a pointer to the next node in the circle
typedef struct CircularListNode
{
    void *data;                    // Pointer to stored data
    struct CircularListNode *next; // Pointer to next node (forms circle)
} CircularListNode;

// Circular linked list structure
// Uses tail pointer for efficient operations (tail->next is head)
typedef struct CircularLinkedList
{
    CircularListNode *tail;              // Pointer to last node (tail->next = head)
    CircularListNode *current;           // Current position for iteration
    size_t size;                         // Current number of elements
    const CircularListInterface *vtable; // Interface for polymorphism
} CircularLinkedList;

// ==================== NODE OPERATIONS ====================

/**
 * Create a new circular linked list node
 * @param data: Pointer to data to store in the node
 * @return: Pointer to newly created node, NULL on allocation failure
 * Time Complexity: O(1)
 */
static inline CircularListNode *circular_create_node(void *data)
{
    CircularListNode *node = (CircularListNode *)malloc(sizeof(CircularListNode));
    if (!node)
        return NULL; // Handle allocation failure

    node->data = data; // Store the data pointer
    node->next = node; // Point to self (single node circle)
    return node;
}

/**
 * Free a circular linked list node
 * Note: Does not free the data itself, only the node structure
 * @param node: Node to deallocate
 * Time Complexity: O(1)
 */
static inline void circular_free_node(CircularListNode *node)
{
    if (node)
    {
        // Note: We don't free node->data as we don't own the data
        // The caller is responsible for managing the actual data
        free(node);
    }
}

// ==================== LIST OPERATIONS ====================

/**
 * Initialize an empty circular linked list
 * @param list: List structure to initialize
 * Time Complexity: O(1)
 */
static inline void circular_init(CircularLinkedList *list)
{
    list->tail = NULL;    // Start with empty list
    list->current = NULL; // No current position
    list->size = 0;       // Zero elements
    list->vtable = NULL;  // Set interface later if needed
}

/**
 * Add element to the front of the circular list
 * Front is defined as the position after tail (tail->next)
 * @param list: Target list
 * @param data: Data to add
 * Time Complexity: O(1)
 * Space Complexity: O(1)
 */
static inline void circular_push_front(CircularLinkedList *list, void *data)
{
    CircularListNode *node = circular_create_node(data);
    if (!node)
        return; // Handle allocation failure gracefully

    if (list->tail == NULL)
    {
        // First element - becomes both head and tail
        list->tail = node;
        list->current = node; // Set current to the first element
    }
    else
    {
        // Insert after tail (which becomes new head)
        node->next = list->tail->next;
        list->tail->next = node;
    }
    list->size++;
}

/**
 * Add element to the back of the circular list
 * Back is the tail position
 * @param list: Target list
 * @param data: Data to add
 * Time Complexity: O(1)
 * Space Complexity: O(1)
 */
static inline void circular_push_back(CircularLinkedList *list, void *data)
{
    CircularListNode *node = circular_create_node(data);
    if (!node)
        return; // Handle allocation failure gracefully

    if (list->tail == NULL)
    {
        // First element - becomes both head and tail
        list->tail = node;
        list->current = node; // Set current to the first element
    }
    else
    {
        // Insert after tail and update tail
        node->next = list->tail->next;
        list->tail->next = node;
        list->tail = node; // New node becomes tail
    }
    list->size++;
}

/**
 * Remove and return element from the front of the circular list
 * Front is defined as the position after tail (tail->next)
 * @param list: Target list
 * @return: Data from removed element, NULL if list is empty
 * Time Complexity: O(1)
 * Space Complexity: O(1)
 */
static inline void *circular_pop_front(CircularLinkedList *list)
{
    if (list->tail == NULL)
        return NULL; // Empty list

    CircularListNode *head = list->tail->next;
    void *data = head->data; // Extract data before freeing node

    if (list->size == 1)
    {
        // Only one element
        list->tail = NULL;
        list->current = NULL;
    }
    else
    {
        // Remove head and fix circular link
        list->tail->next = head->next;

        // Update current if it was pointing to removed node
        if (list->current == head)
        {
            list->current = head->next;
        }
    }

    circular_free_node(head); // Deallocate the node
    list->size--;
    return data;
}

/**
 * Remove and return element from the back of the circular list
 * Back is the tail position
 * @param list: Target list
 * @return: Data from removed element, NULL if list is empty
 * Time Complexity: O(n) - need to find second-to-last node
 * Space Complexity: O(1)
 */
static inline void *circular_pop_back(CircularLinkedList *list)
{
    if (list->tail == NULL)
        return NULL; // Empty list

    void *data = list->tail->data; // Extract data before freeing node

    if (list->size == 1)
    {
        // Only one element
        circular_free_node(list->tail);
        list->tail = NULL;
        list->current = NULL;
    }
    else
    {
        // Find second-to-last node
        CircularListNode *current = list->tail->next; // Start from head
        while (current->next != list->tail)
        {
            current = current->next;
        }

        // Update current if it was pointing to removed node
        if (list->current == list->tail)
        {
            list->current = current;
        }

        // Remove tail and update
        current->next = list->tail->next;
        circular_free_node(list->tail);
        list->tail = current;
    }

    list->size--;
    return data;
}

/**
 * Rotate the circular list forward by specified steps
 * This effectively moves the "head" position forward in the circle
 * @param list: Target list
 * @param steps: Number of positions to rotate forward
 * Time Complexity: O(steps % size)
 * Space Complexity: O(1)
 */
static inline void circular_rotate_forward(CircularLinkedList *list, size_t steps)
{
    if (list->size <= 1 || steps == 0)
        return; // Nothing to rotate

    steps = steps % list->size; // Optimize by taking modulo
    if (steps == 0)
        return;

    // Move tail forward by steps
    for (size_t i = 0; i < steps; i++)
    {
        list->tail = list->tail->next;
    }
}

/**
 * Rotate the circular list backward by specified steps
 * This effectively moves the "head" position backward in the circle
 * @param list: Target list
 * @param steps: Number of positions to rotate backward
 * Time Complexity: O((size - steps) % size)
 * Space Complexity: O(1)
 */
static inline void circular_rotate_backward(CircularLinkedList *list, size_t steps)
{
    if (list->size <= 1 || steps == 0)
        return; // Nothing to rotate

    steps = steps % list->size; // Optimize by taking modulo
    if (steps == 0)
        return;

    // Convert backward rotation to forward rotation
    size_t forward_steps = list->size - steps;
    circular_rotate_forward(list, forward_steps);
}

/**
 * Get the current element without removing it
 * @param list: Target list
 * @return: Data at current position, NULL if list is empty
 * Time Complexity: O(1)
 */
static inline void *circular_current(CircularLinkedList *list)
{
    if (list->current == NULL)
        return NULL;
    return list->current->data;
}

/**
 * Advance the current position to the next element
 * @param list: Target list
 * Time Complexity: O(1)
 */
static inline void circular_advance(CircularLinkedList *list)
{
    if (list->current)
    {
        list->current = list->current->next;
    }
}

/**
 * Verify that the list maintains circular property
 * Used for debugging and validation
 * @param list: Target list
 * @return: true if properly circular, false otherwise
 * Time Complexity: O(n)
 */
static inline bool circular_is_circular(CircularLinkedList *list)
{
    if (list->tail == NULL)
        return true; // Empty list is trivially circular

    CircularListNode *slow = list->tail->next; // Start from head
    CircularListNode *fast = list->tail->next;

    // Use Floyd's cycle detection (though we expect a cycle)
    do
    {
        if (fast == NULL || fast->next == NULL)
            return false; // Found NULL pointer
        slow = slow->next;
        fast = fast->next->next;
    } while (slow != fast && fast != list->tail->next);

    // Verify we can reach tail from head in exactly 'size' steps
    CircularListNode *current = list->tail->next;
    for (size_t i = 0; i < list->size; i++)
    {
        if (i == list->size - 1 && current != list->tail)
            return false;
        current = current->next;
    }

    return current == list->tail->next; // Should be back at head
}

/**
 * Get element at specified index
 * Index 0 is head (tail->next), index increases clockwise
 * @param list: Target list
 * @param index: Zero-based index
 * @return: Data at index, NULL if index out of bounds
 * Time Complexity: O(index % size)
 * Space Complexity: O(1)
 */
static inline void *circular_get_at(CircularLinkedList *list, size_t index)
{
    if (list->size == 0)
        return NULL; // Empty list

    index = index % list->size; // Wrap around for circular access

    CircularListNode *current = list->tail->next; // Start from head
    for (size_t i = 0; i < index; i++)
    {
        current = current->next;
    }
    return current->data;
}

/**
 * Search for an element in the circular list
 * @param list: Target list
 * @param data: Data to search for (pointer comparison)
 * @return: true if found, false otherwise
 * Time Complexity: O(n)
 * Space Complexity: O(1)
 */
static inline bool circular_contains(CircularLinkedList *list, void *data)
{
    if (list->size == 0)
        return false; // Empty list

    CircularListNode *current = list->tail->next; // Start from head
    CircularListNode *head = current;

    do
    {
        if (current->data == data)
            return true; // Pointer comparison
        current = current->next;
    } while (current != head); // Stop when we complete the circle

    return false;
}

/**
 * Get current size of the circular list
 * @param list: Target list
 * @return: Number of elements in the list
 * Time Complexity: O(1)
 */
static inline size_t circular_size(CircularLinkedList *list)
{
    return list->size;
}

/**
 * Check if the circular list is empty
 * @param list: Target list
 * @return: true if empty, false otherwise
 * Time Complexity: O(1)
 */
static inline bool circular_is_empty(CircularLinkedList *list)
{
    return list->size == 0;
}

/**
 * Remove all elements from the circular list
 * Frees all nodes but not the data they point to
 * @param list: Target list
 * Time Complexity: O(n)
 * Space Complexity: O(1)
 */
static inline void circular_clear(CircularLinkedList *list)
{
    if (list->tail == NULL)
        return; // Already empty

    CircularListNode *current = list->tail->next; // Start from head
    CircularListNode *head = current;

    // Break the circle first to avoid infinite loop
    list->tail->next = NULL;

    // Free all nodes
    while (current)
    {
        CircularListNode *next = current->next;
        circular_free_node(current);
        current = next;
    }

    list->tail = NULL;
    list->current = NULL;
    list->size = 0;
}

/**
 * Free the entire circular list structure
 * Calls clear() then resets the list to initial state
 * @param list: Target list
 * Time Complexity: O(n)
 * Space Complexity: O(1)
 */
static inline void circular_free(CircularLinkedList *list)
{
    circular_clear(list); // Remove all elements
    // List structure itself is not freed as it may be stack-allocated
    // Caller is responsible for list structure memory management
}

#endif // CIRCULAR_LINKED_LIST_H