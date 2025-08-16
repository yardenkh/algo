#include "basketball_system.h"

// Comparison functions for heaps
static int player_age_compare_min(const void *a, const void *b) {
    const Player *p1 = (const Player*)a;
    const Player *p2 = (const Player*)b;
    return p1->age - p2->age;
}

static int player_age_compare_max(const void *a, const void *b) {
    const Player *p1 = (const Player*)a;
    const Player *p2 = (const Player*)b;
    return p2->age - p1->age;
}

static int player_height_compare_min(const void *a, const void *b) {
    const Player *p1 = (const Player*)a;
    const Player *p2 = (const Player*)b;
    return (p1->height > p2->height) ? 1 : (p1->height < p2->height) ? -1 : 0;
}

static int player_height_compare_max(const void *a, const void *b) {
    const Player *p1 = (const Player*)a;
    const Player *p2 = (const Player*)b;
    return (p2->height > p1->height) ? 1 : (p2->height < p1->height) ? -1 : 0;
}

static int player_skill_compare_max(const void *a, const void *b) {
    const Player *p1 = (const Player*)a;
    const Player *p2 = (const Player*)b;
    return (p2->skill_rating > p1->skill_rating) ? 1 : 
           (p2->skill_rating < p1->skill_rating) ? -1 : 0;
}

// Hash function helpers are already defined in hashtable.h

void basketball_system_init(BasketballSystem *system) {
    // Initialize primary storage
    dynarray_init(&system->players, 1000);
    dynarray_init(&system->teams, 100);
    dynarray_init(&system->leagues, 10);
    
    // Initialize hash tables using the predefined convenience functions
    hashtable_init_string(&system->player_by_name);
    hashtable_init_int(&system->player_by_id);
    hashtable_init_string(&system->team_by_name);
    hashtable_init_int(&system->team_by_id);
    
    // Initialize specialized indices
    hashtable_init_string(&system->players_by_nationality);
    hashtable_init_string(&system->players_by_position);
    hashtable_init_int(&system->players_by_team);
    
    // Initialize heaps with comparison functions
    min_heap_init(&system->youngest_players, 100);
    max_heap_init(&system->oldest_players, 100);
    min_heap_init(&system->shortest_players, 100);
    max_heap_init(&system->tallest_players, 100);
    max_heap_init(&system->top_skilled_players, 100);
    
    // Initialize utility structures
    stack_init(&system->recent_transactions);
    queue_init(&system->trade_requests);
    
    // Initialize counters
    system->next_player_id = 1;
    system->next_team_id = 1;
    system->next_league_id = 1;
}

void basketball_system_free(BasketballSystem *system) {
    // Free all players
    for (size_t i = 0; i < dynarray_size(&system->players); i++) {
        Player *player = (Player*)dynarray_get(&system->players, i);
        free(player);
    }
    dynarray_free(&system->players);
    
    // Free all teams
    for (size_t i = 0; i < dynarray_size(&system->teams); i++) {
        Team *team = (Team*)dynarray_get(&system->teams, i);
        dynarray_free(&team->roster);
        free(team);
    }
    dynarray_free(&system->teams);
    
    // Free all leagues
    for (size_t i = 0; i < dynarray_size(&system->leagues); i++) {
        League *league = (League*)dynarray_get(&system->leagues, i);
        dynarray_free(&league->teams);
        free(league);
    }
    dynarray_free(&system->leagues);
    
    // Free hash tables
    hashtable_free(&system->player_by_name);
    hashtable_free(&system->player_by_id);
    hashtable_free(&system->team_by_name);
    hashtable_free(&system->team_by_id);
    hashtable_free(&system->players_by_nationality);
    hashtable_free(&system->players_by_position);
    hashtable_free(&system->players_by_team);
    
    // Free heaps
    min_heap_free(&system->youngest_players);
    max_heap_free(&system->oldest_players);
    min_heap_free(&system->shortest_players);
    max_heap_free(&system->tallest_players);
    max_heap_free(&system->top_skilled_players);
    
    // Free utility structures
    stack_free(&system->recent_transactions);
    queue_free(&system->trade_requests);
}

Player* create_player(int id, const char *name, const char *nationality, const char *position,
                     int age, float height, float weight, int jersey_number, 
                     float skill_rating, int team_id) {
    Player *player = malloc(sizeof(Player));
    if (!player) return NULL;
    
    player->player_id = id;
    strncpy(player->name, name, sizeof(player->name) - 1);
    player->name[sizeof(player->name) - 1] = '\0';
    strncpy(player->nationality, nationality, sizeof(player->nationality) - 1);
    player->nationality[sizeof(player->nationality) - 1] = '\0';
    strncpy(player->position, position, sizeof(player->position) - 1);
    player->position[sizeof(player->position) - 1] = '\0';
    player->age = age;
    player->height = height;
    player->weight = weight;
    player->jersey_number = jersey_number;
    player->skill_rating = skill_rating;
    player->team_id = team_id;
    
    return player;
}

void add_player(BasketballSystem *system, const char *name, const char *nationality, 
                const char *position, int age, float height, float weight, 
                int jersey_number, float skill_rating, int team_id) {
    
    Player *player = create_player(system->next_player_id++, name, nationality, position,
                                  age, height, weight, jersey_number, skill_rating, team_id);
    if (!player) {
        printf("Error: Failed to create player\n");
        return;
    }
    
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

Player* find_player_by_name(BasketballSystem *system, const char *name) {
    return (Player*)hashtable_get(&system->player_by_name, name);
}

Player* find_player_by_id(BasketballSystem *system, int id) {
    return (Player*)hashtable_get(&system->player_by_id, &id);
}

Team* create_team(int id, const char *name, const char *city, int league_id) {
    Team *team = malloc(sizeof(Team));
    if (!team) return NULL;
    
    team->team_id = id;
    strncpy(team->name, name, sizeof(team->name) - 1);
    team->name[sizeof(team->name) - 1] = '\0';
    strncpy(team->city, city, sizeof(team->city) - 1);
    team->city[sizeof(team->city) - 1] = '\0';
    team->league_id = league_id;
    dynarray_init(&team->roster, 15);
    team->total_salary = 0.0f;
    team->wins = 0;
    team->losses = 0;
    
    return team;
}

void add_team(BasketballSystem *system, const char *name, const char *city, int league_id) {
    Team *team = create_team(system->next_team_id++, name, city, league_id);
    if (!team) {
        printf("Error: Failed to create team\n");
        return;
    }
    
    // Add to primary storage
    dynarray_push(&system->teams, team);
    
    // Add to indices
    hashtable_put(&system->team_by_name, team->name, team);
    hashtable_put(&system->team_by_id, &team->team_id, team);
    
    printf("Added team %s (ID: %d) to system\n", team->name, team->team_id);
}

Team* find_team_by_name(BasketballSystem *system, const char *name) {
    return (Team*)hashtable_get(&system->team_by_name, name);
}

Team* find_team_by_id(BasketballSystem *system, int id) {
    return (Team*)hashtable_get(&system->team_by_id, &id);
}

League* create_league(int id, const char *name, const char *country, int season_year) {
    League *league = malloc(sizeof(League));
    if (!league) return NULL;
    
    league->league_id = id;
    strncpy(league->name, name, sizeof(league->name) - 1);
    league->name[sizeof(league->name) - 1] = '\0';
    strncpy(league->country, country, sizeof(league->country) - 1);
    league->country[sizeof(league->country) - 1] = '\0';
    dynarray_init(&league->teams, 30);
    league->season_year = season_year;
    
    return league;
}

void add_league(BasketballSystem *system, const char *name, const char *country, int season_year) {
    League *league = create_league(system->next_league_id++, name, country, season_year);
    if (!league) {
        printf("Error: Failed to create league\n");
        return;
    }
    
    // Add to primary storage
    dynarray_push(&system->leagues, league);
    
    printf("Added league %s (ID: %d) to system\n", league->name, league->league_id);
}

// Query operations
DynArray* get_players_by_nationality(BasketballSystem *system, const char *nationality) {
    return (DynArray*)hashtable_get(&system->players_by_nationality, nationality);
}

DynArray* get_players_by_position(BasketballSystem *system, const char *position) {
    return (DynArray*)hashtable_get(&system->players_by_position, position);
}

DynArray* get_team_roster(BasketballSystem *system, int team_id) {
    return (DynArray*)hashtable_get(&system->players_by_team, &team_id);
}

Player* get_youngest_player(BasketballSystem *system) {
    return (Player*)min_heap_peek(&system->youngest_players);
}

Player* get_oldest_player(BasketballSystem *system) {
    return (Player*)max_heap_peek(&system->oldest_players);
}

Player* get_tallest_player(BasketballSystem *system) {
    return (Player*)max_heap_peek(&system->tallest_players);
}

Player* get_shortest_player(BasketballSystem *system) {
    return (Player*)min_heap_peek(&system->shortest_players);
}

Player* get_most_skilled_player(BasketballSystem *system) {
    return (Player*)max_heap_peek(&system->top_skilled_players);
}

// Complex queries
void find_elite_players_by_nationality_and_position(BasketballSystem *system, 
                                                   const char *nationality, 
                                                   const char *position, 
                                                   float min_skill) {
    printf("Elite %s %ss (Skill > %.1f):\n", nationality, position, min_skill);
    printf("=====================================\n");
    
    // Get players by nationality
    DynArray *nat_players = get_players_by_nationality(system, nationality);
    if (!nat_players) {
        printf("No %s players found.\n", nationality);
        return;
    }
    
    // Get players by position
    DynArray *pos_players = get_players_by_position(system, position);
    if (!pos_players) {
        printf("No %s players found.\n", position);
        return;
    }
    
    // Find intersection using hash set
    HashSet nat_set;
    hashset_init_string(&nat_set);
    
    // Add all nationality players to set
    for (size_t i = 0; i < dynarray_size(nat_players); i++) {
        Player *player = (Player*)dynarray_get(nat_players, i);
        hashset_add(&nat_set, player->name); // Use name as unique identifier
    }
    
    // Check each position player
    int count = 0;
    for (size_t i = 0; i < dynarray_size(pos_players); i++) {
        Player *player = (Player*)dynarray_get(pos_players, i);
        
        if (hashset_contains(&nat_set, player->name) && player->skill_rating >= min_skill) {
            printf("%d. %s - Age: %d, Skill: %.1f, Team ID: %d\n",
                   ++count, player->name, player->age, player->skill_rating, player->team_id);
        }
    }
    
    if (count == 0) {
        printf("No elite %s %ss found.\n", nationality, position);
    }
    
    hashset_free(&nat_set);
}

void find_players_in_age_range(BasketballSystem *system, int min_age, int max_age) {
    printf("Players aged %d-%d:\n", min_age, max_age);
    printf("==================\n");
    
    int count = 0;
    for (size_t i = 0; i < dynarray_size(&system->players); i++) {
        Player *player = (Player*)dynarray_get(&system->players, i);
        if (player->age >= min_age && player->age <= max_age) {
            printf("%d. %s - Age: %d, Position: %s, Skill: %.1f\n",
                   ++count, player->name, player->age, player->position, player->skill_rating);
        }
    }
    
    if (count == 0) {
        printf("No players found in age range %d-%d.\n", min_age, max_age);
    }
}

// Trade system
void request_trade(BasketballSystem *system, int from_team, int to_team, int player_id) {
    TradeTransaction *trade = malloc(sizeof(TradeTransaction));
    if (!trade) {
        printf("Error: Failed to create trade request\n");
        return;
    }
    
    trade->from_team_id = from_team;
    trade->to_team_id = to_team;
    trade->player_id = player_id;
    trade->trade_time = time(NULL);
    
    // Simple timestamp
    sprintf(trade->timestamp, "2024-Season-%ld", trade->trade_time);
    
    queue_enqueue(&system->trade_requests, trade);
    printf("Trade request queued: Player %d from Team %d to Team %d\n",
           player_id, from_team, to_team);
}

void process_next_trade(BasketballSystem *system) {
    TradeTransaction *trade = (TradeTransaction*)queue_dequeue(&system->trade_requests);
    
    if (!trade) {
        printf("No pending trades.\n");
        return;
    }
    
    // Execute trade
    Player *player = find_player_by_id(system, trade->player_id);
    if (player) {
        int old_team = player->team_id;
        player->team_id = trade->to_team_id;
        
        // Update team roster indices (simplified for demo)
        DynArray *new_roster = get_team_roster(system, trade->to_team_id);
        
        if (!new_roster) {
            new_roster = malloc(sizeof(DynArray));
            dynarray_init(new_roster, 15);
            hashtable_put(&system->players_by_team, &trade->to_team_id, new_roster);
        }
        dynarray_push(new_roster, player);
        
        // Push to recent transactions stack for undo capability
        stack_push(&system->recent_transactions, trade);
        
        printf("Trade completed: %s moved from Team %d to Team %d\n",
               player->name, old_team, trade->to_team_id);
    } else {
        printf("Error: Player %d not found\n", trade->player_id);
        free(trade);
    }
}

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
        printf("Trade undone: Player %s (ID: %d) returned to Team %d\n",
               player->name, last_trade->player_id, last_trade->from_team_id);
    }
    
    free(last_trade);
}

// Printing and statistics functions
void print_player_info(const Player *player) {
    if (!player) {
        printf("Player not found.\n");
        return;
    }
    
    printf("=== Player Info ===\n");
    printf("Name: %s\n", player->name);
    printf("ID: %d\n", player->player_id);
    printf("Nationality: %s\n", player->nationality);
    printf("Position: %s\n", player->position);
    printf("Age: %d\n", player->age);
    printf("Height: %.2fm\n", player->height);
    printf("Weight: %.1fkg\n", player->weight);
    printf("Jersey: #%d\n", player->jersey_number);
    printf("Skill Rating: %.1f/100\n", player->skill_rating);
    printf("Team ID: %d\n", player->team_id);
    printf("==================\n");
}

void print_team_info(BasketballSystem *system, const Team *team) {
    if (!team) {
        printf("Team not found.\n");
        return;
    }
    
    printf("=== Team Info ===\n");
    printf("Name: %s\n", team->name);
    printf("City: %s\n", team->city);
    printf("ID: %d\n", team->team_id);
    printf("League ID: %d\n", team->league_id);
    printf("Record: %d-%d\n", team->wins, team->losses);
    printf("Total Salary: $%.2fM\n", team->total_salary);
    
    // Show roster
    DynArray *roster = get_team_roster(system, team->team_id);
    if (roster && dynarray_size(roster) > 0) {
        printf("Roster (%zu players):\n", dynarray_size(roster));
        for (size_t i = 0; i < dynarray_size(roster); i++) {
            Player *player = (Player*)dynarray_get(roster, i);
            printf("  %zu. %s (#%d, %s) - %.1f skill\n", 
                   i+1, player->name, player->jersey_number, player->position, player->skill_rating);
        }
    } else {
        printf("No players on roster.\n");
    }
    printf("================\n");
}

void print_system_statistics(BasketballSystem *system) {
    printf("=== Basketball System Statistics ===\n");
    printf("Total Players: %zu\n", dynarray_size(&system->players));
    printf("Total Teams: %zu\n", dynarray_size(&system->teams));
    printf("Total Leagues: %zu\n", dynarray_size(&system->leagues));
    printf("Pending Trades: %zu\n", queue_size(&system->trade_requests));
    printf("Recent Transactions: %zu\n", stack_size(&system->recent_transactions));
    
    if (dynarray_size(&system->players) > 0) {
        Player *youngest = get_youngest_player(system);
        Player *oldest = get_oldest_player(system);
        Player *best = get_most_skilled_player(system);
        
        if (youngest) printf("Youngest Player: %s (%d years)\n", youngest->name, youngest->age);
        if (oldest) printf("Oldest Player: %s (%d years)\n", oldest->name, oldest->age);
        if (best) printf("Most Skilled: %s (%.1f rating)\n", best->name, best->skill_rating);
    }
    printf("===================================\n");
}

void print_league_info(BasketballSystem *system, const League *league) {
    (void)system; // Suppress unused parameter warning
    if (!league) {
        printf("League not found.\n");
        return;
    }
    
    printf("=== League Info ===\n");
    printf("Name: %s\n", league->name);
    printf("Country: %s\n", league->country);
    printf("ID: %d\n", league->league_id);
    printf("Season: %d\n", league->season_year);
    printf("Teams: %zu\n", dynarray_size((DynArray*)&league->teams));
    printf("==================\n");
}

void print_top_players_by_skill(BasketballSystem *system, int count) {
    printf("=== Top %d Players by Skill ===\n", count);
    
    // Create temporary array and sort by skill
    DynArray temp_players;
    dynarray_init(&temp_players, dynarray_size(&system->players));
    
    // Copy all players
    for (size_t i = 0; i < dynarray_size(&system->players); i++) {
        Player *player = (Player*)dynarray_get(&system->players, i);
        dynarray_push(&temp_players, player);
    }
    
    // Simple bubble sort by skill (for demo purposes)
    size_t n = dynarray_size(&temp_players);
    for (size_t i = 0; i < n - 1; i++) {
        for (size_t j = 0; j < n - i - 1; j++) {
            Player *p1 = (Player*)dynarray_get(&temp_players, j);
            Player *p2 = (Player*)dynarray_get(&temp_players, j + 1);
            if (p1->skill_rating < p2->skill_rating) {
                // Swap
                dynarray_set(&temp_players, j, p2);
                dynarray_set(&temp_players, j + 1, p1);
            }
        }
    }
    
    // Print top players
    int limit = (count < (int)n) ? count : (int)n;
    for (int i = 0; i < limit; i++) {
        Player *player = (Player*)dynarray_get(&temp_players, i);
        printf("%d. %s - %.1f skill (%s, %s)\n", 
               i+1, player->name, player->skill_rating, player->position, player->nationality);
    }
    
    dynarray_free(&temp_players);
    printf("==============================\n");
}
