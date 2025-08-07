# Basketball Management System Implementation

A complete implementation of the Basketball League Management System from the data structures guide, demonstrating real-world usage of integrated data structures for high-performance operations.

## 🏗️ Architecture

### Core Components

- **`basketball_system.h`** - System interface and data structures
- **`basketball_system.c`** - Complete implementation with all operations
- **`basketball_demo.c`** - Comprehensive demonstration program
- **`DATA_STRUCTURES_GUIDE.md`** - Detailed theoretical guide

### Data Structures Used

1. **Hash Tables** (`hashtable.h`) - O(1) lookups by name/ID
2. **Hash Sets** (`hashset.h`) - O(1) membership testing for intersections
3. **Min/Max Heaps** (`min_heap.h`, `max_heap.h`) - O(1) extremes access
4. **Dynamic Arrays** (`dynarray.h`) - Primary storage and collections
5. **Stacks** (`stack.h`) - Transaction history with undo capability
6. **Queues** (`queue.h`) - FIFO trade request processing

## 🚀 Quick Start

```bash
# Build and run the demo
make run-demo

# Build only
make basketball

# Run manually
./basketball_demo

# Clean up
make clean
```

## 📊 Features Demonstrated

### Basic Operations (O(1))

- Find player by name or ID
- Get youngest/oldest/most skilled players instantly
- Access specialized indices (by nationality, position, team)

### Complex Queries

- Elite players by nationality AND position with skill filter
- Players in specific age/height ranges
- Set intersection operations for complex filtering

### Trade System

- FIFO queue for trade request processing
- LIFO stack for transaction history and undo
- Real-time roster updates across multiple indices

### Statistics & Reporting

- System overview with performance metrics
- Top players rankings
- Team and league information display

## 🎯 Performance Characteristics

| Operation                  | Data Structure      | Time Complexity |
| -------------------------- | ------------------- | --------------- |
| Player lookup by name      | Hash Table          | O(1)            |
| Player lookup by ID        | Hash Table          | O(1)            |
| Get youngest player        | Min Heap            | O(1)            |
| Get players by nationality | Hash Table + Array  | O(1) + O(k)     |
| Add new player             | Multiple structures | O(log n)        |
| Complex filtered query     | Hash Set + Arrays   | O(m + n)        |
| Process trade              | Queue + Updates     | O(1) + updates  |

**Space Complexity**: O(P) linear in number of players

## 💡 Key Design Patterns

### Multi-Index Architecture

- Primary storage in dynamic arrays
- Multiple hash table indices for different query patterns
- Specialized heaps for performance queries
- Consistent updates across all indices

### Transaction Management

- Queue-based trade processing (FIFO)
- Stack-based undo system (LIFO)
- Atomic updates with rollback capability

### Generic Interface Usage

- String and integer hash tables with proper key management
- Type-safe operations through the hash function interface
- Memory management handled by data structure implementations

## 🧪 Testing

The system includes comprehensive testing:

```bash
# Run data structure tests
make test-all        # All 151 tests
make quick-test      # Essential 27 tests
make full-test       # Comprehensive 124 tests
```

## 🏀 Sample Output

```
=== BASKETBALL LEAGUE MANAGEMENT SYSTEM ===
Demonstrating comprehensive data structure integration

Added league NBA (ID: 1) to system
Added team Lakers (ID: 1) to system
Added player LeBron James (ID: 1) to system

=== BASIC QUERY DEMONSTRATIONS ===
1. Find Player by Name: LeBron James - 95.5 skill
2. Performance Extremes: Most Skilled: Stephen Curry (96.2)
3. USA Players: LeBron James, Stephen Curry, Jayson Tatum...

=== COMPLEX QUERY DEMONSTRATIONS ===
Elite USA Point Guards (Skill > 90):
1. Stephen Curry - Age: 35, Skill: 96.2, Team ID: 2

=== TRADE SYSTEM DEMONSTRATION ===
Trade completed: LeBron James moved from Team 1 to Team 2
Trade undone: Player returned to original team
```

## 🔧 Dependencies

Requires all data structure headers:

- `dynarray/dynarray.h`
- `hash/hashtable.h` & `hash/hashset.h`
- `heap/min_heap.h` & `heap/max_heap.h`
- `linkedlist/doubly_linked_list.h`
- `containers/stack.h` & `containers/queue.h`

## 📈 Real-World Applications

This implementation demonstrates patterns applicable to:

- **Sports Management Systems** - Player tracking, team management
- **Inventory Systems** - Multi-dimensional product indexing
- **User Management** - Profile lookups with complex filtering
- **Financial Systems** - Transaction processing with audit trails
- **Gaming Systems** - Player statistics and leaderboards

## 🎓 Educational Value

Perfect example of:

- **Data Structure Integration** - How different structures work together
- **Performance Engineering** - Achieving O(1) operations through smart indexing
- **System Design** - Balancing functionality, performance, and maintainability
- **Memory Management** - Proper allocation/deallocation patterns
- **API Design** - Clean, intuitive interfaces for complex operations

The complete system successfully demonstrates enterprise-grade performance with educational clarity, making it ideal for understanding advanced data structure applications in real-world software systems.
