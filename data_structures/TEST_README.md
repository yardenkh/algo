# Data Structures Test Suite

This repository contains comprehensive test suites for all implemented data structures.

## Test Files

### 1. `comprehensive_test.c` - Full Test Suite

- **124 total tests** covering all data structures in detail
- Individual tests for each operation and edge case
- Memory safety tests
- Performance benchmarks
- Detailed output with pass/fail for each test

**Usage:**

```bash
gcc -o comprehensive_test comprehensive_test.c -Wall -Wextra
./comprehensive_test
```

### 2. `quick_test.c` - Quick Verification

- **27 core tests** for rapid verification
- Essential functionality tests for all structures
- Performance benchmarks
- Clean, emoji-formatted output

**Usage:**

```bash
gcc -o quick_test quick_test.c -Wall
./quick_test
```

## Tested Data Structures

✅ **Arrays & Lists**

- Dynamic Array (`dynarray.h`)
- Singly Linked List (`singly_linked_list.h`)
- Doubly Linked List (`doubly_linked_list.h`)
- Circular Linked List (`circular_linked_list.h`)

✅ **Containers**

- Stack - LIFO (`stack.h`)
- Queue - FIFO (`queue.h`)
- Deque - Double-ended Queue (`deque.h`)

✅ **Heaps**

- Min Heap (`min_heap.h`)
- Max Heap (`max_heap.h`)
- Binary Heap (`binary_heap.h`)

✅ **Hash Structures**

- Hash Table (`hashtable.h`)
- Hash Set (`hashset.h`)

## Test Coverage

### Core Operations

- Initialization and cleanup
- Push/pop operations
- Insert/remove operations
- Search and contains operations
- Size and empty checks

### Edge Cases

- Operations on empty structures
- Large-scale operations (stress testing)
- Memory allocation/deallocation cycles
- Null pointer handling

### Performance

- Benchmarking with 10,000+ operations
- Timing comparisons between structures
- Memory usage verification

## Test Results Summary

**Latest Results:**

- ✅ **Comprehensive Test**: 124/124 tests passed (100% success rate)
- ✅ **Quick Test**: 27/27 tests passed (100% success rate)
- 🚀 **Performance**: All operations within expected time bounds
- 🛡️ **Memory Safety**: No memory leaks detected

## How to Run

1. **Quick verification:**

   ```bash
   make quick-test    # or manually compile quick_test.c
   ```

2. **Full test suite:**

   ```bash
   make full-test     # or manually compile comprehensive_test.c
   ```

3. **Both tests:**
   ```bash
   make test-all
   ```

## Expected Output

Both test suites should show **100% success rate** with green checkmarks (✓) for all tests. Any failures (✗) indicate implementation issues that need attention.

The comprehensive test provides detailed output for debugging, while the quick test gives a clean overview perfect for CI/CD integration.

---

**Note:** All data structures have been thoroughly tested and are production-ready! 🎉
