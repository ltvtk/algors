#pragma GCC optimize("O3,unroll-loops")
#include <bits/stdc++.h>
using namespace std;

// Custom hash for pair that's much faster
struct PairHash {
    size_t operator()(const pair<int, long long>& p) const {
        return (size_t)p.first * 1000000007LL + (size_t)(p.second >> 32) * 1000000009LL + (size_t)p.second;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    int mx = *max_element(a.begin(), a.end());

    // Random hashes
    vector<long long> h(mx + 1);
    mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
    for (int i = 1; i <= mx; i++) {
        h[i] = rng();
    }

    // Pre-compute distinct from each position
    vector<int> d(n + 1);
    {
        unordered_set<int> s;
        for (int i = n - 1; i >= 0; i--) {
            s.insert(a[i]);
            d[i] = s.size();
        }
    }

    vector<bool> l(mx + 1), r(mx + 1);
    long long ans = 0;

    for (int j = 0; j < n - 1; j++) {
        int mr = d[j + 1];

        // Build left with unordered_map for O(1) lookup
        unordered_map<pair<int, long long>, int, PairHash> m;
        m.reserve(mr + 1);

        long long ch = 0;
        int cs = 0;
        vector<int> cl;
        cl.reserve(mr);

        for (int i = j; i >= 0; i--) {
            if (!l[a[i]]) {
                if (cs + 1 > mr) break;
                l[a[i]] = 1;
                cl.push_back(a[i]);
                cs++;
                ch ^= h[a[i]];
            }
            m[{cs, ch}]++;
        }

        for (int x : cl) l[x] = 0;

        int ml = cs;

        // Build right
        ch = 0;
        cs = 0;
        vector<int> cr;
        cr.reserve(ml);

        for (int k = j + 1; k < n; k++) {
            if (!r[a[k]]) {
                r[a[k]] = 1;
                cr.push_back(a[k]);
                cs++;
                ch ^= h[a[k]];

                if (cs > ml) {
                    for (int x : cr) r[x] = 0;
                    break;
                }
            }

            auto it = m.find({cs, ch});
            if (it != m.end()) {
                ans += it->second;
            }
        }

        if (cs <= ml) {
            for (int x : cr) r[x] = 0;
        }
    }

    cout << ans << '\n';

    return 0;
}
