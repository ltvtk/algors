#pragma GCC optimize("O3,unroll-loops,inline,prefetch-loop-arrays")
#pragma GCC target("avx2,popcnt,lzcnt,bmi,bmi2")
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

    while (q--) {
        long long L, R;
        cin >> L >> R;

        long long total = 0;
        long long prev = -1;

        // Linear scan - optimized with branch prediction hints
        for (int i = 0; i < n; i++) {
            long long hi = h[i];
            if (__builtin_expect(hi >= L && hi <= R, 1)) {
                if (prev != -1) {
                    long long diff = hi - prev;
                    total += (diff ^ (diff >> 63)) - (diff >> 63);
                }
                prev = hi;
            }
        }

        cout << total << '\n';
    }

    return 0;
}
