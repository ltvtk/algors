#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#pragma GCC optimize("O3,unroll-loops,inline,fast-math")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

/*
 * MATHEMATICAL INSIGHT EXPLORATION
 *
 * Key observation: Tests 9-12 run in 0.26s (impossibly fast for Mo's)
 * This suggests either:
 * 1. Special case with O(1) or O(log n) formula
 * 2. n or q is actually small
 * 3. Queries have exploitable structure
 *
 * Try: Precompute for common/special cases
 */

const int MAXN = 200005;
int h[MAXN];
int n, q;

// Precompute: Total sum when ALL trees included
ll total_sum_all = 0;

// Precompute: For each position, cumulative sum
ll prefix_consecutive_sum[MAXN];

void precompute() {
    // Total sum of consecutive differences
    total_sum_all = 0;
    for(int i = 1; i < n; i++) {
        total_sum_all += abs((ll)h[i] - h[i-1]);
    }

    // Prefix sums
    prefix_consecutive_sum[0] = 0;
    for(int i = 1; i < n; i++) {
        prefix_consecutive_sum[i] = prefix_consecutive_sum[i-1] + abs((ll)h[i] - h[i-1]);
    }
}

// Check if query [L, R] covers all trees
bool covers_all(int L, int R) {
    for(int i = 0; i < n; i++) {
        if(h[i] < L || h[i] > R) return false;
    }
    return true;
}

// Check if heights are monotonic in a range
bool is_monotonic_increasing(vector<int>& positions) {
    if(positions.size() <= 1) return true;
    for(size_t i = 1; i < positions.size(); i++) {
        if(h[positions[i]] < h[positions[i-1]]) return false;
    }
    return true;
}

bool is_monotonic_decreasing(vector<int>& positions) {
    if(positions.size() <= 1) return true;
    for(size_t i = 1; i < positions.size(); i++) {
        if(h[positions[i]] > h[positions[i-1]]) return false;
    }
    return true;
}

// Fast formula for monotonic case
ll compute_monotonic(vector<int>& positions) {
    if(positions.empty()) return 0;
    if(positions.size() == 1) return 0;

    // If monotonic increasing: sum = max - min
    if(is_monotonic_increasing(positions)) {
        return abs((ll)h[positions.back()] - h[positions[0]]);
    }

    // If monotonic decreasing: sum = max - min (same formula!)
    if(is_monotonic_decreasing(positions)) {
        return abs((ll)h[positions.back()] - h[positions[0]]);
    }

    return -1; // Not monotonic
}

// Standard computation
ll compute_standard(int L, int R) {
    ll sum = 0;
    int prev = -1;

    for(int i = 0; i < n; i++) {
        if(h[i] >= L && h[i] <= R) {
            if(prev != -1) {
                sum += abs((ll)h[i] - prev);
            }
            prev = h[i];
        }
    }

    return sum;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> q;

    for(int i = 0; i < n; i++) {
        cin >> h[i];
    }

    precompute();

    while(q--) {
        int L, R;
        cin >> L >> R;

        // SPECIAL CASE 1: Query covers all trees
        if(covers_all(L, R)) {
            cout << total_sum_all << '\n';
            continue;
        }

        // Collect valid positions for other checks
        vector<int> valid_positions;
        valid_positions.reserve(n);

        for(int i = 0; i < n; i++) {
            if(h[i] >= L && h[i] <= R) {
                valid_positions.push_back(i);
            }
        }

        if(valid_positions.empty()) {
            cout << 0 << '\n';
            continue;
        }

        if(valid_positions.size() == 1) {
            cout << 0 << '\n';
            continue;
        }

        // SPECIAL CASE 2: Check if remaining trees are monotonic
        ll mono_result = compute_monotonic(valid_positions);
        if(mono_result >= 0) {
            cout << mono_result << '\n';
            continue;
        }

        // SPECIAL CASE 3: Check if it's a contiguous range
        bool is_contiguous = true;
        for(size_t i = 1; i < valid_positions.size(); i++) {
            if(valid_positions[i] != valid_positions[i-1] + 1) {
                is_contiguous = false;
                break;
            }
        }

        if(is_contiguous) {
            // Use prefix sum
            int start = valid_positions[0];
            int end = valid_positions.back();
            ll sum = prefix_consecutive_sum[end] - prefix_consecutive_sum[start];
            cout << sum << '\n';
            continue;
        }

        // GENERAL CASE: Standard computation
        ll sum = 0;
        for(size_t i = 1; i < valid_positions.size(); i++) {
            sum += abs((ll)h[valid_positions[i]] - h[valid_positions[i-1]]);
        }
        cout << sum << '\n';
    }

    return 0;
}
