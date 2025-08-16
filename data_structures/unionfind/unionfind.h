#ifndef UNIONFIND_H
#define UNIONFIND_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

/**
 * UNION-FIND (DISJOINT SET UNION) IMPLEMENTATION
 *
 * Efficient data structure for maintaining disjoint sets with union and find operations.
 * Uses path compression and union by rank optimizations for nearly constant time operations.
 *
 * Time Complexities (with optimizations):
 * - Find: O(α(n)) - inverse Ackermann function (practically constant)
 * - Union: O(α(n)) - inverse Ackermann function (practically constant)
 * - Connected: O(α(n)) - two find operations
 *
 * Space Complexity: O(n) where n is number of elements
 *
 * Use Cases:
 * - Kruskal's minimum spanning tree algorithm
 * - Dynamic connectivity problems
 * - Image segmentation
 * - Social network analysis (connected components)
 * - Percolation problems
 * - Cycle detection in graphs
 */

// Union-Find structure
typedef struct UnionFind
{
    int *parent;    // Parent array for tree structure
    int *rank;      // Rank array for union by rank optimization
    int *size;      // Size of each component (optional)
    int n;          // Total number of elements
    int components; // Number of disjoint components
} UnionFind;

// ==================== CORE OPERATIONS ====================

/**
 * Initialize Union-Find structure
 * @param uf: UnionFind structure to initialize
 * @param n: Number of elements (0 to n-1)
 */
static inline void unionfind_init(UnionFind *uf, int n)
{
    uf->n = n;
    uf->components = n; // Initially each element is its own component

    // Allocate arrays
    uf->parent = (int *)malloc(n * sizeof(int));
    uf->rank = (int *)malloc(n * sizeof(int));
    uf->size = (int *)malloc(n * sizeof(int));

    if (!uf->parent || !uf->rank || !uf->size)
    {
        fprintf(stderr, "unionfind_init: allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Initialize: each element is its own parent with rank 0 and size 1
    for (int i = 0; i < n; i++)
    {
        uf->parent[i] = i;
        uf->rank[i] = 0;
        uf->size[i] = 1;
    }
}

/**
 * Find root of element with path compression
 * @param uf: UnionFind structure
 * @param x: Element to find root of
 * @return: Root of the set containing x
 */
static inline int unionfind_find(UnionFind *uf, int x)
{
    if (x < 0 || x >= uf->n)
        return -1; // Invalid element

    // Path compression: make every node point directly to root
    if (uf->parent[x] != x)
    {
        uf->parent[x] = unionfind_find(uf, uf->parent[x]);
    }
    return uf->parent[x];
}

/**
 * Union two sets containing elements x and y
 * Uses union by rank optimization
 * @param uf: UnionFind structure
 * @param x: First element
 * @param y: Second element
 * @return: true if union was performed (elements were in different sets)
 */
static inline bool unionfind_union(UnionFind *uf, int x, int y)
{
    int root_x = unionfind_find(uf, x);
    int root_y = unionfind_find(uf, y);

    if (root_x == -1 || root_y == -1)
        return false; // Invalid elements
    if (root_x == root_y)
        return false; // Already in same set

    // Union by rank: attach smaller tree under larger tree
    if (uf->rank[root_x] < uf->rank[root_y])
    {
        uf->parent[root_x] = root_y;
        uf->size[root_y] += uf->size[root_x];
    }
    else if (uf->rank[root_x] > uf->rank[root_y])
    {
        uf->parent[root_y] = root_x;
        uf->size[root_x] += uf->size[root_y];
    }
    else
    {
        // Same rank: make one root and increase its rank
        uf->parent[root_y] = root_x;
        uf->size[root_x] += uf->size[root_y];
        uf->rank[root_x]++;
    }

    uf->components--; // One less component after union
    return true;
}

/**
 * Check if two elements are in the same set (connected)
 * @param uf: UnionFind structure
 * @param x: First element
 * @param y: Second element
 * @return: true if x and y are in the same set
 */
static inline bool unionfind_connected(UnionFind *uf, int x, int y)
{
    return unionfind_find(uf, x) == unionfind_find(uf, y);
}

/**
 * Get size of the set containing element x
 * @param uf: UnionFind structure
 * @param x: Element to query
 * @return: Size of set containing x, -1 if invalid element
 */
static inline int unionfind_size_of(UnionFind *uf, int x)
{
    int root = unionfind_find(uf, x);
    return root == -1 ? -1 : uf->size[root];
}

/**
 * Get number of disjoint components
 * @param uf: UnionFind structure
 * @return: Number of separate components
 */
static inline int unionfind_components(UnionFind *uf)
{
    return uf->components;
}

/**
 * Get total number of elements
 * @param uf: UnionFind structure
 * @return: Total number of elements
 */
static inline int unionfind_count(UnionFind *uf)
{
    return uf->n;
}

/**
 * Check if element is a root (representative of its set)
 * @param uf: UnionFind structure
 * @param x: Element to check
 * @return: true if x is a root
 */
static inline bool unionfind_is_root(UnionFind *uf, int x)
{
    if (x < 0 || x >= uf->n)
        return false;
    return uf->parent[x] == x;
}

// ==================== UTILITY OPERATIONS ====================

/**
 * Get all root elements (representatives of each component)
 * @param uf: UnionFind structure
 * @param roots: Array to store roots (caller must allocate)
 * @return: Number of roots found
 */
static inline int unionfind_get_roots(UnionFind *uf, int *roots)
{
    int count = 0;
    for (int i = 0; i < uf->n; i++)
    {
        if (unionfind_is_root(uf, i))
        {
            roots[count++] = i;
        }
    }
    return count;
}

/**
 * Get all elements in the same component as x
 * @param uf: UnionFind structure
 * @param x: Element whose component to find
 * @param component: Array to store component elements (caller must allocate)
 * @return: Number of elements in component, -1 if invalid x
 */
static inline int unionfind_get_component(UnionFind *uf, int x, int *component)
{
    int root = unionfind_find(uf, x);
    if (root == -1)
        return -1;

    int count = 0;
    for (int i = 0; i < uf->n; i++)
    {
        if (unionfind_find(uf, i) == root)
        {
            component[count++] = i;
        }
    }
    return count;
}

/**
 * Reset Union-Find to initial state (all elements separate)
 * @param uf: UnionFind structure
 */
static inline void unionfind_reset(UnionFind *uf)
{
    uf->components = uf->n;

    for (int i = 0; i < uf->n; i++)
    {
        uf->parent[i] = i;
        uf->rank[i] = 0;
        uf->size[i] = 1;
    }
}

/**
 * Free Union-Find memory
 * @param uf: UnionFind structure
 */
static inline void unionfind_free(UnionFind *uf)
{
    if (uf->parent)
    {
        free(uf->parent);
        uf->parent = NULL;
    }
    if (uf->rank)
    {
        free(uf->rank);
        uf->rank = NULL;
    }
    if (uf->size)
    {
        free(uf->size);
        uf->size = NULL;
    }
    uf->n = 0;
    uf->components = 0;
}

// ==================== DEBUGGING AND ANALYSIS ====================

/**
 * Print Union-Find structure for debugging
 * @param uf: UnionFind structure
 */
static inline void unionfind_print(UnionFind *uf)
{
    printf("Union-Find Structure:\n");
    printf("  Elements: %d, Components: %d\n", uf->n, uf->components);

    printf("  Element:  ");
    for (int i = 0; i < uf->n; i++)
    {
        printf("%3d ", i);
    }
    printf("\n");

    printf("  Parent:   ");
    for (int i = 0; i < uf->n; i++)
    {
        printf("%3d ", uf->parent[i]);
    }
    printf("\n");

    printf("  Rank:     ");
    for (int i = 0; i < uf->n; i++)
    {
        printf("%3d ", uf->rank[i]);
    }
    printf("\n");

    printf("  Size:     ");
    for (int i = 0; i < uf->n; i++)
    {
        printf("%3d ", uf->size[i]);
    }
    printf("\n");
}

/**
 * Print all components
 * @param uf: UnionFind structure
 */
static inline void unionfind_print_components(UnionFind *uf)
{
    printf("Components (%d total):\n", uf->components);

    int *roots = (int *)malloc(uf->n * sizeof(int));
    int *component = (int *)malloc(uf->n * sizeof(int));

    if (!roots || !component)
    {
        free(roots);
        free(component);
        return;
    }

    int num_roots = unionfind_get_roots(uf, roots);

    for (int i = 0; i < num_roots; i++)
    {
        int root = roots[i];
        int comp_size = unionfind_get_component(uf, root, component);

        printf("  Component %d (size %d): { ", i + 1, comp_size);
        for (int j = 0; j < comp_size; j++)
        {
            printf("%d", component[j]);
            if (j < comp_size - 1)
                printf(", ");
        }
        printf(" }\n");
    }

    free(roots);
    free(component);
}

/**
 * Validate Union-Find structure integrity
 * @param uf: UnionFind structure
 * @return: true if structure is valid
 */
static inline bool unionfind_validate(UnionFind *uf)
{
    // Check that all elements are valid
    for (int i = 0; i < uf->n; i++)
    {
        if (uf->parent[i] < 0 || uf->parent[i] >= uf->n)
        {
            return false; // Invalid parent
        }
        if (uf->rank[i] < 0)
        {
            return false; // Invalid rank
        }
        if (uf->size[i] < 1)
        {
            return false; // Invalid size
        }
    }

    // Check that component count is correct
    int actual_components = 0;
    for (int i = 0; i < uf->n; i++)
    {
        if (unionfind_is_root(uf, i))
        {
            actual_components++;
        }
    }

    return actual_components == uf->components;
}

// ==================== SPECIALIZED OPERATIONS ====================

/**
 * @brief
 *
 *
 * @param uf: UnionFind structure
 * @param elements: Array of elements to union
 * @param count: Number of elements
 * @return: Number of unions performed
 */
static inline int unionfind_union_all(UnionFind *uf, int *elements, int count)
{
    if (count < 2)
        return 0;

    int unions_performed = 0;
    for (int i = 1; i < count; i++)
    {
        if (unionfind_union(uf, elements[0], elements[i]))
        {
            unions_performed++;
        }
    }
    return unions_performed;
}

/**
 * Check if all elements are connected (single component)
 * @param uf: UnionFind structure
 * @return: true if all elements are in one component
 */
static inline bool unionfind_all_connected(UnionFind *uf)
{
    return uf->components == 1;
}

/**
 * Get largest component size
 * @param uf: UnionFind structure
 * @return: Size of largest component
 */
static inline int unionfind_max_component_size(UnionFind *uf)
{
    int max_size = 0;
    for (int i = 0; i < uf->n; i++)
    {
        if (unionfind_is_root(uf, i) && uf->size[i] > max_size)
        {
            max_size = uf->size[i];
        }
    }
    return max_size;
}

// ==================== CONVENIENCE MACROS ====================

#define UNIONFIND_INIT(uf, n) unionfind_init(&(uf), (n))
#define UNIONFIND_UNION(uf, x, y) unionfind_union(&(uf), (x), (y))
#define UNIONFIND_FIND(uf, x) unionfind_find(&(uf), (x))
#define UNIONFIND_CONNECTED(uf, x, y) unionfind_connected(&(uf), (x), (y))
#define UNIONFIND_FREE(uf) unionfind_free(&(uf))

#endif // UNIONFIND_H