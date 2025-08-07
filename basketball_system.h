#ifndef BASKETBALL_SYSTEM_H
#define BASKETBALL_SYSTEM_H

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
#include <time.h>

// Player structure
typedef struct
{
    int player_id;
    char name[64];
    char nationality[32];
    char position[16]; // PG, SG, SF, PF, C
    int age;
    float height; // in meters
    float weight; // in kg
    int jersey_number;
    float skill_rating; // 0.0 - 100.0
    int team_id;
} Player;

// Team structure
typedef struct
{
    int team_id;
    char name[64];
    char city[64];
    int league_id;
    DynArray roster; // Array of Player pointers
    float total_salary;
    int wins;
    int losses;
} Team;

// League structure
typedef struct
{
    int league_id;
    char name[64];
    char country[64];
    DynArray teams; // Array of Team pointers
    int season_year;
} League;

// Trade transaction structure
typedef struct
{
    int from_team_id;
    int to_team_id;
    int player_id;
    char timestamp[32];
    time_t trade_time;
} TradeTransaction;

// Main basketball management system
typedef struct
{
    // Primary storage
    DynArray players; // All players
    DynArray teams;   // All teams
    DynArray leagues; // All leagues

    // Fast lookup indices
    HashTable player_by_name; // name -> Player*
    HashTable player_by_id;   // id -> Player*
    HashTable team_by_name;   // name -> Team*
    HashTable team_by_id;     // id -> Team*

    // Specialized indices
    HashTable players_by_nationality; // nationality -> DynArray of Player*
    HashTable players_by_position;    // position -> DynArray of Player*
    HashTable players_by_team;        // team_id -> DynArray of Player*

    // Performance optimized structures
    MinHeap youngest_players;    // Min heap by age
    MaxHeap oldest_players;      // Max heap by age
    MinHeap shortest_players;    // Min heap by height
    MaxHeap tallest_players;     // Max heap by height
    MaxHeap top_skilled_players; // Max heap by skill rating

    // Utility structures
    Stack recent_transactions; // Recent player moves
    Queue trade_requests;      // Pending trades

    // System counters
    int next_player_id;
    int next_team_id;
    int next_league_id;

} BasketballSystem;

// Function declarations
void basketball_system_init(BasketballSystem *system);
void basketball_system_free(BasketballSystem *system);

// Player operations
void add_player(BasketballSystem *system, const char *name, const char *nationality,
                const char *position, int age, float height, float weight,
                int jersey_number, float skill_rating, int team_id);
Player *find_player_by_name(BasketballSystem *system, const char *name);
Player *find_player_by_id(BasketballSystem *system, int id);
void remove_player(BasketballSystem *system, int player_id);

// Team operations
void add_team(BasketballSystem *system, const char *name, const char *city, int league_id);
Team *find_team_by_name(BasketballSystem *system, const char *name);
Team *find_team_by_id(BasketballSystem *system, int id);

// League operations
void add_league(BasketballSystem *system, const char *name, const char *country, int season_year);
League *find_league_by_name(BasketballSystem *system, const char *name);

// Query operations
DynArray *get_players_by_nationality(BasketballSystem *system, const char *nationality);
DynArray *get_players_by_position(BasketballSystem *system, const char *position);
DynArray *get_team_roster(BasketballSystem *system, int team_id);
Player *get_youngest_player(BasketballSystem *system);
Player *get_oldest_player(BasketballSystem *system);
Player *get_tallest_player(BasketballSystem *system);
Player *get_shortest_player(BasketballSystem *system);
Player *get_most_skilled_player(BasketballSystem *system);

// Complex queries
void find_elite_players_by_nationality_and_position(BasketballSystem *system,
                                                    const char *nationality,
                                                    const char *position,
                                                    float min_skill);
void find_players_in_age_range(BasketballSystem *system, int min_age, int max_age);
void find_players_in_height_range(BasketballSystem *system, float min_height, float max_height);

// Trade system
void request_trade(BasketballSystem *system, int from_team, int to_team, int player_id);
void process_next_trade(BasketballSystem *system);
void undo_last_trade(BasketballSystem *system);
void show_pending_trades(BasketballSystem *system);
void show_recent_transactions(BasketballSystem *system);

// Statistics and reporting
void print_player_info(const Player *player);
void print_team_info(BasketballSystem *system, const Team *team);
void print_league_info(BasketballSystem *system, const League *league);
void print_system_statistics(BasketballSystem *system);
void print_top_players_by_skill(BasketballSystem *system, int count);

// Utility functions
Player *create_player(int id, const char *name, const char *nationality, const char *position,
                      int age, float height, float weight, int jersey_number,
                      float skill_rating, int team_id);
Team *create_team(int id, const char *name, const char *city, int league_id);
League *create_league(int id, const char *name, const char *country, int season_year);

#endif // BASKETBALL_SYSTEM_H
