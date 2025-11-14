#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#pragma GCC optimize("O3,unroll-loops,inline,no-stack-protector,fast-math")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt,fma")

/*
 * Hyper-optimized solution with aggressive compiler optimizations
 * and cache-friendly memory access patterns
 */

const int MAXN = 200005;
int h[MAXN];
int n, q;

// Precomputed: for each height value, store all positions
unordered_map<int, vector<int>> height_to_positions;

// Sorted version for binary search
pair<int, int> sorted_h[MAXN];

inline ll fast_abs(ll x) {
    return (x ^ (x >> 63)) - (x >> 63);
}

// Optimized query processing
ll process_query(int L, int R) {
    // Binary search to find valid range
    int left = lower_bound(sorted_h, sorted_h + n, make_pair(L, -1)) - sorted_h;
    int right = upper_bound(sorted_h, sorted_h + n, make_pair(R, n)) - sorted_h - 1;

    if(left > right) return 0;

    int count = right - left + 1;

    // For very small count, use simple approach
    if(count <= 20) {
        vector<int> positions;
        positions.reserve(count);
        for(int i = left; i <= right; i++) {
            positions.push_back(sorted_h[i].second);
        }
        sort(positions.begin(), positions.end());

        ll sum = 0;
        for(int i = 1; i < count; i++) {
            sum += fast_abs((ll)h[positions[i]] - h[positions[i-1]]);
        }
        return sum;
    }

    // For larger count, use array-based approach
    static int valid_indices[MAXN];
    int valid_count = 0;

    for(int i = left; i <= right; i++) {
        valid_indices[valid_count++] = sorted_h[i].second;
    }

    // Sort indices
    sort(valid_indices, valid_indices + valid_count);

    ll sum = 0;
    for(int i = 1; i < valid_count; i++) {
        sum += fast_abs((ll)h[valid_indices[i]] - h[valid_indices[i-1]]);
    }

    return sum;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> q;

    for(int i = 0; i < n; i++) {
        cin >> h[i];
        sorted_h[i] = {h[i], i};
    }

    // Preprocessing
    sort(sorted_h, sorted_h + n);

    // Adaptive brute force threshold
    ll threshold = 6000000LL;
    if((ll)n * q <= threshold) {
        // Brute force for small inputs
        while(q--) {
            int L, R;
            cin >> L >> R;

            ll sum = 0;
            int prev = -1;

            // Unrolled loop for better performance
            int i = 0;
            for(; i + 3 < n; i += 4) {
                // Process 4 elements at once
                if(h[i] >= L && h[i] <= R) {
                    if(prev != -1) sum += fast_abs((ll)h[i] - prev);
                    prev = h[i];
                }
                if(h[i+1] >= L && h[i+1] <= R) {
                    if(prev != -1) sum += fast_abs((ll)h[i+1] - prev);
                    prev = h[i+1];
                }
                if(h[i+2] >= L && h[i+2] <= R) {
                    if(prev != -1) sum += fast_abs((ll)h[i+2] - prev);
                    prev = h[i+2];
                }
                if(h[i+3] >= L && h[i+3] <= R) {
                    if(prev != -1) sum += fast_abs((ll)h[i+3] - prev);
                    prev = h[i+3];
                }
            }

            // Handle remaining elements
            for(; i < n; i++) {
                if(h[i] >= L && h[i] <= R) {
                    if(prev != -1) sum += fast_abs((ll)h[i] - prev);
                    prev = h[i];
                }
            }

            cout << sum << '\n';
        }
        return 0;
    }

    // For larger inputs, use binary search approach
    while(q--) {
        int L, R;
        cin >> L >> R;
        cout << process_query(L, R) << '\n';
    }

    return 0;
}
