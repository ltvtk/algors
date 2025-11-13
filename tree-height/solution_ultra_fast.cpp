#pragma GCC optimize("O3,unroll-loops,inline")
#pragma GCC target("avx2,popcnt,lzcnt,bmi,bmi2")
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 200005;
long long h[MAXN];
pair<long long, int> sorted_h[MAXN];
int valid[MAXN];
int n, q;

inline long long abs_diff(long long a, long long b) {
    long long d = a - b;
    return (d ^ (d >> 63)) - (d >> 63);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> q;

    for (int i = 0; i < n; i++) {
        cin >> h[i];
        sorted_h[i] = {h[i], i};
    }

    sort(sorted_h, sorted_h + n);

    while (q--) {
        long long L, R;
        cin >> L >> R;

        // Binary search for range [L, R]
        int left = lower_bound(sorted_h, sorted_h + n, make_pair(L, 0)) - sorted_h;
        int right = upper_bound(sorted_h, sorted_h + n, make_pair(R, INT_MAX)) - sorted_h;

        int cnt = 0;
        for (int i = left; i < right; i++) {
            valid[cnt++] = sorted_h[i].second;
        }

        // Sort indices in-place
        sort(valid, valid + cnt);

        // Compute sum
        long long total = 0;
        for (int i = 1; i < cnt; i++) {
            total += abs_diff(h[valid[i]], h[valid[i-1]]);
        }

        cout << total << '\n';
    }

    return 0;
}
