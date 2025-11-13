#include <bits/stdc++.h>
using namespace std;
using ll = long long;

/*
 * THCS LEVEL - Simple Observations
 *
 * For young students, check simple patterns:
 * 1. Are all queries the same? → Answer once
 * 2. Are heights all same? → Answer always 0
 * 3. Are heights sorted? → Maybe formula
 * 4. Can we precompute something simple?
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

    // OBSERVATION 1: All heights the same?
    bool all_same = true;
    for(int i = 1; i < n; i++) {
        if(h[i] != h[0]) {
            all_same = false;
            break;
        }
    }

    if(all_same) {
        // If all heights same, answer is always 0
        for(int i = 0; i < q; i++) {
            ll L, R;
            cin >> L >> R;
            cout << 0 << '\n';
        }
        return 0;
    }

    // OBSERVATION 2: Check if heights are sorted
    bool sorted_asc = true, sorted_desc = true;
    for(int i = 1; i < n; i++) {
        if(h[i] < h[i-1]) sorted_asc = false;
        if(h[i] > h[i-1]) sorted_desc = false;
    }

    // Read all queries first
    vector<pair<ll,ll>> queries;
    for(int i = 0; i < q; i++) {
        ll L, R;
        cin >> L >> R;
        queries.push_back({L, R});
    }

    // OBSERVATION 3: All queries identical?
    bool all_queries_same = true;
    for(int i = 1; i < q; i++) {
        if(queries[i] != queries[0]) {
            all_queries_same = false;
            break;
        }
    }

    if(all_queries_same) {
        // Compute once, output q times
        ll L = queries[0].first;
        ll R = queries[0].second;

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

        for(int i = 0; i < q; i++) {
            cout << sum << '\n';
        }
        return 0;
    }

    // GENERAL CASE with simple optimization
    // For THCS level, maybe just well-written brute force is enough
    for(auto [L, R] : queries) {
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

    return 0;
}
