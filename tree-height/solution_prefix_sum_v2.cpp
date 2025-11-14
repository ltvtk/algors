#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

/*
 * PREFIX SUM APPROACH 2: Coordinate compression + 2D prefix
 *
 * Key insight for THCS:
 * - Compress heights to unique values
 * - For small number of unique heights, precompute answers
 * - Use 2D prefix sum: prefix[L_idx][R_idx] = answer for range
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

    // If unique heights is small, precompute all answers
    if(m <= 300) {
        // Precompute: answer[i][j] = answer for height range [unique[i], unique[j]]
        vector<vector<ll>> answer(m, vector<ll>(m, 0));

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

        // Answer queries using precomputed values
        while(q--) {
            ll L, R;
            cin >> L >> R;

            // Find range in unique_heights
            int L_idx = lower_bound(unique_heights.begin(), unique_heights.end(), L) - unique_heights.begin();
            int R_idx = upper_bound(unique_heights.begin(), unique_heights.end(), R) - unique_heights.begin() - 1;

            if(L_idx > R_idx || L_idx >= m || R_idx < 0) {
                cout << 0 << '\n';
            } else {
                cout << answer[L_idx][R_idx] << '\n';
            }
        }
    } else {
        // Too many unique heights, use brute force
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
