#ifndef SINGLY_LINKED_LIST_H
#define SINGLY_LINKED_LIST_H

#include "list_interface.h"
#include <stdlib.h>
#include <stdbool.h>

/**
 * SINGLY LINKED LIST IMPLEMENTATION
 *
 * A basic linked list where each node points to the next node.
 * Most memory-efficient linked list but only supports forward traversal.
 *
 * Time Complexities:
 * - Push front: O(1)
 * - Push back: O(1) with tail pointer
 * - Pop front: O(1)
 * - Pop back: O(n) - must traverse to find second-to-last
 * - Search: O(n)
 *
 * Space Complexity: O(n)
 */

// Node structure
typedef struct SinglyListNode
{
    void *data;                  // Stored data
    struct SinglyListNode *next; // Next node
} SinglyListNode;

// List structure
typedef struct SinglyLinkedList
{
    SinglyListNode *head;        // First node
    SinglyListNode *tail;        // Last node (for O(1) push_back)
    size_t size;                 // Element count
    const ListInterface *vtable; // Interface for polymorphism
} SinglyLinkedList;

// ==================== NODE OPERATIONS ====================

/**
 * Create new node
 * @param data: Data to store
 * @return: New node or NULL on failure
 */
static inline SinglyListNode *singly_create_node(void *data)
{
    SinglyListNode *node = (SinglyListNode *)malloc(sizeof(SinglyListNode));
    if (!node)
        return NULL;

    node->data = data;
    node->next = NULL;
    return node;
}

/**
 * Free node
 * @param node: Node to free
 */
static inline void singly_free_node(SinglyListNode *node)
{
    if (node)
        free(node);
}

// ==================== CORE OPERATIONS ====================

/**
 * Initialize empty list
 * @param list: List to initialize
 */
static inline void singly_init(SinglyLinkedList *list)
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
static inline void singly_push_front(SinglyLinkedList *list, void *data)
{
    SinglyListNode *node = singly_create_node(data);
    if (!node)
        return;

    if (!list->head)
    {
        list->head = list->tail = node;
    }
    else
    {
        node->next = list->head;
        list->head = node;
    }
    list->size++;
}

/**
 * Add element to back
 * @param list: Target list
 * @param data: Data to add
 */
static inline void singly_push_back(SinglyLinkedList *list, void *data)
{
    SinglyListNode *node = singly_create_node(data);
    if (!node)
        return;

    if (!list->tail)
    {
        list->head = list->tail = node;
    }
    else
    {
        list->tail->next = node;
        list->tail = node;
    }
    list->size++;
}

/**
 * Remove element from front
 * @param list: Target list
 * @return: Data from removed element, NULL if empty
 */
static inline void *singly_pop_front(SinglyLinkedList *list)
{
    if (!list->head)
        return NULL;

    SinglyListNode *node = list->head;
    void *data = node->data;

    list->head = node->next;
    if (!list->head)
        list->tail = NULL;

    singly_free_node(node);
    list->size--;
    return data;
}

/**
 * Remove element from back
 * @param list: Target list
 * @return: Data from removed element, NULL if empty
 */
static inline void *singly_pop_back(SinglyLinkedList *list)
{
    if (!list->head)
        return NULL;

    if (list->head == list->tail)
    {
        // Only one element
        void *data = list->head->data;
        singly_free_node(list->head);
        list->head = list->tail = NULL;
        list->size--;
        return data;
    }

    // Find second-to-last node
    SinglyListNode *current = list->head;
    while (current->next != list->tail)
    {
        current = current->next;
    }

    void *data = list->tail->data;
    singly_free_node(list->tail);
    list->tail = current;
    current->next = NULL;
    list->size--;
    return data;
}

/**
 * Get element at index
 * @param list: Target list
 * @param index: Index to access
 * @return: Data at index, NULL if out of bounds
 */
static inline void *singly_get_at(SinglyLinkedList *list, size_t index)
{
    if (index >= list->size)
        return NULL;

    SinglyListNode *current = list->head;
    for (size_t i = 0; i < index; i++)
    {
        current = current->next;
    }
    return current->data;
}

/**
 * Insert at index
 * @param list: Target list
 * @param index: Insertion index
 * @param data: Data to insert
 */
static inline void singly_insert_at(SinglyLinkedList *list, size_t index, void *data)
{
    if (index > list->size)
        return;

    if (index == 0)
    {
        singly_push_front(list, data);
        return;
    }
    if (index == list->size)
    {
        singly_push_back(list, data);
        return;
    }

    SinglyListNode *current = list->head;
    for (size_t i = 0; i < index - 1; i++)
    {
        current = current->next;
    }

    SinglyListNode *new_node = singly_create_node(data);
    if (!new_node)
        return;

    new_node->next = current->next;
    current->next = new_node;
    list->size++;
}

/**
 * Search for element
 * @param list: Target list
 * @param data: Data to find
 * @return: true if found
 */
static inline bool singly_contains(SinglyLinkedList *list, void *data)
{
    SinglyListNode *current = list->head;
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
static inline size_t singly_size(SinglyLinkedList *list)
{
    return list->size;
}

/**
 * Check if empty
 * @param list: Target list
 * @return: true if empty
 */
static inline bool singly_is_empty(SinglyLinkedList *list)
{
    return list->size == 0;
}

/**
 * Clear all elements
 * @param list: Target list
 */
static inline void singly_clear(SinglyLinkedList *list)
{
    while (list->head)
    {
        SinglyListNode *next = list->head->next;
        singly_free_node(list->head);
        list->head = next;
    }
    list->tail = NULL;
    list->size = 0;
}

/**
 * Free list
 * @param list: Target list
 */
static inline void singly_free(SinglyLinkedList *list)
{
    singly_clear(list);
}

/**
 * Reverse list in place
 * @param list: Target list
 */
static inline void singly_reverse(SinglyLinkedList *list)
{
    if (list->size <= 1)
        return;

    SinglyListNode *prev = NULL;
    SinglyListNode *current = list->head;
    list->tail = list->head; // Old head becomes new tail

    while (current)
    {
        SinglyListNode *next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }

    list->head = prev; // Last node becomes new head
}

// ==================== INTERFACE IMPLEMENTATION ====================

static void singly_interface_push_front(void *list, void *data)
{
    singly_push_front((SinglyLinkedList *)list, data);
}

static void singly_interface_push_back(void *list, void *data)
{
    singly_push_back((SinglyLinkedList *)list, data);
}

static void *singly_interface_pop_front(void *list)
{
    return singly_pop_front((SinglyLinkedList *)list);
}

static void *singly_interface_pop_back(void *list)
{
    return singly_pop_back((SinglyLinkedList *)list);
}

static size_t singly_interface_size(void *list)
{
    return singly_size((SinglyLinkedList *)list);
}

static bool singly_interface_is_empty(void *list)
{
    return singly_is_empty((SinglyLinkedList *)list);
}

static void singly_interface_clear(void *list)
{
    singly_clear((SinglyLinkedList *)list);
}

static void singly_interface_free_list(void *list)
{
    singly_free((SinglyLinkedList *)list);
}

static const ListInterface singly_vtable = {
    .push_front = singly_interface_push_front,
    .push_back = singly_interface_push_back,
    .pop_front = singly_interface_pop_front,
    .pop_back = singly_interface_pop_back,
    .size = singly_interface_size,
    .is_empty = singly_interface_is_empty,
    .clear = singly_interface_clear,
    .free_list = singly_interface_free_list};

/**
 * Initialize with interface support
 * @param list: List to initialize
 */
static inline void singly_init_with_interface(SinglyLinkedList *list)
{
    singly_init(list);
    list->vtable = &singly_vtable;
}

#endif // SINGLY_LINKED_LIST_H