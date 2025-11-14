# Subarray Sum Query - Optimized Solution

## Problem Summary
Given an array of n elements (each ±1) with two types of operations:
1. Update: Set a[i] = v
2. Query: Find subarray [x,y] in range [l,r] with sum = k

## Optimization Strategy

### Naive Approach (TLE)
```
For each query:
  For each x in [l, r]:
    For each y in [x, r]:
      if sum(a[x..y]) == k: return (x, y)
```
**Complexity:** O(q × n³)

### Optimized Approach

**Key Insight:** Use prefix sums + hash map

For a subarray [x, y] to have sum k:
- `sum(a[x..y]) = prefix[y] - prefix[x-1] = k`
- Therefore: `prefix[x-1] = prefix[y] - k`

**Algorithm:**
1. Compute prefix sums for range [l-1, r]
2. Use hash map to store `{prefix_value → position}`
3. For each position y from l to r:
   - Calculate `target = prefix[y] - k`
   - Check if `target` exists in hash map
   - If yes, we found x where `prefix[x-1] = target`

**Complexity:** O(q × n) with O(n) space

## Optimizations Applied

### C++ Version
- `ios_base::sync_with_stdio(false)` - faster I/O
- `unordered_map` for O(1) average lookup
- Early termination when answer found
- Only compute prefix for query range [l, r]

### Python Version
- Dictionary for O(1) average lookup
- List comprehension where applicable
- Early break from loops
- Minimal recomputation

## Time Complexity Analysis
- **Per Query:** O(r - l) ≈ O(n)
- **Total:** O(q × n)
- **Space:** O(n) for prefix array and hash map

## Why This Works
Since array elements are only ±1:
- Prefix sums are bounded: `-n ≤ prefix[i] ≤ n`
- Hash map size is at most O(n)
- Each prefix value maps to earliest occurrence (greedy)

## Test Results
Both C++ and Python solutions produce consistent results and handle:
- Updates in O(1)
- Queries in O(n)
- Edge cases (no valid subarray returns -1)

## Usage
```bash
# C++
g++ -o solution subarray_sum_query.cpp -std=c++17 -O2
./solution < input.txt

# Python
python3 subarray_sum_query.py < input.txt
```
