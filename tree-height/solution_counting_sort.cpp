#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#pragma GCC optimize("O3,unroll-loops,inline,fast-math")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

/*
 * EXPERIMENTAL: Exploit if heights have small range
 *
 * If heights ≤ some reasonable value (e.g., 10^6),
 * we can use counting/bucket sort instead of comparison sort
 */

const int MAXN = 200005;
const int MAXH = 1000005; // Adjust based on actual constraint

int h[MAXN];
int n, q;

// Bucket: for each height value, store positions
vector<int> buckets[MAXH];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> q;

    int max_height = 0;
    int min_height = INT_MAX;

    for(int i = 0; i < n; i++) {
        cin >> h[i];
        max_height = max(max_height, h[i]);
        min_height = min(min_height, h[i]);
    }

    // Check if range is reasonable for bucketing
    ll range = (ll)max_height - min_height + 1;

    if(range > 2000000LL || (ll)n * q <= 25000000LL) {
        // Range too large or small enough for brute force
        while(q--) {
            int L, R;
            cin >> L >> R;

            ll sum = 0;
            int prev = -1;

            for(int i = 0; i < n; i++) {
                if(h[i] >= L && h[i] <= R) {
                    if(prev != -1) {
                        ll d = (ll)h[i] - prev;
                        sum += (d ^ (d >> 63)) - (d >> 63);
                    }
                    prev = h[i];
                }
            }

            cout << sum << '\n';
        }
        return 0;
    }

    // Use bucketing - shift to 0-indexed
    int shift = min_height;

    for(int i = 0; i < n; i++) {
        int bucket_idx = h[i] - shift;
        buckets[bucket_idx].push_back(i);
    }

    // Process queries with bucket approach
    while(q--) {
        int L, R;
        cin >> L >> R;

        if(L < min_height || R < min_height || L > max_height) {
            cout << 0 << '\n';
            continue;
        }

        int start_bucket = max(0, L - shift);
        int end_bucket = min((int)range - 1, R - shift);

        // Collect all valid positions
        static vector<int> valid_positions;
        valid_positions.clear();

        for(int b = start_bucket; b <= end_bucket; b++) {
            for(int pos : buckets[b]) {
                valid_positions.push_back(pos);
            }
        }

        // Sort by position
        sort(valid_positions.begin(), valid_positions.end());

        // Compute sum
        ll sum = 0;
        for(size_t i = 1; i < valid_positions.size(); i++) {
            ll diff = (ll)h[valid_positions[i]] - h[valid_positions[i-1]];
            sum += (diff ^ (diff >> 63)) - (diff >> 63);
        }

        cout << sum << '\n';
    }

    return 0;
}
