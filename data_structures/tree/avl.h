#ifndef AVL_H
#define AVL_H

#include "tree.h"

// AVL Tree uses the basic tree structure but with TreeNode renamed to AVLNode for clarity
typedef TreeNode AVLNode;
typedef Tree AVLTree;

// ==================== AVL-SPECIFIC FUNCTIONS ====================

// Calculate balance factor of a node
static inline int avl_balance_factor(AVLNode *node)
{
    return node ? tree_height(node->left) - tree_height(node->right) : 0;
}

// Right rotate
static inline AVLNode *avl_rotate_right(AVLNode *y)
{
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    tree_update_height(y);
    tree_update_height(x);

    return x; // New root
}

// Left rotate
static inline AVLNode *avl_rotate_left(AVLNode *x)
{
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    tree_update_height(x);
    tree_update_height(y);

    return y; // New root
}

// Insert a node with AVL balancing
static inline AVLNode *avl_insert_node(AVLNode *node, int data)
{
    // 1. Perform normal BST insertion
    if (node == NULL)
    {
        return tree_create_node(data);
    }

    if (data < node->data)
    {
        node->left = avl_insert_node(node->left, data);
    }
    else if (data > node->data)
    {
        node->right = avl_insert_node(node->right, data);
    }
    else
    {
        // Equal keys not allowed
        return node;
    }

    // 2. Update height of this ancestor node
    tree_update_height(node);

    // 3. Get the balance factor
    int balance = avl_balance_factor(node);

    // 4. If unbalanced, there are 4 cases:

    // Left Left Case
    if (balance > 1 && data < node->left->data)
    {
        return avl_rotate_right(node);
    }

    // Right Right Case
    if (balance < -1 && data > node->right->data)
    {
        return avl_rotate_left(node);
    }

    // Left Right Case
    if (balance > 1 && data > node->left->data)
    {
        node->left = avl_rotate_left(node->left);
        return avl_rotate_right(node);
    }

    // Right Left Case
    if (balance < -1 && data < node->right->data)
    {
        node->right = avl_rotate_right(node->right);
        return avl_rotate_left(node);
    }

    // Return the unchanged node pointer
    return node;
}

// Delete a node with AVL balancing
static inline AVLNode *avl_delete_node(AVLNode *root, int data)
{
    // 1. Perform standard BST delete
    if (root == NULL)
    {
        return root;
    }

    if (data < root->data)
    {
        root->left = avl_delete_node(root->left, data);
    }
    else if (data > root->data)
    {
        root->right = avl_delete_node(root->right, data);
    }
    else
    {
        // Node to be deleted found
        if (root->left == NULL || root->right == NULL)
        {
            AVLNode *temp = root->left ? root->left : root->right;

            if (temp == NULL)
            {
                // No child case
                temp = root;
                root = NULL;
            }
            else
            {
                // One child case
                *root = *temp; // Copy the contents of the non-empty child
            }
            free(temp);
        }
        else
        {
            // Node with two children
            AVLNode *temp = tree_find_min(root->right);

            // Copy the inorder successor's data to this node
            root->data = temp->data;

            // Delete the inorder successor
            root->right = avl_delete_node(root->right, temp->data);
        }
    }

    // If the tree had only one node then return
    if (root == NULL)
    {
        return root;
    }

    // 2. Update height of the current node
    tree_update_height(root);

    // 3. Get the balance factor
    int balance = avl_balance_factor(root);

    // 4. If unbalanced, there are 4 cases:

    // Left Left Case
    if (balance > 1 && avl_balance_factor(root->left) >= 0)
    {
        return avl_rotate_right(root);
    }

    // Left Right Case
    if (balance > 1 && avl_balance_factor(root->left) < 0)
    {
        root->left = avl_rotate_left(root->left);
        return avl_rotate_right(root);
    }

    // Right Right Case
    if (balance < -1 && avl_balance_factor(root->right) <= 0)
    {
        return avl_rotate_left(root);
    }

    // Right Left Case
    if (balance < -1 && avl_balance_factor(root->right) > 0)
    {
        root->right = avl_rotate_right(root->right);
        return avl_rotate_left(root);
    }

    return root;
}

// ==================== AVL TREE FUNCTIONS ====================

// Initialize an empty AVL tree
static inline void avl_init(AVLTree *tree)
{
    tree_init(tree);
}

// Insert a value into the AVL tree
static inline void avl_insert(AVLTree *tree, int data)
{
    AVLNode *old_root = tree->root;
    tree->root = avl_insert_node(tree->root, data);

    // Only increment size if a new node was actually added
    if (tree->root != old_root || tree->size == 0)
    {
        tree->size++;
    }
}

// Delete a value from the AVL tree
static inline bool avl_delete(AVLTree *tree, int data)
{
    AVLNode *found = tree_search_node(tree->root, data);
    if (!found)
    {
        return false; // Value not found
    }

    tree->root = avl_delete_node(tree->root, data);
    tree->size--;
    return true;
}

// Search for a value in the AVL tree
static inline bool avl_search(AVLTree *tree, int data)
{
    return tree_search(tree, data);
}

// Get the size of the AVL tree
static inline size_t avl_size(AVLTree *tree)
{
    return tree_size(tree);
}

// Check if the AVL tree is empty
static inline bool avl_is_empty(AVLTree *tree)
{
    return tree_is_empty(tree);
}

// Free the entire AVL tree
static inline void avl_free(AVLTree *tree)
{
    tree_free(tree);
}

// ==================== AVL TRAVERSAL FUNCTIONS ====================

// Inorder traversal (prints in sorted order)
static inline void avl_inorder_traversal(AVLNode *root)
{
    tree_inorder_traversal(root);
}

// Preorder traversal
static inline void avl_preorder_traversal(AVLNode *root)
{
    tree_preorder_traversal(root);
}

// Postorder traversal
static inline void avl_postorder_traversal(AVLNode *root)
{
    tree_postorder_traversal(root);
}

// ==================== AVL UTILITY DISPLAY FUNCTIONS ====================

// Print tree structure with balance factors (for debugging)
static inline void avl_print_tree_structure(AVLNode *root, int space)
{
    const int COUNT = 10;
    if (root == NULL)
        return;

    space += COUNT;

    avl_print_tree_structure(root->right, space);

    printf("\n");
    for (int i = COUNT; i < space; i++)
    {
        printf(" ");
    }
    printf("%d(h:%d,bf:%d)\n", root->data, root->height, avl_balance_factor(root));

    avl_print_tree_structure(root->left, space);
}

// Print AVL tree information
static inline void avl_print_info(AVLTree *tree)
{
    printf("AVL Tree - Size: %zu, Root height: %d\n",
           tree->size, tree_height(tree->root));

    if (tree->root)
    {
        printf("Root: %d (Balance factor: %d)\n",
               tree->root->data, avl_balance_factor(tree->root));
    }
    else
    {
        printf("Tree is empty\n");
    }
}

// Validate AVL tree properties (for testing)
static inline bool avl_is_valid_helper(AVLNode *root, int *min_val, int *max_val)
{
    if (root == NULL)
    {
        return true;
    }

    // Check BST property
    if ((min_val && root->data <= *min_val) ||
        (max_val && root->data >= *max_val))
    {
        return false;
    }

    // Check AVL property (balance factor)
    int balance = avl_balance_factor(root);
    if (balance < -1 || balance > 1)
    {
        return false;
    }

    // Check height property
    int expected_height = 1 + tree_max(tree_height(root->left), tree_height(root->right));
    if (root->height != expected_height)
    {
        return false;
    }

    // Recursively validate subtrees
    return avl_is_valid_helper(root->left, min_val, &root->data) &&
           avl_is_valid_helper(root->right, &root->data, max_val);
}

// Check if the tree is a valid AVL tree
static inline bool avl_is_valid(AVLTree *tree)
{
    return avl_is_valid_helper(tree->root, NULL, NULL);
}

#endif // AVL_H