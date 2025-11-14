#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

/*
 * EXPERIMENTAL: Different data structure approach
 *
 * Observation: Maybe we can precompute something smart?
 * Try: For each position pair (i, j), determine when they become adjacent
 */

const int MAXN = 200005;
int h[MAXN];
int n, q;

// Precompute: for consecutive positions, when are they both valid?
struct Edge {
    int pos1, pos2;
    int min_h, max_h;
    ll diff;
};

vector<Edge> edges;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> q;

    for(int i = 0; i < n; i++) {
        cin >> h[i];
    }

    // Try brute for smaller inputs
    if((ll)n * q <= 20000000LL) {
        while(q--) {
            int L, R;
            cin >> L >> R;

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

            cout << sum << '\n';
        }
        return 0;
    }

    // For larger: try different approach
    // Precompute information about consecutive pairs
    edges.reserve(n - 1);

    for(int i = 0; i < n - 1; i++) {
        int min_h = min(h[i], h[i+1]);
        int max_h = max(h[i], h[i+1]);
        ll diff = abs((ll)h[i] - h[i+1]);
        edges.push_back({i, i+1, min_h, max_h, diff});
    }

    // Process queries with sorted approach
    vector<pair<int,int>> sorted_h;
    sorted_h.reserve(n);
    for(int i = 0; i < n; i++) {
        sorted_h.emplace_back(h[i], i);
    }
    sort(sorted_h.begin(), sorted_h.end());

    while(q--) {
        int L, R;
        cin >> L >> R;

        // Binary search for valid range
        int left = lower_bound(sorted_h.begin(), sorted_h.end(), make_pair(L, -1)) - sorted_h.begin();
        int right = upper_bound(sorted_h.begin(), sorted_h.end(), make_pair(R, n)) - sorted_h.begin();

        if(left >= right) {
            cout << 0 << '\n';
            continue;
        }

        // Extract positions and sort
        static int valid[MAXN];
        int cnt = 0;

        for(int i = left; i < right; i++) {
            valid[cnt++] = sorted_h[i].second;
        }

        sort(valid, valid + cnt);

        ll sum = 0;
        for(int i = 1; i < cnt; i++) {
            sum += abs((ll)h[valid[i]] - h[valid[i-1]]);
        }

        cout << sum << '\n';
    }

    return 0;
}
