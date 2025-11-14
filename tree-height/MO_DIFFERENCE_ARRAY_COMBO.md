# Mo's Algorithm + Mảng Hiệu (Difference Array) - Hybrid Approach

## 🎯 Ý Tưởng Chính

Kết hợp **Mo's Algorithm** (đã đạt 12/20 AC) với **Mảng Hiệu** (kỹ thuật THCS cơ bản) để tối ưu thêm!

## 💡 Key Insight: Tại Sao Kết Hợp?

### Vấn đề của Mo's Algorithm Hiện Tại:
- Mỗi `add(pos)` hoặc `remove(pos)` cần:
  - Tìm `prev` và `next` trong set: **O(log n)**
  - Update sum dựa trên 3 giá trị: h[pos], h[prev], h[next]
- Với 200k queries, có thể cần **hàng triệu operations** với set

### Mảng Hiệu Concept:
- **Mảng hiệu cơ bản**: `diff[i] = a[i] - a[i-1]`
- **Prefix sum**: `sum(L, R) = prefix[R] - prefix[L-1]`
- **Incremental updates**: Khi thêm/bớt phần tử, chỉ affect **local differences**!

### Kết Hợp:
Thay vì dùng `std::set` động, **precompute cấu trúc** để optimize add/remove operations!

## 🔧 Ba Variants Được Implement

### Variant 1: `solution_mo_difference_array.cpp`

**Approach**: Maintain active positions in sorted array thay vì set

```cpp
// Instead of: set<int> active;
vector<int> active_positions; // Sorted by original position

void add_position(int pos) {
    // Binary search for insertion point
    auto it = lower_bound(active_positions.begin(), active_positions.end(), pos);

    // Calculate contribution (mảng hiệu concept!)
    ll contribution = 0;
    if(it != active_positions.begin()) {
        int prev_pos = *prev(it);
        contribution += abs(h[pos] - h[prev_pos]);
    }
    if(it != active_positions.end()) {
        int next_pos = *it;
        contribution += abs(h[pos] - h[next_pos]);

        // Crucially: Remove old edge between prev and next
        if(it != active_positions.begin()) {
            int prev_pos = *prev(it);
            current_sum -= abs(h[prev_pos] - h[next_pos]);
        }
    }

    current_sum += contribution;
    active_positions.insert(it, pos);
}
```

**Optimization**:
- Vector insert can be slower than set for large sizes
- But better cache locality
- Binary search is same O(log n)

**Expected improvement**: Small (5-10%), but more predictable performance

---

### Variant 2: `solution_mo_precomputed_diff.cpp` ⭐⭐

**Approach**: Use **static array** instead of dynamic vector/set

```cpp
// Static array - much faster!
int active_list[MAXN];
int active_count = 0;
bool is_active[MAXN];

void add_position(int pos) {
    int insert_idx = find_insert_pos(pos); // Binary search

    // Calculate contribution
    ll contribution = 0;
    if(insert_idx > 0) {
        int prev_pos = active_list[insert_idx - 1];
        contribution += abs(h[pos] - h[prev_pos]);
    }
    if(insert_idx < active_count) {
        int next_pos = active_list[insert_idx];
        contribution += abs(h[pos] - h[next_pos]);

        // Remove old edge (mảng hiệu insight!)
        if(insert_idx > 0) {
            int prev_pos = active_list[insert_idx - 1];
            current_sum -= abs(h[prev_pos] - h[next_pos]);
        }
    }

    current_sum += contribution;

    // Shift array and insert - O(n) worst case but fast in practice
    for(int i = active_count; i > insert_idx; i--) {
        active_list[i] = active_list[i - 1];
    }
    active_list[insert_idx] = pos;
    active_count++;
}
```

**Optimizations**:
- **Static array**: No dynamic allocation overhead
- **Simple loop**: Better than vector::insert for compiler optimization
- **Cache-friendly**: Contiguous memory
- **bool array**: Faster than set::find() for checking membership

**Expected improvement**: **10-20% faster** than set-based Mo's

---

### Variant 3: `solution_mo_cached_neighbors.cpp` ⭐⭐⭐

**Approach**: **Precompute potential neighbors** + ultra-optimized operations

```cpp
// Precompute: for each position, cache nearby positions
struct CachedInfo {
    int left_neighbors[10];  // Potential left neighbors
    int right_neighbors[10]; // Potential right neighbors
    int ln_count, rn_count;
};
CachedInfo cached[MAXN];

void precompute_neighbors() {
    for(int i = 0; i < n; i++) {
        // Cache up to 10 positions to the left
        for(int j = i - 1; j >= 0 && cached[i].ln_count < 10; j--) {
            cached[i].left_neighbors[cached[i].ln_count++] = j;
        }
        // Similar for right
    }
}

// Use bitset for O(1) active checking
bitset<MAXN> is_active;

// Aligned array for better cache performance
alignas(64) int active_arr[MAXN];

// Use memmove for fast array shifting
memmove(&active_arr[idx + 1], &active_arr[idx],
        (active_cnt - idx) * sizeof(int));
```

**Optimizations**:
- **Cached neighbors**: Exploit locality - neighbors often nearby in original array
- **bitset**: O(1) membership checking, cache-efficient
- **Memory alignment**: 64-byte alignment for cache line optimization
- **memmove**: Compiler-optimized memory operations
- **Aggressive block size**: 0.65×√n instead of 1.0×√n

**Expected improvement**: **20-30% faster** than set-based Mo's

**Why this might break through 12/20**:
- Tests 13-16 AC at 2.7s (close to 3s limit)
- 20-30% speedup → might get to ~2.1s → **AC**!
- Tests 5-8, 17-20 might need different approach still

---

## 📊 Comparison

| Solution | Key Technique | Expected Speedup | Complexity (per add/remove) |
|----------|---------------|------------------|----------------------------|
| Original Mo's | std::set | Baseline (12/20) | O(log n) |
| V1: vector | Sorted vector | +5-10% | O(log n) search + O(k) insert |
| V2: static array | Static array | +10-20% | O(log n) search + O(k) shift |
| V3: cached | Precompute + cache | +20-30% | O(log n) + optimized constants |

## 🎯 Strategy

### Priority Order for Testing:

1. **V3: cached neighbors** ⭐⭐⭐
   - Most aggressive optimizations
   - Might push tests 13-16 under 3s
   - Expected: 13-14/20 AC (gain 1-2 tests)

2. **V2: static array** ⭐⭐
   - Simpler but effective
   - More predictable performance
   - Expected: 12-13/20 AC (gain 0-1 tests)

3. **V1: vector** ⭐
   - Baseline improvement
   - Might not be enough
   - Expected: 12/20 AC (same as current)

### Analysis Based on Results:

**If V3 gets 13-14/20**:
- Confirms that constant optimization matters
- Tests 13-16 were indeed on the edge
- Need different approach for tests 5-8, 17-20

**If V3 still gets 12/20**:
- Constant optimization not enough
- Need algorithmic improvement
- Check prefix sum approaches or other observations

## 🔬 Why Mảng Hiệu Concept Helps

### Core Insight:
Khi maintain sorted sequence of positions, mỗi thay đổi chỉ affect **at most 2 edges**!

```
Before: ... [A] --- [C] ...
          edge: |h[A] - h[C]|

After adding B: ... [A] --- [B] --- [C] ...
          edges: |h[A] - h[B]| + |h[B] - h[C]|

Change = |h[A] - h[B]| + |h[B] - h[C]| - |h[A] - h[C]|
       = local difference update!
```

Đây chính là **mảng hiệu concept**:
- Thay đổi local không affect toàn bộ array
- Chỉ update differences của neighbors trực tiếp
- **Incremental updates** instead of recalculation

### Kết hợp với Mo's:
- Mo's cho efficient query ordering: O((n+q)√n) queries
- Mảng hiệu cho efficient per-operation updates: optimized constants
- **Together**: Better performance across large query sets!

## 💭 Additional Optimizations Applied

### 1. Block Size Tuning
```cpp
int block_size = max(1, (int)(sqrt(n) * 0.65)); // Aggressive
```
- Smaller blocks → more sorting overhead, less movement
- Testing 0.65×√n instead of 1.0×√n

### 2. Compiler Optimizations
```cpp
#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
```
- Loop unrolling for array shifts
- SIMD instructions for memory operations

### 3. Cache Optimization
- 64-byte alignment for arrays
- Contiguous memory access patterns
- Minimize pointer chasing

## 🏆 Expected Outcomes

### Best Case (V3):
- Tests 1-4: AC (small) ✓
- Tests 5-8: TLE (need different approach) ✗
- Tests 9-12: AC (0.26s - already fast) ✓
- **Tests 13-16: AC (was 2.7s → now ~2.1s)** ✓✓
- Tests 17-20: TLE (need different approach) ✗
- **Total: 13-14/20 AC** (gain 1-2 tests)

### Realistic Case (V2/V3):
- **Tests 13-14: AC** (border tests)
- **Tests 15-16: Still close to 3s**
- **Total: 13/20 AC** (gain 1 test)

### Worst Case:
- Still 12/20 AC
- Need to explore different algorithmic approaches
- But learned valuable optimization techniques!

## 🔄 Next Steps After Testing

**If successful (13+/20)**:
- Combine with prefix sum approaches for tests 5-8, 17-20
- Try hybrid: Mo's for some tests, precompute for others

**If not successful (still 12/20)**:
- Focus on prefix sum v2 (coordinate compression)
- Look for mathematical patterns in failing tests
- Consider test-specific approaches

## 📝 Key Takeaway

**Mảng hiệu + Mo's = Incremental updates + Efficient ordering**

Đây là sự kết hợp của:
- **THCS technique** (mảng hiệu - difference array)
- **Advanced algorithm** (Mo's algorithm)
- **Low-level optimizations** (cache, alignment, static arrays)

Result: **Practical speedup** that might push us over the edge for border test cases! 🚀
