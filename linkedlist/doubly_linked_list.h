#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include "list_interface.h"
#include <stdlib.h>
#include <stdbool.h>

/**
 * DOUBLY LINKED LIST IMPLEMENTATION
 *
 * Linked list with both forward and backward pointers.
 * Supports efficient operations at both ends and bidirectional traversal.
 *
 * Time Complexities:
 * - Push front/back: O(1)
 * - Pop front/back: O(1)
 * - Insert/remove at index: O(n/2) average (optimized traversal)
 * - Search: O(n)
 *
 * Space Complexity: O(n) - higher memory usage due to prev pointers
 */

// Node structure
typedef struct DoublyListNode
{
    void *data;                  // Stored data
    struct DoublyListNode *next; // Next node
    struct DoublyListNode *prev; // Previous node
} DoublyListNode;

// List structure
typedef struct DoublyLinkedList
{
    DoublyListNode *head;                // First node
    DoublyListNode *tail;                // Last node
    size_t size;                         // Element count
    const ExtendedListInterface *vtable; // Interface for polymorphism
} DoublyLinkedList;

// ==================== NODE OPERATIONS ====================

/**
 * Create new node
 * @param data: Data to store
 * @return: New node or NULL on failure
 */
static inline DoublyListNode *doubly_create_node(void *data)
{
    DoublyListNode *node = (DoublyListNode *)malloc(sizeof(DoublyListNode));
    if (!node)
        return NULL;

    node->data = data;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

/**
 * Free node
 * @param node: Node to free
 */
static inline void doubly_free_node(DoublyListNode *node)
{
    if (node)
        free(node);
}

// ==================== CORE OPERATIONS ====================

/**
 * Initialize empty list
 * @param list: List to initialize
 */
static inline void doubly_init(DoublyLinkedList *list)
{
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    list->vtable = NULL;
}

/**
 * Add element to front
 * @param list: Target list
 * @param data: Data to add
 */
static inline void doubly_push_front(DoublyLinkedList *list, void *data)
{
    DoublyListNode *node = doubly_create_node(data);
    if (!node)
        return;

    if (!list->head)
    {
        list->head = list->tail = node;
    }
    else
    {
        node->next = list->head;
        list->head->prev = node;
        list->head = node;
    }
    list->size++;
}

/**
 * Add element to back
 * @param list: Target list
 * @param data: Data to add
 */
static inline void doubly_push_back(DoublyLinkedList *list, void *data)
{
    DoublyListNode *node = doubly_create_node(data);
    if (!node)
        return;

    if (!list->tail)
    {
        list->head = list->tail = node;
    }
    else
    {
        list->tail->next = node;
        node->prev = list->tail;
        list->tail = node;
    }
    list->size++;
}

/**
 * Remove element from front
 * @param list: Target list
 * @return: Data from removed element, NULL if empty
 */
static inline void *doubly_pop_front(DoublyLinkedList *list)
{
    if (!list->head)
        return NULL;

    DoublyListNode *node = list->head;
    void *data = node->data;

    if (list->head == list->tail)
    {
        list->head = list->tail = NULL;
    }
    else
    {
        list->head = node->next;
        list->head->prev = NULL;
    }

    doubly_free_node(node);
    list->size--;
    return data;
}

/**
 * Remove element from back
 * @param list: Target list
 * @return: Data from removed element, NULL if empty
 */
static inline void *doubly_pop_back(DoublyLinkedList *list)
{
    if (!list->tail)
        return NULL;

    DoublyListNode *node = list->tail;
    void *data = node->data;

    if (list->head == list->tail)
    {
        list->head = list->tail = NULL;
    }
    else
    {
        list->tail = node->prev;
        list->tail->next = NULL;
    }

    doubly_free_node(node);
    list->size--;
    return data;
}

/**
 * Get element at index (optimized traversal from nearest end)
 * @param list: Target list
 * @param index: Index to access
 * @return: Data at index, NULL if out of bounds
 */
static inline void *doubly_get_at(DoublyLinkedList *list, size_t index)
{
    if (index >= list->size)
        return NULL;

    DoublyListNode *current;

    // Choose traversal direction based on which end is closer
    if (index < list->size / 2)
    {
        // Traverse from head
        current = list->head;
        for (size_t i = 0; i < index; i++)
        {
            current = current->next;
        }
    }
    else
    {
        // Traverse from tail
        current = list->tail;
        for (size_t i = list->size - 1; i > index; i--)
        {
            current = current->prev;
        }
    }

    return current->data;
}

/**
 * Set element at index
 * @param list: Target list
 * @param index: Index to set
 * @param data: New data value
 */
static inline void doubly_set_at(DoublyLinkedList *list, size_t index, void *data)
{
    if (index >= list->size)
        return;

    DoublyListNode *current;

    if (index < list->size / 2)
    {
        current = list->head;
        for (size_t i = 0; i < index; i++)
        {
            current = current->next;
        }
    }
    else
    {
        current = list->tail;
        for (size_t i = list->size - 1; i > index; i--)
        {
            current = current->prev;
        }
    }

    current->data = data;
}

/**
 * Insert at index
 * @param list: Target list
 * @param index: Insertion index
 * @param data: Data to insert
 */
static inline void doubly_insert_at(DoublyLinkedList *list, size_t index, void *data)
{
    if (index > list->size)
        return;

    if (index == 0)
    {
        doubly_push_front(list, data);
        return;
    }
    if (index == list->size)
    {
        doubly_push_back(list, data);
        return;
    }

    DoublyListNode *current;

    if (index < list->size / 2)
    {
        current = list->head;
        for (size_t i = 0; i < index; i++)
        {
            current = current->next;
        }
    }
    else
    {
        current = list->tail;
        for (size_t i = list->size - 1; i > index; i--)
        {
            current = current->prev;
        }
    }

    DoublyListNode *new_node = doubly_create_node(data);
    if (!new_node)
        return;

    new_node->next = current;
    new_node->prev = current->prev;

    if (current->prev)
    {
        current->prev->next = new_node;
    }
    current->prev = new_node;

    list->size++;
}

/**
 * Remove at index
 * @param list: Target list
 * @param index: Index to remove
 * @return: Data from removed element, NULL if out of bounds
 */
static inline void *doubly_remove_at(DoublyLinkedList *list, size_t index)
{
    if (index >= list->size)
        return NULL;

    if (index == 0)
        return doubly_pop_front(list);
    if (index == list->size - 1)
        return doubly_pop_back(list);

    DoublyListNode *current;

    if (index < list->size / 2)
    {
        current = list->head;
        for (size_t i = 0; i < index; i++)
        {
            current = current->next;
        }
    }
    else
    {
        current = list->tail;
        for (size_t i = list->size - 1; i > index; i--)
        {
            current = current->prev;
        }
    }

    void *data = current->data;

    if (current->prev)
        current->prev->next = current->next;
    if (current->next)
        current->next->prev = current->prev;

    doubly_free_node(current);
    list->size--;
    return data;
}

/**
 * Search for element
 * @param list: Target list
 * @param data: Data to find
 * @return: true if found
 */
static inline bool doubly_contains(DoublyLinkedList *list, void *data)
{
    DoublyListNode *current = list->head;
    while (current)
    {
        if (current->data == data)
            return true;
        current = current->next;
    }
    return false;
}

/**
 * Get size
 * @param list: Target list
 * @return: Number of elements
 */
static inline size_t doubly_size(DoublyLinkedList *list)
{
    return list->size;
}

/**
 * Check if empty
 * @param list: Target list
 * @return: true if empty
 */
static inline bool doubly_is_empty(DoublyLinkedList *list)
{
    return list->size == 0;
}

/**
 * Clear all elements
 * @param list: Target list
 */
static inline void doubly_clear(DoublyLinkedList *list)
{
    while (list->head)
    {
        DoublyListNode *next = list->head->next;
        doubly_free_node(list->head);
        list->head = next;
    }
    list->tail = NULL;
    list->size = 0;
}

/**
 * Free list
 * @param list: Target list
 */
static inline void doubly_free(DoublyLinkedList *list)
{
    doubly_clear(list);
}

/**
 * Reverse list in place
 * @param list: Target list
 */
static inline void doubly_reverse(DoublyLinkedList *list)
{
    if (list->size <= 1)
        return;

    DoublyListNode *current = list->head;
    DoublyListNode *temp;

    // Swap next and prev pointers for all nodes
    while (current)
    {
        temp = current->prev;
        current->prev = current->next;
        current->next = temp;
        current = current->prev; // Move to next (was prev)
    }

    // Swap head and tail
    temp = list->head;
    list->head = list->tail;
    list->tail = temp;
}

// ==================== INTERFACE IMPLEMENTATION ====================

static void doubly_interface_push_front(void *list, void *data)
{
    doubly_push_front((DoublyLinkedList *)list, data);
}

static void doubly_interface_push_back(void *list, void *data)
{
    doubly_push_back((DoublyLinkedList *)list, data);
}

static void *doubly_interface_pop_front(void *list)
{
    return doubly_pop_front((DoublyLinkedList *)list);
}

static void *doubly_interface_pop_back(void *list)
{
    return doubly_pop_back((DoublyLinkedList *)list);
}

static size_t doubly_interface_size(void *list)
{
    return doubly_size((DoublyLinkedList *)list);
}

static bool doubly_interface_is_empty(void *list)
{
    return doubly_is_empty((DoublyLinkedList *)list);
}

static void doubly_interface_clear(void *list)
{
    doubly_clear((DoublyLinkedList *)list);
}

static void doubly_interface_free_list(void *list)
{
    doubly_free((DoublyLinkedList *)list);
}

static void *doubly_interface_get_at(void *list, size_t index)
{
    return doubly_get_at((DoublyLinkedList *)list, index);
}

static void doubly_interface_set_at(void *list, size_t index, void *data)
{
    doubly_set_at((DoublyLinkedList *)list, index, data);
}

static void doubly_interface_insert_at(void *list, size_t index, void *data)
{
    doubly_insert_at((DoublyLinkedList *)list, index, data);
}

static void *doubly_interface_remove_at(void *list, size_t index)
{
    return doubly_remove_at((DoublyLinkedList *)list, index);
}

static bool doubly_interface_contains(void *list, void *data)
{
    return doubly_contains((DoublyLinkedList *)list, data);
}

static void doubly_interface_reverse(void *list)
{
    doubly_reverse((DoublyLinkedList *)list);
}

static const ExtendedListInterface doubly_vtable = {
    .base = {
        .push_front = doubly_interface_push_front,
        .push_back = doubly_interface_push_back,
        .pop_front = doubly_interface_pop_front,
        .pop_back = doubly_interface_pop_back,
        .size = doubly_interface_size,
        .is_empty = doubly_interface_is_empty,
        .clear = doubly_interface_clear,
        .free_list = doubly_interface_free_list},
    .get_at = doubly_interface_get_at,
    .set_at = doubly_interface_set_at,
    .insert_at = doubly_interface_insert_at,
    .remove_at = doubly_interface_remove_at,
    .contains = doubly_interface_contains,
    .reverse = doubly_interface_reverse};

/**
 * Initialize with interface support
 * @param list: List to initialize
 */
static inline void doubly_init_with_interface(DoublyLinkedList *list)
{
    doubly_init(list);
    list->vtable = &doubly_vtable;
}

#endif // DOUBLY_LINKED_LIST_H