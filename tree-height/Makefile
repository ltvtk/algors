CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra
CXXFLAGS_ULTRA = -std=c++17 -O3 -Wall -Wextra

TARGETS = solution_optimized solution_advanced solution_ultra

.PHONY: all clean test

all: $(TARGETS)

solution_optimized: solution_optimized.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

solution_advanced: solution_advanced.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

solution_ultra: solution_ultra_optimized.cpp
	$(CXX) $(CXXFLAGS_ULTRA) $< -o $@

test: $(TARGETS)
	@echo "Testing solution_optimized..."
	@./solution_optimized < test_input.txt > test_output.txt
	@diff test_output.txt test_expected.txt && echo "✓ solution_optimized passed" || echo "✗ solution_optimized failed"
	@echo ""
	@echo "Testing solution_advanced..."
	@./solution_advanced < test_input.txt > test_output.txt
	@diff test_output.txt test_expected.txt && echo "✓ solution_advanced passed" || echo "✗ solution_advanced failed"
	@echo ""
	@echo "Testing solution_ultra..."
	@./solution_ultra < test_input.txt > test_output.txt
	@diff test_output.txt test_expected.txt && echo "✓ solution_ultra passed" || echo "✗ solution_ultra failed"
	@rm -f test_output.txt

clean:
	rm -f $(TARGETS) test_output.txt

benchmark: $(TARGETS)
	@echo "Benchmarking solutions..."
	@echo "solution_optimized:"
	@time ./solution_optimized < test_input.txt > /dev/null
	@echo ""
	@echo "solution_advanced:"
	@time ./solution_advanced < test_input.txt > /dev/null
	@echo ""
	@echo "solution_ultra:"
	@time ./solution_ultra < test_input.txt > /dev/null

help:
	@echo "Available targets:"
	@echo "  all       - Build all solutions (default)"
	@echo "  test      - Run tests on all solutions"
	@echo "  benchmark - Benchmark all solutions"
	@echo "  clean     - Remove compiled binaries"
	@echo "  help      - Show this help message"
