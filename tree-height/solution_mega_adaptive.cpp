#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#pragma GCC optimize("O3,unroll-loops,inline,no-stack-protector,fast-math,tree-vectorize")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt,fma")

/*
 * MEGA ADAPTIVE SOLUTION
 *
 * Combines multiple strategies with intelligent selection:
 * 1. Brute force for small n*q
 * 2. Sorted + binary search for selective queries
 * 3. Inline optimizations everywhere
 * 4. Multiple thresholds and heuristics
 */

const int MAXN = 200005;
int h[MAXN];
pair<int, int> sorted_h[MAXN];
int valid_idx[MAXN];

inline ll fast_abs(ll x) {
    return (x ^ (x >> 63)) - (x >> 63);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int n, q;
    cin >> n >> q;

    for(int i = 0; i < n; i++) {
        cin >> h[i];
        sorted_h[i] = {h[i], i};
    }

    // Strategy 1: Pure brute force for very small inputs
    if(n <= 500 || q <= 500 || (ll)n * q <= 8000000LL) {
        while(q--) {
            int L, R;
            cin >> L >> R;

            ll sum = 0;
            int prev = -1;

            for(int i = 0; i < n; i++) {
                int hi = h[i];
                if(hi >= L && hi <= R) {
                    if(prev != -1) {
                        sum += fast_abs((ll)hi - prev);
                    }
                    prev = hi;
                }
            }

            cout << sum << '\n';
        }
        return 0;
    }

    // Strategy 2: Sorted approach for larger inputs
    sort(sorted_h, sorted_h + n);

    // Read all queries first for potential optimizations
    vector<pair<pair<int,int>, int>> queries;
    queries.reserve(q);

    for(int i = 0; i < q; i++) {
        int L, R;
        cin >> L >> R;
        queries.push_back({{L, R}, i});
    }

    vector<ll> answers(q);

    // Process each query
    for(const auto& query : queries) {
        int L = query.first.first;
        int R = query.first.second;
        int qid = query.second;

        // Binary search for range
        int left = lower_bound(sorted_h, sorted_h + n, make_pair(L, -1)) - sorted_h;
        int right = upper_bound(sorted_h, sorted_h + n, make_pair(R, n)) - sorted_h;

        if(left >= right) {
            answers[qid] = 0;
            continue;
        }

        int count = right - left;

        // Extract and sort indices
        for(int i = 0; i < count; i++) {
            valid_idx[i] = sorted_h[left + i].second;
        }

        sort(valid_idx, valid_idx + count);

        // Calculate sum
        ll sum = 0;
        for(int i = 1; i < count; i++) {
            sum += fast_abs((ll)h[valid_idx[i]] - h[valid_idx[i-1]]);
        }

        answers[qid] = sum;
    }

    // Output answers
    for(ll ans : answers) {
        cout << ans << '\n';
    }

    return 0;
}
