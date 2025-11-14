CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra
CXXFLAGS_ULTRA = -std=c++17 -O3 -Wall -Wextra

TARGETS = solution_optimized solution_advanced solution_ultra solution_sorted solution_ultra_fast solution_linear solution_mo_pbds solution_mo_optimized solution_mo_ultra

.PHONY: all clean test advanced mos

all: $(TARGETS)

solution_optimized: solution_optimized.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

solution_advanced: solution_advanced.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

solution_ultra: solution_ultra_optimized.cpp
	$(CXX) $(CXXFLAGS_ULTRA) $< -o $@

solution_sorted: solution_sorted_optimized.cpp
	$(CXX) $(CXXFLAGS_ULTRA) $< -o $@

solution_ultra_fast: solution_ultra_fast.cpp
	$(CXX) $(CXXFLAGS_ULTRA) -march=native $< -o $@ 2>/dev/null || $(CXX) $(CXXFLAGS_ULTRA) $< -o $@

solution_linear: solution_linear_scan.cpp
	$(CXX) $(CXXFLAGS_ULTRA) -march=native $< -o $@ 2>/dev/null || $(CXX) $(CXXFLAGS_ULTRA) $< -o $@

solution_mo_pbds: solution_mo_pbds.cpp
	$(CXX) $(CXXFLAGS_ULTRA) -march=native $< -o $@ 2>/dev/null || $(CXX) $(CXXFLAGS_ULTRA) $< -o $@

solution_mo_optimized: solution_mo_optimized.cpp
	$(CXX) $(CXXFLAGS_ULTRA) -march=native $< -o $@ 2>/dev/null || $(CXX) $(CXXFLAGS_ULTRA) $< -o $@

solution_mo_ultra: solution_mo_ultra.cpp
	$(CXX) $(CXXFLAGS_ULTRA) -march=native $< -o $@ 2>/dev/null || $(CXX) $(CXXFLAGS_ULTRA) $< -o $@

test: solution_optimized solution_advanced solution_ultra
	@echo "=== Testing Basic Solutions ==="
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

advanced: solution_sorted solution_ultra_fast solution_linear
	@echo "=== Testing Advanced Solutions ==="
	@echo "Testing solution_sorted..."
	@./solution_sorted < test_input.txt > test_output.txt
	@diff test_output.txt test_expected.txt && echo "✓ solution_sorted passed" || echo "✗ solution_sorted failed"
	@echo ""
	@echo "Testing solution_ultra_fast..."
	@./solution_ultra_fast < test_input.txt > test_output.txt
	@diff test_output.txt test_expected.txt && echo "✓ solution_ultra_fast passed" || echo "✗ solution_ultra_fast failed"
	@echo ""
	@echo "Testing solution_linear..."
	@./solution_linear < test_input.txt > test_output.txt
	@diff test_output.txt test_expected.txt && echo "✓ solution_linear passed" || echo "✗ solution_linear failed"
	@rm -f test_output.txt

mos: solution_mo_pbds solution_mo_optimized solution_mo_ultra
	@echo "=== Testing Mo's Algorithm Solutions ==="
	@echo "Testing solution_mo_pbds (GNU PBDS tree)..."
	@./solution_mo_pbds < test_input.txt > test_output.txt
	@diff test_output.txt test_expected.txt && echo "✓ solution_mo_pbds passed" || echo "✗ solution_mo_pbds failed"
	@echo ""
	@echo "Testing solution_mo_optimized (cached iterators)..."
	@./solution_mo_optimized < test_input.txt > test_output.txt
	@diff test_output.txt test_expected.txt && echo "✓ solution_mo_optimized passed" || echo "✗ solution_mo_optimized failed"
	@echo ""
	@echo "Testing solution_mo_ultra (extreme optimizations)..."
	@./solution_mo_ultra < test_input.txt > test_output.txt
	@diff test_output.txt test_expected.txt && echo "✓ solution_mo_ultra passed" || echo "✗ solution_mo_ultra failed"
	@rm -f test_output.txt

clean:
	rm -f $(TARGETS) test_output.txt

benchmark: $(TARGETS)
	@echo "Benchmarking all solutions with test input..."
	@echo ""
	@echo "solution_optimized (O(n*q) basic):"
	@time ./solution_optimized < test_input.txt > /dev/null
	@echo ""
	@echo "solution_sorted (O(q*k*log k) sorted):"
	@time ./solution_sorted < test_input.txt > /dev/null
	@echo ""
	@echo "solution_ultra_fast (O(q*k*log k) static arrays):"
	@time ./solution_ultra_fast < test_input.txt > /dev/null
	@echo ""
	@echo "solution_linear (O(n*q) hyper-optimized):"
	@time ./solution_linear < test_input.txt > /dev/null

help:
	@echo "Available targets:"
	@echo "  all       - Build all solutions (default)"
	@echo "  test      - Run tests on basic solutions"
	@echo "  advanced  - Run tests on advanced solutions"
	@echo "  mos       - Run tests on Mo's algorithm solutions"
	@echo "  benchmark - Benchmark all solutions"
	@echo "  clean     - Remove compiled binaries"
	@echo "  help      - Show this help message"
