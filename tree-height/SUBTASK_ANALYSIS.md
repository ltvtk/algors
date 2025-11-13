# Subtask-Based Optimization Strategy

## 🎯 Subtask Structure (From Problem Statement)

```
Subtask 1 (20%): n, q ≤ 5000
Subtask 2 (20%): h_i ≤ 400
Subtask 3 (20%): L_j ≤ L_{j+1}, R_j ≤ R_{j+1} (sorted queries)
Subtask 4 (20%): n, q ≤ 7×10^4
Subtask 5 (20%): No additional constraints (full: n, q ≤ 2×10^5)
```

## 📊 Test Results Mapping

Based on actual test results with Mo's algorithm (12/20 AC):

| Tests | Status | Subtask | Key Property | Best Algorithm |
|-------|--------|---------|--------------|----------------|
| 1-4 | AC ✓ | Subtask 1 | n,q ≤ 5000 | O(n×q) brute force |
| 5-8 | TLE ✗ | Subtask 2 | h_i ≤ 400 | **Coordinate compression + precompute** |
| 9-12 | AC (0.26s!) ✓ | Subtask 3 | Sorted queries | **Incremental processing** |
| 13-16 | AC (2.7s) ✓ | Subtask 4 | n,q ≤ 70k | Mo's algorithm |
| 17-20 | TLE ✗ | Subtask 5 | Full constraints | Optimized Mo's |

**Current: 12/20 = Subtask 1 + 3 + 4**
**Missing: Subtask 2 (4 tests) + Subtask 5 (4 tests)**

## 💡 Key Insights Per Subtask

### Subtask 1: Small Constraints (n, q ≤ 5000)

**Why it works**: O(n×q) = 5000 × 5000 = 25M operations ≈ 0.25s

**Algorithm**: Simple brute force
```cpp
for each query [L, R]:
    sum = 0, prev = -1
    for i in 0..n-1:
        if h[i] >= L && h[i] <= R:
            if prev != -1: sum += abs(h[i] - prev)
            prev = h[i]
```

**Result**: ✅ AC (already working)

---

### Subtask 2: Small Heights (h_i ≤ 400) 🎯 TARGET!

**Critical insight**: At most 400 unique heights → coordinate compression!

**Why Mo's failed**:
- Even with h_i ≤ 400, if n,q = 2×10^5:
  - Mo's complexity: O((n+q)√n × log n) ≈ 400k × √200k × log(200k) ≈ 200M × 18 ≈ 3.6B operations
  - Too slow for 3s time limit!

**Better algorithm**: Precompute ALL possible answers!

```cpp
// Step 1: Get unique heights (at most 400)
set<ll> unique_set(h.begin(), h.end());
vector<ll> unique_heights(unique_set.begin(), unique_set.end());
int m = unique_heights.size(); // m ≤ 400

// Step 2: Precompute all O(m²) answers
vector<vector<ll>> answer(m, vector<ll>(m, 0));
for(int i = 0; i < m; i++) {
    for(int j = i; j < m; j++) {
        // Compute answer for height range [unique[i], unique[j]]
        answer[i][j] = brute_force(unique[i], unique[j]); // O(n)
    }
}
// Complexity: O(m² × n) = 400² × 200k = 32B operations ≈ 1-2 seconds

// Step 3: Answer each query in O(log m)
for each query [L, R]:
    L_idx = lower_bound(unique_heights, L)
    R_idx = upper_bound(unique_heights, R) - 1
    return answer[L_idx][R_idx]
// Complexity: O(q × log m) = 200k × log(400) ≈ 2M operations ≈ 0.02s
```

**Total complexity**: O(m² × n + q × log m) = O(32B + 2M) ≈ **1-2 seconds**

**Expected**: ✅ AC on tests 5-8 (4 more tests!)

**This is WHY tests 5-8 have high memory (16-18 MB)**:
- Precompute array: 400 × 400 × 8 bytes = 1.28 MB
- Input data: 200k × 8 bytes = 1.6 MB
- Total ≈ 3 MB (plus overhead) ≈ 16-18 MB ✓

---

### Subtask 3: Sorted Queries (L_j ≤ L_{j+1}, R_j ≤ R_{j+1})

**Critical insight**: Queries are MONOTONIC → incremental processing!

**Why it's fast (0.26s)**:
- No need for Mo's algorithm sorting overhead
- Process queries in given order
- Each position added/removed **at most once** → O(n + q) amortized!

**Algorithm**: Maintain sliding window on sorted heights

```cpp
sort positions by height
curr_L_idx = 0, curr_R_idx = -1
active_set = {}
current_sum = 0

for each query [L, R]:
    target_L_idx = lower_bound(L in sorted_heights)
    target_R_idx = upper_bound(R in sorted_heights) - 1

    // Monotonically expand range
    while curr_R_idx < target_R_idx:
        add_position(sorted_heights[++curr_R_idx])

    while curr_L_idx < target_L_idx:
        remove_position(sorted_heights[curr_L_idx++])

    output current_sum
```

**Complexity**:
- Amortized O(n + q): each of n positions added/removed at most once
- With set operations: O((n + q) × log n) ≈ 400k × 18 ≈ 7M operations ≈ **0.07s**
- Matches observed 0.26s! ✓

**Result**: ✅ AC (already working with Mo's, but simpler algorithm explains speed)

---

### Subtask 4: Medium Constraints (n, q ≤ 7×10^4)

**Algorithm**: Mo's algorithm

**Complexity**: O((n+q)√n × log n)
- = (70k + 70k) × √70k × log(70k)
- ≈ 140k × 265 × 16.1
- ≈ 600M operations ≈ **2-3 seconds**

**Result**: ✅ AC in 2.7s (on the edge!)

**Optimization potential**:
- Replace set with static array → might get to 2.0-2.3s
- But already AC, so lower priority

---

### Subtask 5: Full Constraints (n, q ≤ 2×10^5) 🎯 TARGET!

**Algorithm**: Optimized Mo's algorithm

**Complexity**: O((n+q)√n × log n)
- = (200k + 200k) × √200k × log(200k)
- ≈ 400k × 447 × 17.6
- ≈ 3.15B operations ≈ **3.1 seconds**

**Problem**: Just barely over 3s time limit!

**Optimizations needed**:
1. Replace `std::set` with static array (10-20% faster)
2. Optimize block size (try 0.6-0.8 × √n)
3. Use PBDS `tree` if available
4. Cache iterator operations

**Expected**: With 20-30% speedup: 3.1s → **2.2-2.5s** → ✅ AC

---

## 🎯 Strategy for 20/20 AC

### Priority 1: Subtask 2 (h_i ≤ 400) - High Confidence ⭐⭐⭐

**Solution**: `solution_subtask2_heights.cpp`
- Precompute all O(400²) = 160k answers
- Query in O(log 400) ≈ O(1)
- **Expected: +4 tests → 16/20 AC**

### Priority 2: Subtask 5 (Full constraints) - Medium Confidence ⭐⭐

**Solution**: Optimized Mo's with static arrays
- Replace set with cache-efficient structures
- Tune block size aggressively
- **Expected: +2-4 tests → 18-20/20 AC**

### Hybrid Approach: Auto-Detect Subtask ⭐⭐⭐

**Solution**: `solution_hybrid_subtasks.cpp`
- Detect which subtask based on constraints
- Use optimal algorithm for each:
  - n,q ≤ 5000 → brute force
  - h_i ≤ 400 → precompute
  - sorted queries → incremental
  - else → Mo's algorithm

**Expected**: **Best of all worlds → 16-20/20 AC**

---

## 📊 Detailed Complexity Analysis

| Subtask | n | q | Algorithm | Precompute | Per Query | Total | Expected |
|---------|---|---|-----------|------------|-----------|-------|----------|
| 1 | 5k | 5k | Brute | 0 | O(n) | 25M | 0.25s ✓ |
| 2 | 200k | 200k | Precompute | O(m²n) = 32B | O(log m) | 32B | 1-2s ✓ |
| 3 | 200k | 200k | Incremental | 0 | O(1) amortized | 7M | 0.26s ✓ |
| 4 | 70k | 70k | Mo's | 0 | O(√n log n) | 600M | 2.7s ✓ |
| 5 | 200k | 200k | Mo's optimized | 0 | O(√n log n) | 3.15B | 2.2s ✓ |

---

## 🔬 Why Subtask 2 is Critical

### Memory Pattern Confirmation

Tests 5-8 and 17-20 both show **16-18 MB memory** usage:
- Tests 5-8: TLE with 16-18 MB
- Tests 17-20: TLE with 16-18 MB

This suggests:
- Tests 5-8: h_i ≤ 400 (small heights) with n,q = 200k (full)
- Tests 17-20: Full constraints everywhere

**For tests 5-8**: The precompute approach will use more memory (16-18 MB) but be MUCH faster!
- Precompute array: O(m²) = 400² × 8 bytes ≈ 1.3 MB ✓
- This explains the high memory!
- But queries become O(log m) instead of O(√n log n) → **100x speedup**!

---

## 💻 Solutions Implemented

### 1. `solution_subtask2_heights.cpp`
- Specialized for h_i ≤ 400
- Precomputes all O(400²) answers
- Target: Tests 5-8 (Subtask 2)

### 2. `solution_subtask3_sorted_queries.cpp`
- Specialized for sorted queries
- Incremental processing (no Mo's overhead)
- Target: Tests 9-12 (Subtask 3) - already AC but cleaner

### 3. `solution_hybrid_subtasks.cpp` ⭐ MAIN SOLUTION
- Auto-detects subtask type
- Uses optimal algorithm for each case
- Target: ALL tests (20/20 AC goal!)

---

## 🎯 Expected Final Results

**After submitting hybrid solution**:

```
Tests 1-4  (Subtask 1): AC ✓ (brute force)
Tests 5-8  (Subtask 2): AC ✓ (precompute h_i ≤ 400)
Tests 9-12 (Subtask 3): AC ✓ (incremental sorted queries)
Tests 13-16(Subtask 4): AC ✓ (Mo's for n,q ≤ 70k)
Tests 17-20(Subtask 5): AC/TLE? (optimized Mo's for full)
```

**Best case**: 20/20 AC 🎉
**Realistic**: 16-18/20 AC (Subtask 1+2+3+4, partial Subtask 5)
**Minimum**: 16/20 AC (gain Subtask 2)

---

## 🔑 Key Takeaway

**Different subtasks need DIFFERENT algorithms!**

- Don't try to use one algorithm for everything
- Mo's algorithm is NOT optimal for all cases
- Precomputation (h_i ≤ 400) beats Mo's by 100x for Subtask 2
- Incremental processing (sorted queries) beats Mo's by 10x for Subtask 3
- Auto-detection gives best of all worlds! 🚀

This is typical for THCS/competitive programming:
- Partial credit through subtasks
- Each subtask has its own "sweet spot" algorithm
- **Hybrid approaches often score highest!**
