#pragma GCC optimize("O3,unroll-loops")
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    int max_val = *max_element(a.begin(), a.end());

    // Random hashes
    vector<long long> h(max_val + 1);
    mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
    for (int i = 1; i <= max_val; i++) {
        h[i] = rng();
    }

    // Pre-compute distinct values from each position
    vector<int> dist(n + 1, 0);
    {
        unordered_set<int> s;
        for (int i = n - 1; i >= 0; i--) {
            s.insert(a[i]);
            dist[i] = s.size();
        }
    }

    vector<bool> sl(max_val + 1), sr(max_val + 1);
    long long ans = 0;

    for (int j = 0; j < n - 1; j++) {
        int mr = dist[j + 1];

        // Build left segments
        map<pair<int, long long>, int> lm;
        long long ch = 0;
        int cs = 0;
        vector<int> cl;
        cl.reserve(mr);

        for (int i = j; i >= 0; i--) {
            if (!sl[a[i]]) {
                if (cs + 1 > mr) break;
                sl[a[i]] = 1;
                cl.push_back(a[i]);
                cs++;
                ch ^= h[a[i]];
            }
            lm[{cs, ch}]++;
        }

        for (int x : cl) sl[x] = 0;

        int ml = cs;

        // Build right segments
        ch = 0;
        cs = 0;
        vector<int> cr;
        cr.reserve(ml);

        for (int k = j + 1; k < n; k++) {
            if (!sr[a[k]]) {
                sr[a[k]] = 1;
                cr.push_back(a[k]);
                cs++;
                ch ^= h[a[k]];

                if (cs > ml) {
                    for (int x : cr) sr[x] = 0;
                    break;
                }
            }

            auto it = lm.find({cs, ch});
            if (it != lm.end()) {
                ans += it->second;
            }
        }

        if (cs <= ml) {
            for (int x : cr) sr[x] = 0;
        }
    }

    cout << ans << '\n';

    return 0;
}
