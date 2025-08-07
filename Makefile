# Makefile for Data Structures Test Suite

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
QUICK_TEST = quick_test
COMPREHENSIVE_TEST = comprehensive_test

.PHONY: all quick-test full-test clean test-all help

# Default target
all: help

# Quick test - essential functionality
quick-test: $(QUICK_TEST)
	@echo "🚀 Running Quick Test Suite..."
	@./$(QUICK_TEST)

# Comprehensive test - all features
full-test: $(COMPREHENSIVE_TEST)
	@echo "🔍 Running Comprehensive Test Suite..."
	@./$(COMPREHENSIVE_TEST)

# Run both test suites
test-all: quick-test full-test
	@echo "✨ All tests completed!"

# Build executables
$(QUICK_TEST): quick_test.c
	$(CC) $(CFLAGS) -o $@ $<

$(COMPREHENSIVE_TEST): comprehensive_test.c
	$(CC) $(CFLAGS) -o $@ $<

# Clean up
clean:
	rm -f $(QUICK_TEST) $(COMPREHENSIVE_TEST)
	@echo "🧹 Cleaned up test executables"

# Help target
help:
	@echo "📋 Data Structures Test Suite"
	@echo "============================="
	@echo "Available targets:"
	@echo "  quick-test    - Run quick verification (27 tests)"
	@echo "  full-test     - Run comprehensive suite (124 tests)"
	@echo "  test-all      - Run both test suites"
	@echo "  clean         - Remove compiled executables"
	@echo "  help          - Show this help message"
	@echo ""
	@echo "Example usage:"
	@echo "  make quick-test     # Fast verification"
	@echo "  make full-test      # Detailed testing"
	@echo "  make test-all       # Run everything"
