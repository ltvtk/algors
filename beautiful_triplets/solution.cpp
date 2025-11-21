#pragma GCC optimize("O3,unroll-loops,inline")
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n;
    cin >> n;

    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    int mx = *max_element(a.begin(), a.end());

    // Random hashes
    vector<long long> h(mx + 1);
    mt19937_64 rng(42);  // Fixed seed for consistency
    for (int i = 1; i <= mx; i++) {
        h[i] = rng();
    }

    // Pre-compute distinct count
    vector<int> d(n);
    {
        vector<int> seen(mx + 1, 0);
        int cnt = 0;
        for (int i = n - 1; i >= 0; i--) {
            if (!seen[a[i]]) {
                seen[a[i]] = 1;
                cnt++;
            }
            d[i] = cnt;
        }
    }

    vector<int> l(mx + 1, 0), r(mx + 1, 0);
    long long ans = 0;

    for (int j = 0; j < n - 1; j++) {
        int mr = d[j + 1];

        // Use map for stability
        map<pair<int, long long>, int> mp;

        long long ch = 0;
        int cs = 0;

        for (int i = j; i >= 0; i--) {
            if (!l[a[i]]) {
                if (cs + 1 > mr) break;
                l[a[i]] = 1;
                cs++;
                ch ^= h[a[i]];
            }
            mp[{cs, ch}]++;
        }

        // Reset left marks
        ch = 0;
        cs = 0;
        for (int i = j; i >= 0; i--) {
            if (l[a[i]]) {
                l[a[i]] = 0;
                if (cs++ > mr) break;
            }
        }

        int ml = 0;
        for (auto& p : mp) {
            ml = max(ml, p.first.first);
        }

        // Build right
        ch = 0;
        cs = 0;

        for (int k = j + 1; k < n; k++) {
            if (!r[a[k]]) {
                r[a[k]] = 1;
                cs++;
                ch ^= h[a[k]];

                if (cs > ml) break;
            }

            auto it = mp.find({cs, ch});
            if (it != mp.end()) {
                ans += it->second;
            }
        }

        // Reset right marks
        for (int k = j + 1; k < n && cs > 0; k++) {
            if (r[a[k]]) {
                r[a[k]] = 0;
                cs--;
            }
        }
    }

    cout << ans << '\n';

    return 0;
}
