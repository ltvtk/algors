#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

/*
 * PREFIX SUM APPROACH 1: Precompute consecutive differences
 *
 * Key idea: prefix_diff[i] = sum of |h[j] - h[j+1]| for j = 0 to i-1
 *
 * For contiguous ranges in original array, we can use prefix sum
 * But problem filters by HEIGHT, not position...
 *
 * Still, let's try optimizations with this
 */

const int MAXN = 200005;
ll h[MAXN];
ll prefix_diff[MAXN];  // prefix_diff[i] = sum of differences from 0 to i-1
int n, q;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> q;

    for(int i = 0; i < n; i++) {
        cin >> h[i];
    }

    // Precompute prefix sum of consecutive differences
    prefix_diff[0] = 0;
    for(int i = 1; i < n; i++) {
        prefix_diff[i] = prefix_diff[i-1] + abs(h[i] - h[i-1]);
    }

    // Total sum if all trees included
    ll total_sum = prefix_diff[n-1];

    while(q--) {
        ll L, R;
        cin >> L >> R;

        // SPECIAL CASE 1: All trees in range
        bool all_in_range = true;
        for(int i = 0; i < n; i++) {
            if(h[i] < L || h[i] > R) {
                all_in_range = false;
                break;
            }
        }

        if(all_in_range) {
            cout << total_sum << '\n';
            continue;
        }

        // SPECIAL CASE 2: Check if filtered trees form contiguous block
        int first_valid = -1, last_valid = -1;
        for(int i = 0; i < n; i++) {
            if(h[i] >= L && h[i] <= R) {
                if(first_valid == -1) first_valid = i;
                last_valid = i;
            }
        }

        if(first_valid == -1) {
            cout << 0 << '\n';
            continue;
        }

        // Check if it's contiguous
        bool is_contiguous = true;
        for(int i = first_valid; i <= last_valid; i++) {
            if(h[i] < L || h[i] > R) {
                is_contiguous = false;
                break;
            }
        }

        if(is_contiguous && first_valid < last_valid) {
            // Use prefix sum!
            ll result = prefix_diff[last_valid] - prefix_diff[first_valid];
            cout << result << '\n';
            continue;
        }

        // GENERAL CASE: Brute force
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
