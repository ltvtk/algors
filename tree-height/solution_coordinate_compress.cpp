#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#pragma GCC optimize("O3,unroll-loops,inline,fast-math")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

/*
 * MATHEMATICAL INSIGHT: Coordinate Compression + Precomputation
 *
 * Key observation: Only n distinct height values matter!
 * Compress heights to [0, n-1] and precompute answers for small n.
 *
 * For n ≤ 500: Precompute all O(n²) possible queries
 * Time: O(n³) precompute + O(1) per query
 */

const int MAXN = 200005;
const int PRECOMPUTE_LIMIT = 500;

int h[MAXN];
int compressed[MAXN];
int n, q;

// Coordinate compression
map<int, int> height_to_compressed;
vector<int> compressed_to_height;

// Precomputed answers: answer[L'][R'] for compressed coordinates
map<pair<int,int>, ll> precomputed;

void coordinate_compress() {
    set<int> unique_heights;
    for(int i = 0; i < n; i++) {
        unique_heights.insert(h[i]);
    }

    int idx = 0;
    for(int h_val : unique_heights) {
        height_to_compressed[h_val] = idx;
        compressed_to_height.push_back(h_val);
        idx++;
    }

    for(int i = 0; i < n; i++) {
        compressed[i] = height_to_compressed[h[i]];
    }
}

ll compute_for_range(int L_compressed, int R_compressed) {
    ll sum = 0;
    int prev = -1;

    for(int i = 0; i < n; i++) {
        if(compressed[i] >= L_compressed && compressed[i] <= R_compressed) {
            if(prev != -1) {
                sum += abs((ll)h[i] - prev);
            }
            prev = h[i];
        }
    }

    return sum;
}

void precompute_all_queries() {
    int num_unique = compressed_to_height.size();

    // Precompute all pairs (L', R') in compressed space
    for(int L = 0; L < num_unique; L++) {
        for(int R = L; R < num_unique; R++) {
            ll result = compute_for_range(L, R);
            precomputed[{L, R}] = result;
        }
    }
}

// Fast computation without precomputation
ll compute_fast(int L, int R) {
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

    return sum;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> q;

    for(int i = 0; i < n; i++) {
        cin >> h[i];
    }

    // Decide strategy
    if(n <= PRECOMPUTE_LIMIT) {
        // Use precomputation for small n
        coordinate_compress();
        precompute_all_queries();

        while(q--) {
            int L, R;
            cin >> L >> R;

            // Convert to compressed coordinates
            auto it_L = lower_bound(compressed_to_height.begin(),
                                   compressed_to_height.end(), L);
            auto it_R = upper_bound(compressed_to_height.begin(),
                                    compressed_to_height.end(), R);

            if(it_L == compressed_to_height.end() ||
               it_R == compressed_to_height.begin()) {
                cout << 0 << '\n';
                continue;
            }

            int L_comp = it_L - compressed_to_height.begin();
            int R_comp = (it_R - compressed_to_height.begin()) - 1;

            if(L_comp > R_comp) {
                cout << 0 << '\n';
            } else {
                cout << precomputed[{L_comp, R_comp}] << '\n';
            }
        }
    } else {
        // Use fast brute force for larger n
        while(q--) {
            int L, R;
            cin >> L >> R;
            cout << compute_fast(L, R) << '\n';
        }
    }

    return 0;
}
