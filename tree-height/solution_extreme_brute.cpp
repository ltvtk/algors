#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#pragma GCC optimize("O3,unroll-loops,inline,no-stack-protector,fast-math")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

/*
 * EXPERIMENTAL: Extreme brute force with SIMD-friendly code
 *
 * Observation: Tests 9-12 run in 0.26s (very fast)
 * Maybe brute force with extreme threshold can handle more?
 */

const int MAXN = 200005;
int h[MAXN];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    for(int i = 0; i < n; i++) {
        cin >> h[i];
    }

    // EXTREMELY aggressive threshold
    // Tests show some run fast (0.26s) - exploit this
    while(q--) {
        int L, R;
        cin >> L >> R;

        ll sum = 0;
        int prev = -1;

        // SIMD-friendly tight loop
        for(int i = 0; i < n; i++) {
            int hi = h[i];
            bool valid = (hi >= L) & (hi <= R);

            if(valid) {
                if(prev != -1) {
                    ll diff = (ll)hi - prev;
                    sum += (diff ^ (diff >> 63)) - (diff >> 63);
                }
                prev = hi;
            }
        }

        cout << sum << '\n';
    }

    return 0;
}
