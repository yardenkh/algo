#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

/**
 * HASH TABLE IMPLEMENTATION
 * 
 * Efficient key-value storage using separate chaining for collision resolution.
 * Provides average O(1) operations with good hash functions.
 * 
 * Time Complexities:
 * - Insert/Update: O(1) average, O(n) worst case
 * - Search: O(1) average, O(n) worst case  
 * - Delete: O(1) average, O(n) worst case
 * 
 * Space Complexity: O(n + m) where n = elements, m = buckets
 */

// Configuration constants
#define HASHTABLE_DEFAULT_SIZE 16
#define HASHTABLE_LOAD_FACTOR 0.75
#define HASHTABLE_MIN_SIZE 8
#define HASHTABLE_GROWTH_FACTOR 2

// Hash table entry (separate chaining)
typedef struct HashEntry {
    void *key;               // Generic key
    void *value;             // Associated value  
    struct HashEntry *next;  // Next entry in chain
} HashEntry;

// Hash function interface for different key types
typedef struct HashFunction {
    size_t (*hash)(const void *key, size_t capacity);       // Primary hash function
    bool (*key_equals)(const void *key1, const void *key2); // Key comparison
    void *(*key_copy)(const void *key);                     // Key duplication
    void (*key_free)(void *key);                            // Key deallocation
} HashFunction;

// Hash table structure
typedef struct HashTable {
    HashEntry **buckets;              // Array of chain heads
    size_t size;                      // Number of key-value pairs
    size_t capacity;                  // Number of buckets
    double load_factor_threshold;     // Resize trigger
    const HashFunction *hash_func;    // Hash function implementation
} HashTable;

// ==================== DEFAULT HASH FUNCTIONS ====================

/**
 * djb2 hash for strings (fast and well-distributed)
 * @param key: String key
 * @param capacity: Table capacity
 * @return: Hash value
 */
static inline size_t hash_string_djb2(const void *key, size_t capacity) {
    const char *str = (const char *)key;
    size_t hash = 5381;
    int c;
    
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash % capacity;
}

/**
 * String equality comparison
 * @param key1, key2: String keys to compare
 * @return: true if equal
 */
static inline bool hash_string_equals(const void *key1, const void *key2) {
    return strcmp((const char *)key1, (const char *)key2) == 0;
}

/**
 * String key duplication
 * @param key: String to copy
 * @return: Newly allocated copy
 */
static inline void *hash_string_copy(const void *key) {
    const char *str = (const char *)key;
    size_t len = strlen(str);
    char *copy = (char *)malloc(len + 1);
    if (!copy) {
        fprintf(stderr, "hash_string_copy: allocation failed\n");
        exit(EXIT_FAILURE);
    }
    strcpy(copy, str);
    return copy;
}

/**
 * String key deallocation
 * @param key: String to free
 */
static inline void hash_string_free(void *key) {
    if (key) free(key);
}

/**
 * Integer hash function (multiplicative hashing)
 * @param key: Pointer to integer
 * @param capacity: Table capacity
 * @return: Hash value
 */
static inline size_t hash_int(const void *key, size_t capacity) {
    int val = *(const int *)key;
    const size_t A = 2654435761U; // 2^32 / golden ratio
    return ((size_t)val * A) % capacity;
}

/**
 * Integer equality comparison
 * @param key1, key2: Integer keys to compare
 * @return: true if equal
 */
static inline bool hash_int_equals(const void *key1, const void *key2) {
    return *(const int *)key1 == *(const int *)key2;
}

/**
 * Integer key duplication
 * @param key: Integer to copy
 * @return: Newly allocated copy
 */
static inline void *hash_int_copy(const void *key) {
    int *copy = (int *)malloc(sizeof(int));
    if (!copy) {
        fprintf(stderr, "hash_int_copy: allocation failed\n");
        exit(EXIT_FAILURE);
    }
    *copy = *(const int *)key;
    return copy;
}

/**
 * Integer key deallocation
 * @param key: Integer to free
 */
static inline void hash_int_free(void *key) {
    if (key) free(key);
}

// Pre-defined hash functions
static const HashFunction STRING_HASH_FUNC = {
    .hash = hash_string_djb2,
    .key_equals = hash_string_equals,
    .key_copy = hash_string_copy,
    .key_free = hash_string_free
};

static const HashFunction INT_HASH_FUNC = {
    .hash = hash_int,
    .key_equals = hash_int_equals,
    .key_copy = hash_int_copy,
    .key_free = hash_int_free
};

// ==================== ENTRY OPERATIONS ====================

/**
 * Create new hash entry
 * @param key: Key to store
 * @param value: Value to associate
 * @param hash_func: Hash function for key operations
 * @return: New entry or NULL on failure
 */
static inline HashEntry *hashtable_create_entry(const void *key, void *value, const HashFunction *hash_func) {
    HashEntry *entry = (HashEntry *)malloc(sizeof(HashEntry));
    if (!entry) return NULL;
    
    entry->key = hash_func->key_copy(key);
    entry->value = value;
    entry->next = NULL;
    return entry;
}

/**
 * Free hash entry
 * @param entry: Entry to free
 * @param hash_func: Hash function for key deallocation
 */
static inline void hashtable_free_entry(HashEntry *entry, const HashFunction *hash_func) {
    if (entry) {
        hash_func->key_free(entry->key);
        free(entry);
    }
}

// ==================== CORE OPERATIONS ====================

/**
 * Initialize hash table
 * @param table: Table to initialize
 * @param initial_capacity: Starting bucket count
 * @param hash_func: Hash function to use
 */
static inline void hashtable_init(HashTable *table, size_t initial_capacity, const HashFunction *hash_func) {
    table->capacity = initial_capacity < HASHTABLE_MIN_SIZE ? HASHTABLE_MIN_SIZE : initial_capacity;
    table->size = 0;
    table->load_factor_threshold = HASHTABLE_LOAD_FACTOR;
    table->hash_func = hash_func;
    
    table->buckets = (HashEntry **)calloc(table->capacity, sizeof(HashEntry *));
    if (!table->buckets) {
        fprintf(stderr, "hashtable_init: allocation failed\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * Initialize string hash table with default settings
 * @param table: Table to initialize
 */
static inline void hashtable_init_string(HashTable *table) {
    hashtable_init(table, HASHTABLE_DEFAULT_SIZE, &STRING_HASH_FUNC);
}

/**
 * Initialize integer hash table with default settings
 * @param table: Table to initialize
 */
static inline void hashtable_init_int(HashTable *table) {
    hashtable_init(table, HASHTABLE_DEFAULT_SIZE, &INT_HASH_FUNC);
}

/**
 * Resize hash table
 * @param table: Table to resize
 * @param new_capacity: New bucket count
 */
static inline void hashtable_resize(HashTable *table, size_t new_capacity) {
    HashEntry **old_buckets = table->buckets;
    size_t old_capacity = table->capacity;
    
    // Initialize new table
    table->capacity = new_capacity;
    table->size = 0;
    table->buckets = (HashEntry **)calloc(new_capacity, sizeof(HashEntry *));
    if (!table->buckets) {
        fprintf(stderr, "hashtable_resize: allocation failed\n");
        exit(EXIT_FAILURE);
    }
    
    // Rehash all entries
    for (size_t i = 0; i < old_capacity; i++) {
        HashEntry *entry = old_buckets[i];
        while (entry) {
            HashEntry *next = entry->next;
            
            // Rehash entry to new location
            size_t new_index = table->hash_func->hash(entry->key, new_capacity);
            entry->next = table->buckets[new_index];
            table->buckets[new_index] = entry;
            table->size++;
            
            entry = next;
        }
    }
    
    free(old_buckets);
}

/**
 * Insert or update key-value pair
 * @param table: Target table
 * @param key: Key to insert/update
 * @param value: Value to associate
 * @return: true if successful
 */
static inline bool hashtable_put(HashTable *table, const void *key, void *value) {
    // Resize if load factor exceeded
    if ((double)table->size >= table->load_factor_threshold * table->capacity) {
        hashtable_resize(table, table->capacity * HASHTABLE_GROWTH_FACTOR);
    }
    
    size_t index = table->hash_func->hash(key, table->capacity);
    
    // Search for existing key
    HashEntry *current = table->buckets[index];
    while (current) {
        if (table->hash_func->key_equals(current->key, key)) {
            current->value = value; // Update existing
            return true;
        }
        current = current->next;
    }
    
    // Insert new entry at head of chain
    HashEntry *new_entry = hashtable_create_entry(key, value, table->hash_func);
    if (!new_entry) return false;
    
    new_entry->next = table->buckets[index];
    table->buckets[index] = new_entry;
    table->size++;
    
    return true;
}

/**
 * Retrieve value by key
 * @param table: Target table
 * @param key: Key to search for
 * @return: Associated value or NULL if not found
 */
static inline void *hashtable_get(HashTable *table, const void *key) {
    size_t index = table->hash_func->hash(key, table->capacity);
    
    HashEntry *current = table->buckets[index];
    while (current) {
        if (table->hash_func->key_equals(current->key, key)) {
            return current->value;
        }
        current = current->next;
    }
    
    return NULL; // Key not found
}

/**
 * Remove key-value pair
 * @param table: Target table
 * @param key: Key to remove
 * @return: true if key was found and removed
 */
static inline bool hashtable_remove(HashTable *table, const void *key) {
    size_t index = table->hash_func->hash(key, table->capacity);
    
    HashEntry *current = table->buckets[index];
    HashEntry *prev = NULL;
    
    while (current) {
        if (table->hash_func->key_equals(current->key, key)) {
            // Remove entry from chain
            if (prev) {
                prev->next = current->next;
            } else {
                table->buckets[index] = current->next;
            }
            
            hashtable_free_entry(current, table->hash_func);
            table->size--;
            return true;
        }
        prev = current;
        current = current->next;
    }
    
    return false; // Key not found
}

/**
 * Check if key exists
 * @param table: Target table
 * @param key: Key to check
 * @return: true if key exists
 */
static inline bool hashtable_contains(HashTable *table, const void *key) {
    return hashtable_get(table, key) != NULL;
}

/**
 * Get current size
 * @param table: Target table
 * @return: Number of key-value pairs
 */
static inline size_t hashtable_size(HashTable *table) {
    return table->size;
}

/**
 * Check if empty
 * @param table: Target table
 * @return: true if empty
 */
static inline bool hashtable_is_empty(HashTable *table) {
    return table->size == 0;
}

/**
 * Clear all entries
 * @param table: Target table
 */
static inline void hashtable_clear(HashTable *table) {
    for (size_t i = 0; i < table->capacity; i++) {
        HashEntry *current = table->buckets[i];
        while (current) {
            HashEntry *next = current->next;
            hashtable_free_entry(current, table->hash_func);
            current = next;
        }
        table->buckets[i] = NULL;
    }
    table->size = 0;
}

/**
 * Free all memory
 * @param table: Table to free
 */
static inline void hashtable_free(HashTable *table) {
    hashtable_clear(table);
    free(table->buckets);
    table->buckets = NULL;
    table->capacity = 0;
}

// ==================== UTILITY FUNCTIONS ====================

/**
 * Get current load factor
 * @param table: Target table
 * @return: Current load factor
 */
static inline double hashtable_load_factor(HashTable *table) {
    return table->capacity > 0 ? (double)table->size / table->capacity : 0.0;
}

/**
 * Get number of empty buckets
 * @param table: Target table
 * @return: Count of empty buckets
 */
static inline size_t hashtable_empty_buckets(HashTable *table) {
    size_t empty = 0;
    for (size_t i = 0; i < table->capacity; i++) {
        if (table->buckets[i] == NULL) {
            empty++;
        }
    }
    return empty;
}

/**
 * Get maximum chain length
 * @param table: Target table
 * @return: Longest chain length
 */
static inline size_t hashtable_max_chain_length(HashTable *table) {
    size_t max_length = 0;
    for (size_t i = 0; i < table->capacity; i++) {
        size_t chain_length = 0;
        HashEntry *current = table->buckets[i];
        while (current) {
            chain_length++;
            current = current->next;
        }
        if (chain_length > max_length) {
            max_length = chain_length;
        }
    }
    return max_length;
}

/**
 * Print table statistics for debugging
 * @param table: Target table
 */
static inline void hashtable_print_stats(HashTable *table) {
    printf("Hash Table Statistics:\n");
    printf("  Size: %zu, Capacity: %zu\n", table->size, table->capacity);
    printf("  Load Factor: %.3f (threshold: %.3f)\n", 
           hashtable_load_factor(table), table->load_factor_threshold);
    printf("  Empty Buckets: %zu (%.1f%%)\n", 
           hashtable_empty_buckets(table), 
           100.0 * hashtable_empty_buckets(table) / table->capacity);
    printf("  Max Chain Length: %zu\n", hashtable_max_chain_length(table));
}

// ==================== CONVENIENCE FUNCTIONS ====================

/**
 * Put string key-value pair (assumes string keys)
 * @param table: String hash table
 * @param key: String key
 * @param value: Value to associate
 * @return: true if successful
 */
static inline bool hashtable_put_string(HashTable *table, const char *key, void *value) {
    return hashtable_put(table, key, value);
}

/**
 * Get value by string key
 * @param table: String hash table
 * @param key: String key
 * @return: Associated value or NULL
 */
static inline void *hashtable_get_string(HashTable *table, const char *key) {
    return hashtable_get(table, key);
}

/**
 * Remove by string key
 * @param table: String hash table
 * @param key: String key to remove
 * @return: true if removed
 */
static inline bool hashtable_remove_string(HashTable *table, const char *key) {
    return hashtable_remove(table, key);
}

/**
 * Put integer key-value pair (assumes integer keys)
 * @param table: Integer hash table
 * @param key: Integer key
 * @param value: Value to associate
 * @return: true if successful
 */
static inline bool hashtable_put_int(HashTable *table, int key, void *value) {
    return hashtable_put(table, &key, value);
}

/**
 * Get value by integer key
 * @param table: Integer hash table
 * @param key: Integer key
 * @return: Associated value or NULL
 */
static inline void *hashtable_get_int(HashTable *table, int key) {
    return hashtable_get(table, &key);
}

/**
 * Remove by integer key
 * @param table: Integer hash table
 * @param key: Integer key to remove
 * @return: true if removed
 */
static inline bool hashtable_remove_int(HashTable *table, int key) {
    return hashtable_remove(table, &key);
}

#endif // HASHTABLE_H