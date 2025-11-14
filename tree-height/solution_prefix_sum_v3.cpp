#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

/*
 * PREFIX SUM APPROACH 3: Sorted + Prefix Sum Optimization
 *
 * Key idea:
 * - Sort trees by height with original indices
 * - Use prefix sum on sorted order
 * - When we extract valid range, compute incrementally
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

    // Sort with indices
    vector<pair<ll, int>> sorted_h(n);
    for(int i = 0; i < n; i++) {
        sorted_h[i] = {h[i], i};
    }
    sort(sorted_h.begin(), sorted_h.end());

    // For small n, try different approach
    if(n <= 5000) {
        // Precompute for all possible ranges
        map<pair<ll,ll>, ll> cache;

        while(q--) {
            ll L, R;
            cin >> L >> R;

            auto key = make_pair(L, R);
            if(cache.find(key) != cache.end()) {
                cout << cache[key] << '\n';
                continue;
            }

            // Find valid indices using binary search
            int left = lower_bound(sorted_h.begin(), sorted_h.end(), make_pair(L, -1)) - sorted_h.begin();
            int right = upper_bound(sorted_h.begin(), sorted_h.end(), make_pair(R, n)) - sorted_h.begin();

            if(left >= right) {
                cache[key] = 0;
                cout << 0 << '\n';
                continue;
            }

            // Extract indices and sort
            vector<int> valid_indices;
            valid_indices.reserve(right - left);
            for(int i = left; i < right; i++) {
                valid_indices.push_back(sorted_h[i].second);
            }
            sort(valid_indices.begin(), valid_indices.end());

            // Compute sum with prefix optimization
            ll sum = 0;
            for(size_t i = 1; i < valid_indices.size(); i++) {
                sum += abs(h[valid_indices[i]] - h[valid_indices[i-1]]);
            }

            cache[key] = sum;
            cout << sum << '\n';
        }
    } else {
        // Large n: use standard approach
        while(q--) {
            ll L, R;
            cin >> L >> R;

            // Binary search
            int left = lower_bound(sorted_h.begin(), sorted_h.end(), make_pair(L, -1)) - sorted_h.begin();
            int right = upper_bound(sorted_h.begin(), sorted_h.end(), make_pair(R, n)) - sorted_h.begin();

            if(left >= right) {
                cout << 0 << '\n';
                continue;
            }

            // Extract and sort indices
            static vector<int> valid_indices;
            valid_indices.clear();
            valid_indices.reserve(right - left);
            for(int i = left; i < right; i++) {
                valid_indices.push_back(sorted_h[i].second);
            }
            sort(valid_indices.begin(), valid_indices.end());

            // Compute sum
            ll sum = 0;
            for(size_t i = 1; i < valid_indices.size(); i++) {
                sum += abs(h[valid_indices[i]] - h[valid_indices[i-1]]);
            }

            cout << sum << '\n';
        }
    }

    return 0;
}
