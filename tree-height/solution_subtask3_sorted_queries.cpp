#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

/*
 * SOLUTION FOR SUBTASK 3: L_j ≤ L_{j+1}, R_j ≤ R_{j+1}
 *
 * Key insight: Queries are SORTED!
 * - L values are non-decreasing
 * - R values are non-decreasing
 *
 * We can process INCREMENTALLY:
 * 1. Start with empty range
 * 2. For each query, expand L and R monotonically
 * 3. No need for Mo's algorithm sorting overhead!
 *
 * This explains the 0.26s speed - MUCH simpler than Mo's!
 *
 * Complexity: O(n + q) amortized - each position added/removed at most once!
 */

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<ll> h(n);
    for(int i = 0; i < n; i++) {
        cin >> h[i];
    }

    // Sort positions by height for range queries
    vector<pair<ll, int>> sorted_h(n);
    for(int i = 0; i < n; i++) {
        sorted_h[i] = {h[i], i};
    }
    sort(sorted_h.begin(), sorted_h.end());

    // Incremental processing
    int curr_L_idx = 0;  // Current left boundary in sorted_h
    int curr_R_idx = -1; // Current right boundary in sorted_h
    ll curr_L_val = 0, curr_R_val = -1; // Current L and R values

    // Active positions (in original order)
    set<int> active;
    ll current_sum = 0;

    // Helper to add position
    auto add_position = [&](int pos) {
        auto it = active.insert(pos).first;

        ll contribution = 0;

        // Check previous
        if(it != active.begin()) {
            auto prev_it = prev(it);
            contribution += abs(h[pos] - h[*prev_it]);
        }

        // Check next
        auto next_it = next(it);
        if(next_it != active.end()) {
            contribution += abs(h[pos] - h[*next_it]);

            // Remove old edge between prev and next
            if(it != active.begin()) {
                auto prev_it = prev(it);
                current_sum -= abs(h[*prev_it] - h[*next_it]);
            }
        }

        current_sum += contribution;
    };

    // Process queries incrementally
    for(int query_idx = 0; query_idx < q; query_idx++) {
        ll L, R;
        cin >> L >> R;

        // Find range in sorted_h
        int target_L_idx = lower_bound(sorted_h.begin(), sorted_h.end(), make_pair(L, -1)) - sorted_h.begin();
        int target_R_idx = upper_bound(sorted_h.begin(), sorted_h.end(), make_pair(R, n)) - sorted_h.begin() - 1;

        if(target_L_idx > target_R_idx) {
            cout << 0 << '\n';
            continue;
        }

        // If this is first query or range completely different, reset
        if(query_idx == 0 || target_L_idx > curr_R_idx || target_R_idx < curr_L_idx) {
            // Reset
            active.clear();
            current_sum = 0;
            curr_L_idx = target_L_idx;
            curr_R_idx = target_L_idx - 1;
        }

        // Expand R (monotonically increasing)
        while(curr_R_idx < target_R_idx) {
            curr_R_idx++;
            add_position(sorted_h[curr_R_idx].second);
        }

        // Shrink R if needed (shouldn't happen in sorted queries, but handle it)
        while(curr_R_idx > target_R_idx) {
            // Remove position
            int pos = sorted_h[curr_R_idx].second;
            auto it = active.find(pos);

            ll contribution = 0;
            if(it != active.begin()) {
                auto prev_it = prev(it);
                contribution += abs(h[pos] - h[*prev_it]);
            }
            auto next_it = next(it);
            if(next_it != active.end()) {
                contribution += abs(h[pos] - h[*next_it]);
                if(it != active.begin()) {
                    auto prev_it = prev(it);
                    current_sum += abs(h[*prev_it] - h[*next_it]);
                }
            }
            current_sum -= contribution;
            active.erase(it);

            curr_R_idx--;
        }

        // Shrink L (monotonically increasing)
        while(curr_L_idx < target_L_idx) {
            // Remove position
            int pos = sorted_h[curr_L_idx].second;
            auto it = active.find(pos);

            ll contribution = 0;
            if(it != active.begin()) {
                auto prev_it = prev(it);
                contribution += abs(h[pos] - h[*prev_it]);
            }
            auto next_it = next(it);
            if(next_it != active.end()) {
                contribution += abs(h[pos] - h[*next_it]);
                if(it != active.begin()) {
                    auto prev_it = prev(it);
                    current_sum += abs(h[*prev_it] - h[*next_it]);
                }
            }
            current_sum -= contribution;
            active.erase(it);

            curr_L_idx++;
        }

        // Expand L if needed (shouldn't happen in sorted queries)
        while(curr_L_idx > target_L_idx) {
            curr_L_idx--;
            add_position(sorted_h[curr_L_idx].second);
        }

        cout << current_sum << '\n';
    }

    return 0;
}
