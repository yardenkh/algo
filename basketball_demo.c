#include "basketball_system.h"

void create_sample_data(BasketballSystem *system) {
    // Create leagues
    add_league(system, "NBA", "USA", 2024);
    add_league(system, "EuroLeague", "Europe", 2024);
    
    // Create teams
    add_team(system, "Lakers", "Los Angeles", 1);
    add_team(system, "Warriors", "Golden State", 1);
    add_team(system, "Celtics", "Boston", 1);
    add_team(system, "Real Madrid", "Madrid", 2);
    add_team(system, "Barcelona", "Barcelona", 2);
    
    // Create star players
    printf("\n=== Adding Players ===\n");
    
    // NBA players
    add_player(system, "LeBron James", "USA", "SF", 39, 2.03f, 113.4f, 23, 95.5f, 1);
    add_player(system, "Stephen Curry", "USA", "PG", 35, 1.88f, 84.8f, 30, 96.2f, 2);
    add_player(system, "Jayson Tatum", "USA", "SF", 25, 2.03f, 95.3f, 0, 93.8f, 3);
    add_player(system, "Luka Doncic", "Slovenia", "PG", 24, 2.01f, 104.3f, 77, 94.7f, 1);
    add_player(system, "Giannis Antetokounmpo", "Greece", "PF", 29, 2.11f, 109.0f, 34, 96.1f, 2);
    add_player(system, "Nikola Jokic", "Serbia", "C", 29, 2.11f, 129.3f, 15, 95.8f, 3);
    
    // EuroLeague players  
    add_player(system, "Sergio Llull", "Spain", "PG", 35, 1.90f, 86.0f, 23, 91.2f, 4);
    add_player(system, "Nikola Mirotic", "Spain", "PF", 33, 2.08f, 102.1f, 33, 92.5f, 4);
    add_player(system, "Cory Higgins", "USA", "SG", 34, 1.96f, 88.5f, 22, 89.8f, 5);
    add_player(system, "Alex Abrines", "Spain", "SG", 30, 1.98f, 86.2f, 31, 88.7f, 5);
    
    // Young prospects
    add_player(system, "Victor Wembanyama", "France", "C", 20, 2.21f, 95.3f, 1, 92.3f, 1);
    add_player(system, "Paolo Banchero", "Italy", "PF", 21, 2.03f, 113.4f, 5, 89.5f, 2);
}

void demo_basic_queries(BasketballSystem *system) {
    printf("\n=== BASIC QUERY DEMONSTRATIONS ===\n");
    
    // 1. Find player by name - O(1)
    printf("\n1. Find Player by Name:\n");
    Player *lebron = find_player_by_name(system, "LeBron James");
    if (lebron) {
        print_player_info(lebron);
    }
    
    // 2. Get performance extremes - O(1)
    printf("\n2. Performance Extremes:\n");
    Player *youngest = get_youngest_player(system);
    Player *oldest = get_oldest_player(system);
    Player *most_skilled = get_most_skilled_player(system);
    
    if (youngest) printf("Youngest: %s (%d years old)\n", youngest->name, youngest->age);
    if (oldest) printf("Oldest: %s (%d years old)\n", oldest->name, oldest->age);
    if (most_skilled) printf("Most Skilled: %s (%.1f rating)\n", most_skilled->name, most_skilled->skill_rating);
    
    // 3. Players by nationality - O(1) + O(k)
    printf("\n3. USA Players:\n");
    DynArray *usa_players = get_players_by_nationality(system, "USA");
    if (usa_players) {
        for (size_t i = 0; i < dynarray_size(usa_players); i++) {
            Player *p = (Player*)dynarray_get(usa_players, i);
            printf("  %s - %s, Team %d\n", p->name, p->position, p->team_id);
        }
    }
    
    // 4. Players by position - O(1) + O(k)
    printf("\n4. Point Guards:\n");
    DynArray *point_guards = get_players_by_position(system, "PG");
    if (point_guards) {
        for (size_t i = 0; i < dynarray_size(point_guards); i++) {
            Player *pg = (Player*)dynarray_get(point_guards, i);
            printf("  %s (%s) - %.1f skill\n", pg->name, pg->nationality, pg->skill_rating);
        }
    }
}

void demo_complex_queries(BasketballSystem *system) {
    printf("\n=== COMPLEX QUERY DEMONSTRATIONS ===\n");
    
    // 1. Elite players by nationality and position
    printf("\n1. Elite USA Point Guards (Skill > 90):\n");
    find_elite_players_by_nationality_and_position(system, "USA", "PG", 90.0f);
    
    printf("\n2. Elite Spanish Players (Skill > 85):\n");
    find_elite_players_by_nationality_and_position(system, "Spain", "PF", 85.0f);
    
    // 3. Age range queries
    printf("\n3. Young Players (Age 20-25):\n");
    find_players_in_age_range(system, 20, 25);
    
    printf("\n4. Veterans (Age 35+):\n");
    find_players_in_age_range(system, 35, 45);
}

void demo_trade_system(BasketballSystem *system) {
    printf("\n=== TRADE SYSTEM DEMONSTRATION ===\n");
    
    // Show initial team rosters
    printf("\n1. Initial Team Rosters:\n");
    Team *lakers = find_team_by_name(system, "Lakers");
    Team *warriors = find_team_by_name(system, "Warriors");
    
    if (lakers) print_team_info(system, lakers);
    if (warriors) print_team_info(system, warriors);
    
    // Request some trades
    printf("\n2. Requesting Trades:\n");
    Player *lebron = find_player_by_name(system, "LeBron James");
    Player *luka = find_player_by_name(system, "Luka Doncic");
    
    if (lebron) {
        request_trade(system, lebron->team_id, 2, lebron->player_id); // LeBron to Warriors
    }
    if (luka) {
        request_trade(system, luka->team_id, 3, luka->player_id);    // Luka to Celtics
    }
    
    // Process trades
    printf("\n3. Processing Trades:\n");
    process_next_trade(system);
    process_next_trade(system);
    
    // Show updated roster
    printf("\n4. Updated Warriors Roster:\n");
    if (warriors) print_team_info(system, warriors);
    
    // Demonstrate undo
    printf("\n5. Undo Last Trade:\n");
    undo_last_trade(system);
    
    printf("\n6. Warriors Roster After Undo:\n");
    if (warriors) print_team_info(system, warriors);
}

void demo_statistics_and_reporting(BasketballSystem *system) {
    printf("\n=== STATISTICS AND REPORTING ===\n");
    
    // System overview
    printf("\n1. System Statistics:\n");
    print_system_statistics(system);
    
    // Top players by skill
    printf("\n2. Top 5 Players by Skill:\n");
    print_top_players_by_skill(system, 5);
    
    // League information
    printf("\n3. League Information:\n");
    for (size_t i = 0; i < dynarray_size(&system->leagues); i++) {
        League *league = (League*)dynarray_get(&system->leagues, i);
        print_league_info(system, league);
    }
}

int main() {
    printf("=== BASKETBALL LEAGUE MANAGEMENT SYSTEM ===\n");
    printf("Demonstrating comprehensive data structure integration\n");
    
    // Initialize the system
    BasketballSystem system;
    basketball_system_init(&system);
    
    // Create sample data
    create_sample_data(&system);
    
    // Demonstrate different query types
    demo_basic_queries(&system);
    demo_complex_queries(&system);
    demo_trade_system(&system);
    demo_statistics_and_reporting(&system);
    
    // Performance demonstration
    printf("\n=== PERFORMANCE ANALYSIS ===\n");
    printf("All major operations demonstrated:\n");
    printf("✓ O(1) lookups by name and ID\n");
    printf("✓ O(1) access to extremes (youngest, oldest, best)\n");
    printf("✓ O(1) + O(k) filtered queries by attributes\n");
    printf("✓ O(m + n) complex intersection queries\n");
    printf("✓ FIFO trade processing with LIFO undo capability\n");
    printf("✓ Linear space complexity in number of players\n");
    
    printf("\nData structures used:\n");
    printf("• Hash Tables: Player/team lookups, specialized indices\n");
    printf("• Hash Sets: Fast membership testing for intersections\n");
    printf("• Min/Max Heaps: Instant access to extremes\n");
    printf("• Dynamic Arrays: Primary storage and collections\n");
    printf("• Stacks: Transaction history with undo\n");
    printf("• Queues: FIFO trade request processing\n");
    
    // Clean up
    basketball_system_free(&system);
    
    printf("\n=== DEMONSTRATION COMPLETED ===\n");
    printf("System successfully demonstrates real-world usage of\n");
    printf("integrated data structures for high-performance queries\n");
    printf("and complex operations in a basketball management system.\n");
    
    return 0;
}
