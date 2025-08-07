#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * BINARY SEARCH TREE IMPLEMENTATION
 *
 * Basic binary search tree with standard BST properties:
 * - Left subtree values < parent value
 * - Right subtree values > parent value
 * - No duplicate values allowed
 *
 * Time Complexities:
 * - Search: O(log n) average, O(n) worst case
 * - Insert: O(log n) average, O(n) worst case
 * - Delete: O(log n) average, O(n) worst case
 *
 * Space Complexity: O(n)
 *
 * Note: This is an unbalanced tree. Use AVL tree for guaranteed O(log n) operations.
 */

// Tree node structure
typedef struct TreeNode
{
    int data;               // Node value
    int height;             // Height for balanced trees
    struct TreeNode *left;  // Left child
    struct TreeNode *right; // Right child
} TreeNode;

// Tree structure
typedef struct Tree
{
    TreeNode *root; // Root node
    size_t size;    // Number of nodes
} Tree;

// ==================== NODE OPERATIONS ====================

/**
 * Create new tree node
 * @param data: Value to store
 * @return: New node or NULL on failure
 */
static inline TreeNode *tree_create_node(int data)
{
    TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
    if (!node)
    {
        fprintf(stderr, "tree_create_node: allocation failed\n");
        exit(EXIT_FAILURE);
    }

    node->data = data;
    node->height = 0;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/**
 * Get height of node (-1 for NULL nodes)
 * @param node: Target node
 * @return: Height of node
 */
static inline int tree_height(TreeNode *node)
{
    return node ? node->height : -1;
}

/**
 * Get maximum of two integers
 * @param a, b: Values to compare
 * @return: Maximum value
 */
static inline int tree_max(int a, int b)
{
    return (a > b) ? a : b;
}

/**
 * Update height of node based on children
 * @param node: Node to update
 */
static inline void tree_update_height(TreeNode *node)
{
    if (node)
    {
        node->height = 1 + tree_max(tree_height(node->left), tree_height(node->right));
    }
}

/**
 * Find node with minimum value in subtree
 * @param node: Root of subtree
 * @return: Node with minimum value
 */
static inline TreeNode *tree_find_min(TreeNode *node)
{
    while (node && node->left)
    {
        node = node->left;
    }
    return node;
}

/**
 * Find node with maximum value in subtree
 * @param node: Root of subtree
 * @return: Node with maximum value
 */
static inline TreeNode *tree_find_max(TreeNode *node)
{
    while (node && node->right)
    {
        node = node->right;
    }
    return node;
}

/**
 * Search for value in subtree
 * @param root: Root of subtree
 * @param data: Value to search for
 * @return: Node containing value or NULL if not found
 */
static inline TreeNode *tree_search_node(TreeNode *root, int data)
{
    if (!root || root->data == data)
    {
        return root;
    }

    if (data < root->data)
    {
        return tree_search_node(root->left, data);
    }

    return tree_search_node(root->right, data);
}

/**
 * Free all nodes in subtree
 * @param root: Root of subtree to free
 */
static inline void tree_free_nodes(TreeNode *root)
{
    if (root)
    {
        tree_free_nodes(root->left);
        tree_free_nodes(root->right);
        free(root);
    }
}

// ==================== BST OPERATIONS ====================

/**
 * Insert value into BST subtree
 * @param node: Root of subtree
 * @param data: Value to insert
 * @return: New root of subtree
 */
static inline TreeNode *tree_insert_node(TreeNode *node, int data)
{
    if (!node)
    {
        return tree_create_node(data);
    }

    if (data < node->data)
    {
        node->left = tree_insert_node(node->left, data);
    }
    else if (data > node->data)
    {
        node->right = tree_insert_node(node->right, data);
    }
    // Equal keys not allowed - return unchanged

    tree_update_height(node);
    return node;
}

/**
 * Delete value from BST subtree
 * @param root: Root of subtree
 * @param data: Value to delete
 * @return: New root of subtree
 */
static inline TreeNode *tree_delete_node(TreeNode *root, int data)
{
    if (!root)
        return root;

    if (data < root->data)
    {
        root->left = tree_delete_node(root->left, data);
    }
    else if (data > root->data)
    {
        root->right = tree_delete_node(root->right, data);
    }
    else
    {
        // Node to delete found
        if (!root->left)
        {
            TreeNode *temp = root->right;
            free(root);
            return temp;
        }
        else if (!root->right)
        {
            TreeNode *temp = root->left;
            free(root);
            return temp;
        }

        // Node with two children - replace with inorder successor
        TreeNode *temp = tree_find_min(root->right);
        root->data = temp->data;
        root->right = tree_delete_node(root->right, temp->data);
    }

    tree_update_height(root);
    return root;
}

// ==================== TREE OPERATIONS ====================

/**
 * Initialize empty tree
 * @param tree: Tree to initialize
 */
static inline void tree_init(Tree *tree)
{
    tree->root = NULL;
    tree->size = 0;
}

/**
 * Insert value into tree
 * @param tree: Target tree
 * @param data: Value to insert
 */
static inline void tree_insert(Tree *tree, int data)
{
    TreeNode *old_root = tree->root;
    tree->root = tree_insert_node(tree->root, data);

    // Only increment size if new node was added
    if (tree->root != old_root || tree->size == 0)
    {
        tree->size++;
    }
}

/**
 * Delete value from tree
 * @param tree: Target tree
 * @param data: Value to delete
 * @return: true if value was found and deleted
 */
static inline bool tree_delete(Tree *tree, int data)
{
    TreeNode *found = tree_search_node(tree->root, data);
    if (!found)
        return false;

    tree->root = tree_delete_node(tree->root, data);
    tree->size--;
    return true;
}

/**
 * Search for value in tree
 * @param tree: Target tree
 * @param data: Value to search for
 * @return: true if found
 */
static inline bool tree_search(Tree *tree, int data)
{
    return tree_search_node(tree->root, data) != NULL;
}

/**
 * Get current size of tree
 * @param tree: Target tree
 * @return: Number of nodes
 */
static inline size_t tree_size(Tree *tree)
{
    return tree->size;
}

/**
 * Check if tree is empty
 * @param tree: Target tree
 * @return: true if empty
 */
static inline bool tree_is_empty(Tree *tree)
{
    return tree->size == 0;
}

/**
 * Free entire tree
 * @param tree: Tree to free
 */
static inline void tree_free(Tree *tree)
{
    tree_free_nodes(tree->root);
    tree->root = NULL;
    tree->size = 0;
}

// ==================== TRAVERSAL OPERATIONS ====================

/**
 * Inorder traversal (left, root, right) - prints values in sorted order
 * @param root: Root of subtree
 */
static inline void tree_inorder_traversal(TreeNode *root)
{
    if (root)
    {
        tree_inorder_traversal(root->left);
        printf("%d ", root->data);
        tree_inorder_traversal(root->right);
    }
}

/**
 * Preorder traversal (root, left, right)
 * @param root: Root of subtree
 */
static inline void tree_preorder_traversal(TreeNode *root)
{
    if (root)
    {
        printf("%d ", root->data);
        tree_preorder_traversal(root->left);
        tree_preorder_traversal(root->right);
    }
}

/**
 * Postorder traversal (left, right, root)
 * @param root: Root of subtree
 */
static inline void tree_postorder_traversal(TreeNode *root)
{
    if (root)
    {
        tree_postorder_traversal(root->left);
        tree_postorder_traversal(root->right);
        printf("%d ", root->data);
    }
}

// ==================== UTILITY FUNCTIONS ====================

/**
 * Get minimum value in tree
 * @param tree: Target tree
 * @return: Minimum value, returns 0 if tree is empty
 */
static inline int tree_get_min(Tree *tree)
{
    TreeNode *min_node = tree_find_min(tree->root);
    return min_node ? min_node->data : 0;
}

/**
 * Get maximum value in tree
 * @param tree: Target tree
 * @return: Maximum value, returns 0 if tree is empty
 */
static inline int tree_get_max(Tree *tree)
{
    TreeNode *max_node = tree_find_max(tree->root);
    return max_node ? max_node->data : 0;
}

/**
 * Get height of tree
 * @param tree: Target tree
 * @return: Height of tree (-1 if empty)
 */
static inline int tree_get_height(Tree *tree)
{
    return tree_height(tree->root);
}

/**
 * Check if tree is valid BST
 * @param root: Root node
 * @param min_val: Minimum allowed value (NULL for no limit)
 * @param max_val: Maximum allowed value (NULL for no limit)
 * @return: true if valid BST
 */
static inline bool tree_is_valid_bst_helper(TreeNode *root, int *min_val, int *max_val)
{
    if (!root)
        return true;

    // Check BST property
    if ((min_val && root->data <= *min_val) ||
        (max_val && root->data >= *max_val))
    {
        return false;
    }

    // Recursively validate subtrees
    return tree_is_valid_bst_helper(root->left, min_val, &root->data) &&
           tree_is_valid_bst_helper(root->right, &root->data, max_val);
}

/**
 * Check if tree is valid BST
 * @param tree: Target tree
 * @return: true if valid BST
 */
static inline bool tree_is_valid_bst(Tree *tree)
{
    return tree_is_valid_bst_helper(tree->root, NULL, NULL);
}

/**
 * Count nodes in subtree
 * @param root: Root of subtree
 * @return: Number of nodes
 */
static inline size_t tree_count_nodes(TreeNode *root)
{
    if (!root)
        return 0;
    return 1 + tree_count_nodes(root->left) + tree_count_nodes(root->right);
}

/**
 * Print tree structure for debugging (rotated 90 degrees)
 * @param root: Root of subtree
 * @param space: Current indentation
 */
static inline void tree_print_structure(TreeNode *root, int space)
{
    const int COUNT = 10;
    if (!root)
        return;

    space += COUNT;

    tree_print_structure(root->right, space);

    printf("\n");
    for (int i = COUNT; i < space; i++)
    {
        printf(" ");
    }
    printf("%d(h:%d)\n", root->data, root->height);

    tree_print_structure(root->left, space);
}

/**
 * Print tree information
 * @param tree: Target tree
 */
static inline void tree_print_info(Tree *tree)
{
    printf("Binary Tree - Size: %zu, Height: %d\n",
           tree->size, tree_get_height(tree));

    if (tree->root)
    {
        printf("Root: %d, Min: %d, Max: %d\n",
               tree->root->data, tree_get_min(tree), tree_get_max(tree));
        printf("Valid BST: %s\n", tree_is_valid_bst(tree) ? "Yes" : "No");
    }
    else
    {
        printf("Tree is empty\n");
    }
}

/**
 * Create tree from sorted array (creates balanced tree)
 * @param arr: Sorted array of values
 * @param start: Start index
 * @param end: End index
 * @return: Root of balanced subtree
 */
static inline TreeNode *tree_from_sorted_array_helper(int *arr, int start, int end)
{
    if (start > end)
        return NULL;

    int mid = start + (end - start) / 2;
    TreeNode *root = tree_create_node(arr[mid]);

    root->left = tree_from_sorted_array_helper(arr, start, mid - 1);
    root->right = tree_from_sorted_array_helper(arr, mid + 1, end);

    tree_update_height(root);
    return root;
}

/**
 * Create balanced tree from sorted array
 * @param tree: Tree to initialize
 * @param arr: Sorted array of unique values
 * @param size: Array size
 */
static inline void tree_from_sorted_array(Tree *tree, int *arr, size_t size)
{
    tree_init(tree);
    if (size > 0)
    {
        tree->root = tree_from_sorted_array_helper(arr, 0, size - 1);
        tree->size = size;
    }
}

/**
 * Convert tree to sorted array (inorder traversal)
 * @param root: Root of subtree
 * @param arr: Array to fill
 * @param index: Current index in array
 */
static inline void tree_to_array_helper(TreeNode *root, int *arr, size_t *index)
{
    if (root)
    {
        tree_to_array_helper(root->left, arr, index);
        arr[(*index)++] = root->data;
        tree_to_array_helper(root->right, arr, index);
    }
}

/**
 * Convert tree to sorted array
 * @param tree: Source tree
 * @param arr: Destination array (must be large enough)
 * @return: Number of elements copied
 */
static inline size_t tree_to_array(Tree *tree, int *arr)
{
    size_t index = 0;
    tree_to_array_helper(tree->root, arr, &index);
    return index;
}

// ==================== CONVENIENCE FUNCTIONS ====================

/**
 * Insert multiple values into tree
 * @param tree: Target tree
 * @param values: Array of values to insert
 * @param count: Number of values
 */
static inline void tree_insert_array(Tree *tree, int *values, size_t count)
{
    for (size_t i = 0; i < count; i++)
    {
        tree_insert(tree, values[i]);
    }
}

/**
 * Print all values in tree (inorder)
 * @param tree: Target tree
 */
static inline void tree_print_values(Tree *tree)
{
    printf("Tree values (sorted): ");
    if (tree_is_empty(tree))
    {
        printf("(empty)");
    }
    else
    {
        tree_inorder_traversal(tree->root);
    }
    printf("\n");
}

#endif // TREE_H