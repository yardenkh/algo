# Data Structures Guide: Building a Basketball League Management System

## Table of Contents

1. [Introduction](#introduction)
2. [Data Structure Overview](#data-structure-overview)
3. [System Design](#system-design)
4. [Implementation Guide](#implementation-guide)
5. [Performance Analysis](#performance-analysis)
6. [Usage Examples](#usage-examples)

## Introduction

This guide demonstrates how to combine multiple data structures to build a comprehensive basketball league management system. We'll manage players, teams, leagues, and enable fast queries across multiple dimensions.

### System Requirements

- Manage multiple leagues (NBA, EuroLeague, etc.)
- Track players with attributes (height, weight, age, name, number, skills)
- Organize teams within leagues
- Fast lookups by player name, team, nationality, position
- Efficient range queries (players by age, height, etc.)
- Team statistics and roster management

## Data Structure Overview

### Core Data Structures Used

1. **Hash Tables** - O(1) lookups by key (name, ID)
2. **Hash Sets** - O(1) membership testing
3. **Min/Max Heaps** - O(log n) priority operations
4. **Dynamic Arrays** - O(1) append, O(1) random access
5. **Doubly Linked Lists** - O(1) insertion/deletion
6. **Stacks/Queues** - LIFO/FIFO operations

## System Design

```c
// Include all necessary data structures
#include "dynarray/dynarray.h"
#include "hash/hashtable.h"
#include "hash/hashset.h"
#include "heap/min_heap.h"
#include "heap/max_heap.h"
#include "linkedlist/doubly_linked_list.h"
#include "containers/stack.h"
#include "containers/queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Player structure
typedef struct {
    int player_id;
    char name[64];
    char nationality[32];
    char position[16];    // PG, SG, SF, PF, C
    int age;
    float height;        // in meters
    float weight;        // in kg
    int jersey_number;
    float skill_rating;  // 0.0 - 100.0
    int team_id;
} Player;

// Team structure
typedef struct {
    int team_id;
    char name[64];
    char city[64];
    int league_id;
    DynArray roster;     // Array of Player pointers
    float total_salary;
    int wins;
    int losses;
} Team;

// League structure
typedef struct {
    int league_id;
    char name[64];
    char country[64];
    DynArray teams;      // Array of Team pointers
    int season_year;
} League;

// Main basketball management system
typedef struct {
    // Primary storage
    DynArray players;    // All players
    DynArray teams;      // All teams
    DynArray leagues;    // All leagues

    // Fast lookup indices
    HashTable player_by_name;     // name -> Player*
    HashTable player_by_id;       // id -> Player*
    HashTable team_by_name;       // name -> Team*
    HashTable team_by_id;         // id -> Team*

    // Specialized indices
    HashTable players_by_nationality; // nationality -> DynArray of Player*
    HashTable players_by_position;    // position -> DynArray of Player*
    HashTable players_by_team;        // team_id -> DynArray of Player*

    // Performance optimized structures
    MinHeap youngest_players;     // Min heap by age
    MaxHeap oldest_players;       // Max heap by age
    MinHeap shortest_players;     // Min heap by height
    MaxHeap tallest_players;      // Max heap by height
    MaxHeap top_skilled_players;  // Max heap by skill rating

    // Utility structures
    Stack recent_transactions;    // Recent player moves
    Queue trade_requests;         // Pending trades

} BasketballSystem;
```

## Implementation Guide

### 1. System Initialization

```c
void basketball_system_init(BasketballSystem *system) {
    // Initialize primary storage
    dynarray_init(&system->players, 1000);
    dynarray_init(&system->teams, 100);
    dynarray_init(&system->leagues, 10);

    // Initialize hash tables for fast lookups
    hashtable_init_string(&system->player_by_name);
    hashtable_init_int(&system->player_by_id);
    hashtable_init_string(&system->team_by_name);
    hashtable_init_int(&system->team_by_id);

    // Initialize specialized indices
    hashtable_init_string(&system->players_by_nationality);
    hashtable_init_string(&system->players_by_position);
    hashtable_init_int(&system->players_by_team);

    // Initialize heaps for performance queries
    min_heap_init(&system->youngest_players, 100);
    max_heap_init(&system->oldest_players, 100);
    min_heap_init(&system->shortest_players, 100);
    max_heap_init(&system->tallest_players, 100);
    max_heap_init(&system->top_skilled_players, 100);

    // Initialize utility structures
    stack_init(&system->recent_transactions);
    queue_init(&system->trade_requests);
}

// Helper function for age comparison in heaps
int player_age_compare(const void *a, const void *b) {
    Player *p1 = (Player*)a;
    Player *p2 = (Player*)b;
    return p1->age - p2->age;
}

// Helper function for skill comparison
int player_skill_compare(const void *a, const void *b) {
    Player *p1 = (Player*)a;
    Player *p2 = (Player*)b;
    return (p1->skill_rating > p2->skill_rating) ? 1 :
           (p1->skill_rating < p2->skill_rating) ? -1 : 0;
}
```

### 2. Adding Players - Demonstrating Multiple Data Structure Updates

```c
void add_player(BasketballSystem *system, Player *player) {
    // Add to primary storage
    dynarray_push(&system->players, player);

    // Add to hash table indices for O(1) lookups
    hashtable_put(&system->player_by_name, player->name, player);
    hashtable_put(&system->player_by_id, &player->player_id, player);

    // Update specialized indices

    // 1. Players by nationality
    DynArray *nat_players = (DynArray*)hashtable_get(&system->players_by_nationality,
                                                     player->nationality);
    if (!nat_players) {
        nat_players = malloc(sizeof(DynArray));
        dynarray_init(nat_players, 10);
        hashtable_put(&system->players_by_nationality, player->nationality, nat_players);
    }
    dynarray_push(nat_players, player);

    // 2. Players by position
    DynArray *pos_players = (DynArray*)hashtable_get(&system->players_by_position,
                                                     player->position);
    if (!pos_players) {
        pos_players = malloc(sizeof(DynArray));
        dynarray_init(pos_players, 20);
        hashtable_put(&system->players_by_position, player->position, pos_players);
    }
    dynarray_push(pos_players, player);

    // 3. Players by team
    DynArray *team_players = (DynArray*)hashtable_get(&system->players_by_team,
                                                      &player->team_id);
    if (!team_players) {
        team_players = malloc(sizeof(DynArray));
        dynarray_init(team_players, 15); // Typical roster size
        hashtable_put(&system->players_by_team, &player->team_id, team_players);
    }
    dynarray_push(team_players, player);

    // Update heaps for performance queries
    min_heap_push(&system->youngest_players, player);
    max_heap_push(&system->oldest_players, player);
    min_heap_push(&system->shortest_players, player);
    max_heap_push(&system->tallest_players, player);
    max_heap_push(&system->top_skilled_players, player);

    printf("Added player %s (ID: %d) to system\n", player->name, player->player_id);
}
```

### 3. Query Operations - Leveraging Different Data Structures

```c
// O(1) lookup by name using hash table
Player* find_player_by_name(BasketballSystem *system, const char *name) {
    return (Player*)hashtable_get(&system->player_by_name, name);
}

// O(1) lookup by ID using hash table
Player* find_player_by_id(BasketballSystem *system, int id) {
    return (Player*)hashtable_get(&system->player_by_id, &id);
}

// O(1) get all players from nationality using specialized index
DynArray* get_players_by_nationality(BasketballSystem *system, const char *nationality) {
    return (DynArray*)hashtable_get(&system->players_by_nationality, nationality);
}

// O(1) get all players by position
DynArray* get_players_by_position(BasketballSystem *system, const char *position) {
    return (DynArray*)hashtable_get(&system->players_by_position, position);
}

// O(1) get team roster
DynArray* get_team_roster(BasketballSystem *system, int team_id) {
    return (DynArray*)hashtable_get(&system->players_by_team, &team_id);
}

// O(1) get youngest player using min heap
Player* get_youngest_player(BasketballSystem *system) {
    return (Player*)min_heap_peek(&system->youngest_players);
}

// O(1) get most skilled player using max heap
Player* get_most_skilled_player(BasketballSystem *system) {
    return (Player*)max_heap_peek(&system->top_skilled_players);
}
```

### 4. Complex Queries - Combining Multiple Data Structures

```c
// Find all point guards from USA with skill > 85.0
void find_elite_usa_point_guards(BasketballSystem *system) {
    printf("Elite USA Point Guards (Skill > 85.0):\n");
    printf("=====================================\n");

    // Step 1: Get all USA players - O(1) hash table lookup
    DynArray *usa_players = get_players_by_nationality(system, "USA");
    if (!usa_players) {
        printf("No USA players found.\n");
        return;
    }

    // Step 2: Get all point guards - O(1) hash table lookup
    DynArray *point_guards = get_players_by_position(system, "PG");
    if (!point_guards) {
        printf("No point guards found.\n");
        return;
    }

    // Step 3: Find intersection and filter by skill
    HashSet usa_set;
    hashset_init(&usa_set, dynarray_size(usa_players));

    // Add all USA players to set for O(1) membership testing
    for (size_t i = 0; i < dynarray_size(usa_players); i++) {
        Player *player = (Player*)dynarray_get(usa_players, i);
        hashset_add(&usa_set, player);
    }

    // Check each point guard
    int count = 0;
    for (size_t i = 0; i < dynarray_size(point_guards); i++) {
        Player *pg = (Player*)dynarray_get(point_guards, i);

        // O(1) membership test and skill filter
        if (hashset_contains(&usa_set, pg) && pg->skill_rating > 85.0) {
            printf("%d. %s - Age: %d, Skill: %.1f, Team ID: %d\n",
                   ++count, pg->name, pg->age, pg->skill_rating, pg->team_id);
        }
    }

    if (count == 0) {
        printf("No elite USA point guards found.\n");
    }

    hashset_free(&usa_set);
}
```

### 5. Trade System - Using Stacks and Queues

```c
typedef struct {
    int from_team_id;
    int to_team_id;
    int player_id;
    char timestamp[32];
} TradeTransaction;

// Add trade request to queue (FIFO processing)
void request_trade(BasketballSystem *system, int from_team, int to_team, int player_id) {
    TradeTransaction *trade = malloc(sizeof(TradeTransaction));
    trade->from_team_id = from_team;
    trade->to_team_id = to_team;
    trade->player_id = player_id;

    // Add timestamp (simplified)
    sprintf(trade->timestamp, "2024-Season");

    queue_enqueue(&system->trade_requests, trade);
    printf("Trade request queued: Player %d from Team %d to Team %d\n",
           player_id, from_team, to_team);
}

// Process trade requests in FIFO order
void process_next_trade(BasketballSystem *system) {
    TradeTransaction *trade = (TradeTransaction*)queue_dequeue(&system->trade_requests);

    if (!trade) {
        printf("No pending trades.\n");
        return;
    }

    // Execute trade (update player's team_id and indices)
    Player *player = find_player_by_id(system, trade->player_id);
    if (player) {
        int old_team = player->team_id;
        player->team_id = trade->to_team_id;

        // Update team roster indices
        // Remove from old team
        DynArray *old_roster = get_team_roster(system, old_team);
        // Add to new team
        DynArray *new_roster = get_team_roster(system, trade->to_team_id);

        // Push to recent transactions stack for undo capability
        stack_push(&system->recent_transactions, trade);

        printf("Trade completed: %s moved from Team %d to Team %d\n",
               player->name, old_team, trade->to_team_id);
    }
}

// Undo last trade using stack (LIFO)
void undo_last_trade(BasketballSystem *system) {
    TradeTransaction *last_trade = (TradeTransaction*)stack_pop(&system->recent_transactions);

    if (!last_trade) {
        printf("No trades to undo.\n");
        return;
    }

    // Reverse the trade
    Player *player = find_player_by_id(system, last_trade->player_id);
    if (player) {
        player->team_id = last_trade->from_team_id;
        printf("Trade undone: Player %d returned to Team %d\n",
               last_trade->player_id, last_trade->from_team_id);
    }

    free(last_trade);
}
```

## Performance Analysis

### Time Complexities

| Operation                  | Data Structure Used | Time Complexity   |
| -------------------------- | ------------------- | ----------------- |
| Find player by name        | Hash Table          | O(1)              |
| Find player by ID          | Hash Table          | O(1)              |
| Get youngest player        | Min Heap            | O(1)              |
| Get players by nationality | Hash Table + Array  | O(1) + O(k)\*     |
| Add new player             | Multiple structures | O(log n)\*\*      |
| Process trade              | Queue + Updates     | O(1) + O(updates) |
| Complex filtered query     | Hash Set + Arrays   | O(m + n)          |

\*k = number of players of that nationality  
\*\*Dominated by heap insertions

### Space Complexity

- **Primary Storage**: O(P + T + L) where P=players, T=teams, L=leagues
- **Indices**: O(P) for each specialized index
- **Heaps**: O(P) total across all heaps
- **Total**: O(P) - Linear in number of players

## Usage Examples

### Complete Example Program

```c
int main() {
    BasketballSystem system;
    basketball_system_init(&system);

    // Create sample players
    Player lebron = {1, "LeBron James", "USA", "SF", 38, 2.03f, 113.4f, 23, 95.5f, 101};
    Player luka = {2, "Luka Doncic", "Slovenia", "PG", 24, 2.01f, 104.3f, 77, 94.2f, 102};
    Player giannis = {3, "Giannis Antetokounmpo", "Greece", "PF", 28, 2.11f, 109.0f, 34, 96.1f, 103};

    // Add players to system
    add_player(&system, &lebron);
    add_player(&system, &luka);
    add_player(&system, &giannis);

    printf("\n=== QUERY EXAMPLES ===\n");

    // 1. Direct lookups - O(1)
    Player *found = find_player_by_name(&system, "LeBron James");
    if (found) {
        printf("Found: %s, Age: %d, Skill: %.1f\n",
               found->name, found->age, found->skill_rating);
    }

    // 2. Get most skilled player - O(1)
    Player *best = get_most_skilled_player(&system);
    if (best) {
        printf("Most skilled: %s (%.1f)\n", best->name, best->skill_rating);
    }

    // 3. Get all players by nationality
    printf("\nUSA Players:\n");
    DynArray *usa_players = get_players_by_nationality(&system, "USA");
    if (usa_players) {
        for (size_t i = 0; i < dynarray_size(usa_players); i++) {
            Player *p = (Player*)dynarray_get(usa_players, i);
            printf("- %s\n", p->name);
        }
    }

    // 4. Trade system demonstration
    printf("\n=== TRADE SYSTEM ===\n");
    request_trade(&system, 101, 102, 1); // Trade LeBron
    process_next_trade(&system);
    undo_last_trade(&system);

    return 0;
}
```

This guide demonstrates how combining multiple data structures creates a powerful, efficient system. Each structure serves a specific purpose:

- **Hash Tables**: Lightning-fast lookups
- **Heaps**: Instant access to extremes
- **Arrays**: Efficient bulk storage
- **Sets**: Fast membership testing
- **Stacks/Queues**: Transaction management

The key is matching the right data structure to each specific use case while maintaining consistency across all indices.

