#ifndef LIST_INTERFACE_H
#define LIST_INTERFACE_H

#include <stdlib.h>
#include <stdbool.h>

/**
 * LIST INTERFACE
 *
 * Common interface for all linked list variants following SOLID principles.
 * Provides different interface levels for different capabilities.
 */

// Forward declarations
typedef struct SinglyLinkedList SinglyLinkedList;
typedef struct DoublyLinkedList DoublyLinkedList;
typedef struct CircularLinkedList CircularLinkedList;

// Basic list interface - operations all lists must support
typedef struct ListInterface
{
    void (*push_front)(void *list, void *data); // Add to front O(1)
    void (*push_back)(void *list, void *data);  // Add to back
    void *(*pop_front)(void *list);             // Remove from front O(1)
    void *(*pop_back)(void *list);              // Remove from back
    size_t (*size)(void *list);                 // Get size O(1)
    bool (*is_empty)(void *list);               // Check if empty O(1)
    void (*clear)(void *list);                  // Remove all elements
    void (*free_list)(void *list);              // Free memory
} ListInterface;

// Extended interface for lists supporting indexed operations
typedef struct ExtendedListInterface
{
    ListInterface base; // Inherit basic operations

    void *(*get_at)(void *list, size_t index);               // Get element at index
    void (*set_at)(void *list, size_t index, void *data);    // Set element at index
    void (*insert_at)(void *list, size_t index, void *data); // Insert at index
    void *(*remove_at)(void *list, size_t index);            // Remove at index
    bool (*contains)(void *list, void *data);                // Search for element
    void (*reverse)(void *list);                             // Reverse list
} ExtendedListInterface;

// Specialized interface for circular lists
typedef struct CircularListInterface
{
    ListInterface base; // Inherit basic operations

    void (*rotate_forward)(void *list, size_t steps);  // Rotate forward
    void (*rotate_backward)(void *list, size_t steps); // Rotate backward
    void *(*current)(void *list);                      // Get current element
    void (*advance)(void *list);                       // Move to next
    bool (*is_circular)(void *list);                   // Verify circular property
} CircularListInterface;

#endif // LIST_INTERFACE_H