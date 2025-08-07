#ifndef HASHSET_H
#define HASHSET_H

#include "hashtable.h"
#include <stdbool.h>

/**
 * HASH SET IMPLEMENTATION
 *
 * Set data structure built on top of hash table.
 * Stores unique keys with no associated values.
 *
 * Time Complexities:
 * - Insert: O(1) average
 * - Remove: O(1) average
 * - Contains: O(1) average
 *
 * Space Complexity: O(n)
 */

// Dummy value used for all set entries
static void *HASHSET_DUMMY_VALUE = (void *)1;

// Hash set is just a hash table with dummy values
typedef HashTable HashSet;

// ==================== CORE OPERATIONS ====================

/**
 * Initialize hash set
 * @param set: Set to initialize
 * @param initial_capacity: Starting capacity
 * @param hash_func: Hash function for keys
 */
static inline void hashset_init(HashSet *set, size_t initial_capacity, const HashFunction *hash_func)
{
    hashtable_init(set, initial_capacity, hash_func);
}

/**
 * Initialize string hash set
 * @param set: Set to initialize
 */
static inline void hashset_init_string(HashSet *set)
{
    hashtable_init_string(set);
}

/**
 * Initialize integer hash set
 * @param set: Set to initialize
 */
static inline void hashset_init_int(HashSet *set)
{
    hashtable_init_int(set);
}

/**
 * Add element to set
 * @param set: Target set
 * @param key: Element to add
 * @return: true if added (false if already exists)
 */
static inline bool hashset_add(HashSet *set, const void *key)
{
    bool existed = hashtable_contains(set, key);
    hashtable_put(set, key, HASHSET_DUMMY_VALUE);
    return !existed; // Return true if element was newly added
}

/**
 * Remove element from set
 * @param set: Target set
 * @param key: Element to remove
 * @return: true if removed (false if didn't exist)
 */
static inline bool hashset_remove(HashSet *set, const void *key)
{
    return hashtable_remove(set, key);
}

/**
 * Check if element exists in set
 * @param set: Target set
 * @param key: Element to check
 * @return: true if exists
 */
static inline bool hashset_contains(HashSet *set, const void *key)
{
    return hashtable_contains(set, key);
}

/**
 * Get current size
 * @param set: Target set
 * @return: Number of elements
 */
static inline size_t hashset_size(HashSet *set)
{
    return hashtable_size(set);
}

/**
 * Check if empty
 * @param set: Target set
 * @return: true if empty
 */
static inline bool hashset_is_empty(HashSet *set)
{
    return hashtable_is_empty(set);
}

/**
 * Clear all elements
 * @param set: Target set
 */
static inline void hashset_clear(HashSet *set)
{
    hashtable_clear(set);
}

/**
 * Free set memory
 * @param set: Target set
 */
static inline void hashset_free(HashSet *set)
{
    hashtable_free(set);
}

// ==================== SET OPERATIONS ====================

/**
 * Union of two sets (result = set1 ∪ set2)
 * @param set1: First set
 * @param set2: Second set
 * @param result: Resulting union set (should be initialized)
 */
static inline void hashset_union(HashSet *set1, HashSet *set2, HashSet *result)
{
    // Add all elements from set1
    for (size_t i = 0; i < set1->capacity; i++)
    {
        HashEntry *entry = set1->buckets[i];
        while (entry)
        {
            hashset_add(result, entry->key);
            entry = entry->next;
        }
    }

    // Add all elements from set2
    for (size_t i = 0; i < set2->capacity; i++)
    {
        HashEntry *entry = set2->buckets[i];
        while (entry)
        {
            hashset_add(result, entry->key);
            entry = entry->next;
        }
    }
}

/**
 * Intersection of two sets (result = set1 ∩ set2)
 * @param set1: First set
 * @param set2: Second set
 * @param result: Resulting intersection set (should be initialized)
 */
static inline void hashset_intersection(HashSet *set1, HashSet *set2, HashSet *result)
{
    // Use smaller set for iteration efficiency
    HashSet *smaller = hashset_size(set1) <= hashset_size(set2) ? set1 : set2;
    HashSet *larger = (smaller == set1) ? set2 : set1;

    for (size_t i = 0; i < smaller->capacity; i++)
    {
        HashEntry *entry = smaller->buckets[i];
        while (entry)
        {
            if (hashset_contains(larger, entry->key))
            {
                hashset_add(result, entry->key);
            }
            entry = entry->next;
        }
    }
}

/**
 * Difference of two sets (result = set1 - set2)
 * @param set1: First set
 * @param set2: Second set
 * @param result: Resulting difference set (should be initialized)
 */
static inline void hashset_difference(HashSet *set1, HashSet *set2, HashSet *result)
{
    for (size_t i = 0; i < set1->capacity; i++)
    {
        HashEntry *entry = set1->buckets[i];
        while (entry)
        {
            if (!hashset_contains(set2, entry->key))
            {
                hashset_add(result, entry->key);
            }
            entry = entry->next;
        }
    }
}

/**
 * Check if set1 is subset of set2
 * @param set1: Potential subset
 * @param set2: Potential superset
 * @return: true if set1 ⊆ set2
 */
static inline bool hashset_is_subset(HashSet *set1, HashSet *set2)
{
    if (hashset_size(set1) > hashset_size(set2))
        return false;

    for (size_t i = 0; i < set1->capacity; i++)
    {
        HashEntry *entry = set1->buckets[i];
        while (entry)
        {
            if (!hashset_contains(set2, entry->key))
            {
                return false;
            }
            entry = entry->next;
        }
    }
    return true;
}

/**
 * Check if two sets are equal
 * @param set1: First set
 * @param set2: Second set
 * @return: true if sets are equal
 */
static inline bool hashset_equals(HashSet *set1, HashSet *set2)
{
    return hashset_size(set1) == hashset_size(set2) &&
           hashset_is_subset(set1, set2);
}

// ==================== CONVENIENCE FUNCTIONS ====================

/**
 * Add string to set
 * @param set: String set
 * @param str: String to add
 * @return: true if added
 */
static inline bool hashset_add_string(HashSet *set, const char *str)
{
    return hashset_add(set, str);
}

/**
 * Remove string from set
 * @param set: String set
 * @param str: String to remove
 * @return: true if removed
 */
static inline bool hashset_remove_string(HashSet *set, const char *str)
{
    return hashset_remove(set, str);
}

/**
 * Check if string exists in set
 * @param set: String set
 * @param str: String to check
 * @return: true if exists
 */
static inline bool hashset_contains_string(HashSet *set, const char *str)
{
    return hashset_contains(set, str);
}

/**
 * Add integer to set
 * @param set: Integer set
 * @param value: Integer to add
 * @return: true if added
 */
static inline bool hashset_add_int(HashSet *set, int value)
{
    return hashset_add(set, &value);
}

/**
 * Remove integer from set
 * @param set: Integer set
 * @param value: Integer to remove
 * @return: true if removed
 */
static inline bool hashset_remove_int(HashSet *set, int value)
{
    return hashset_remove(set, &value);
}

/**
 * Check if integer exists in set
 * @param set: Integer set
 * @param value: Integer to check
 * @return: true if exists
 */
static inline bool hashset_contains_int(HashSet *set, int value)
{
    return hashset_contains(set, &value);
}

// ==================== UTILITY FUNCTIONS ====================

/**
 * Print set statistics
 * @param set: Target set
 */
static inline void hashset_print_stats(HashSet *set)
{
    printf("Hash Set Statistics:\n");
    hashtable_print_stats(set);
}

/**
 * Create copy of set
 * @param source: Source set
 * @param dest: Destination set (should be uninitialized)
 */
static inline void hashset_copy(HashSet *source, HashSet *dest)
{
    hashset_init(dest, source->capacity, source->hash_func);

    for (size_t i = 0; i < source->capacity; i++)
    {
        HashEntry *entry = source->buckets[i];
        while (entry)
        {
            hashset_add(dest, entry->key);
            entry = entry->next;
        }
    }
}

#endif // HASHSET_H