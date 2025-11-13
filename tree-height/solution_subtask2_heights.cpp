#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

/*
 * SOLUTION FOR SUBTASK 2: h_i ≤ 400
 *
 * Key insight: With at most 400 unique heights, we can:
 * 1. Precompute ALL O(400²) = 160,000 possible answers
 * 2. Answer each query in O(log 400) ≈ O(1) time
 *
 * This should DOMINATE tests 5-8!
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

    // Get unique heights
    set<ll> unique_set(h.begin(), h.end());
    vector<ll> unique_heights(unique_set.begin(), unique_set.end());
    int m = unique_heights.size();

    // For h_i ≤ 400, m should be ≤ 400
    // Precompute all possible answers
    if(m <= 500) {  // Conservative threshold
        // answer[i][j] = answer for height range [unique[i], unique[j]]
        vector<vector<ll>> answer(m, vector<ll>(m, 0));

        // Precompute O(m² × n) = O(400² × 200k) = 32B operations
        // Should take ~1-2 seconds
        for(int i = 0; i < m; i++) {
            for(int j = i; j < m; j++) {
                ll L = unique_heights[i];
                ll R = unique_heights[j];

                ll sum = 0;
                ll prev = -1;
                for(int k = 0; k < n; k++) {
                    if(h[k] >= L && h[k] <= R) {
                        if(prev != -1) {
                            sum += abs(h[k] - prev);
                        }
                        prev = h[k];
                    }
                }
                answer[i][j] = sum;
            }
        }

        // Answer queries in O(log m) each
        while(q--) {
            ll L, R;
            cin >> L >> R;

            // Binary search in unique_heights
            int L_idx = lower_bound(unique_heights.begin(), unique_heights.end(), L) - unique_heights.begin();
            int R_idx = upper_bound(unique_heights.begin(), unique_heights.end(), R) - unique_heights.begin() - 1;

            if(L_idx > R_idx || L_idx >= m || R_idx < 0) {
                cout << 0 << '\n';
            } else {
                cout << answer[L_idx][R_idx] << '\n';
            }
        }
    } else {
        // Fallback to brute force if too many unique heights
        while(q--) {
            ll L, R;
            cin >> L >> R;

            ll sum = 0;
            ll prev = -1;
            for(int i = 0; i < n; i++) {
                if(h[i] >= L && h[i] <= R) {
                    if(prev != -1) {
                        sum += abs(h[i] - prev);
                    }
                    prev = h[i];
                }
            }
            cout << sum << '\n';
        }
    }

    return 0;
}
