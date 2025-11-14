#pragma GCC optimize("O3,unroll-loops")
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<long long> h(n);
    for (int i = 0; i < n; i++) {
        cin >> h[i];
    }

    // Precompute: sort trees by height with their indices
    vector<pair<long long, int>> sorted_h(n);
    for (int i = 0; i < n; i++) {
        sorted_h[i] = {h[i], i};
    }
    sort(sorted_h.begin(), sorted_h.end());

    while (q--) {
        long long L, R;
        cin >> L >> R;

        // Binary search for range [L, R] in sorted array
        auto it_left = lower_bound(sorted_h.begin(), sorted_h.end(), make_pair(L, 0));
        auto it_right = upper_bound(sorted_h.begin(), sorted_h.end(), make_pair(R, INT_MAX));

        // Extract indices of valid trees
        vector<int> valid_indices;
        valid_indices.reserve(it_right - it_left);
        for (auto it = it_left; it != it_right; ++it) {
            valid_indices.push_back(it->second);
        }

        // Sort indices to maintain original order
        sort(valid_indices.begin(), valid_indices.end());

        // Compute sum of differences
        long long total = 0;
        for (size_t i = 1; i < valid_indices.size(); i++) {
            long long diff = h[valid_indices[i]] - h[valid_indices[i-1]];
            total += (diff >= 0) ? diff : -diff;
        }

        cout << total << '\n';
    }

    return 0;
}
