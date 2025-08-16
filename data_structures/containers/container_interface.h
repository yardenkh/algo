#ifndef CONTAINER_INTERFACE_H
#define CONTAINER_INTERFACE_H

#include <stdlib.h>
#include <stdbool.h>

/**
 * CONTAINER INTERFACE
 *
 * Common interface for all container types (stacks, queues, deques, etc.)
 * following SOLID principles for clean, maintainable code.
 */

// Basic container operations that most containers support
typedef struct ContainerInterface
{
    void (*push)(void *container, void *data); // Add element
    void *(*pop)(void *container);             // Remove and return element
    void *(*peek)(void *container);            // View element without removing
    size_t (*size)(void *container);           // Get current size
    bool (*is_empty)(void *container);         // Check if empty
    void (*clear)(void *container);            // Remove all elements
    void (*free_container)(void *container);   // Free memory
} ContainerInterface;

// Extended interface for containers supporting both ends
typedef struct DequeInterface
{
    ContainerInterface base; // Inherit basic operations

    void (*push_front)(void *container, void *data); // Add to front
    void (*push_back)(void *container, void *data);  // Add to back
    void *(*pop_front)(void *container);             // Remove from front
    void *(*pop_back)(void *container);              // Remove from back
    void *(*peek_front)(void *container);            // View front
    void *(*peek_back)(void *container);             // View back
} DequeInterface;

// Interface for containers supporting indexed access
typedef struct IndexedContainerInterface
{
    ContainerInterface base; // Inherit basic operations

    void *(*get_at)(void *container, size_t index);               // Get element at index
    bool (*set_at)(void *container, size_t index, void *data);    // Set element at index
    bool (*insert_at)(void *container, size_t index, void *data); // Insert at index
    void *(*remove_at)(void *container, size_t index);            // Remove at index
    bool (*contains)(void *container, void *data);                // Search for element
} IndexedContainerInterface;

#endif // CONTAINER_INTERFACE_H