#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#pragma GCC optimize("O3,unroll-loops,inline")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

/*
 * Sqrt Decomposition Approach
 *
 * Key idea: Divide array into sqrt(n) blocks
 * For each block, precompute information that helps answer queries fast
 */

const int MAXN = 200005;
int h[MAXN];
int n, q;

// Sqrt decomposition parameters
int block_size;
int num_blocks;

// For each block, store sorted heights and positions
vector<pair<int, int>> blocks[450]; // blocks[b] = {height, position}

void build_blocks() {
    block_size = max(1, (int)sqrt(n * 1.5));
    num_blocks = (n + block_size - 1) / block_size;

    for(int b = 0; b < num_blocks; b++) {
        int start = b * block_size;
        int end = min(n, start + block_size);

        for(int i = start; i < end; i++) {
            blocks[b].push_back({h[i], i});
        }
        sort(blocks[b].begin(), blocks[b].end());
    }
}

// Get all valid positions in range [L, R] for a single block
void get_valid_in_block(int block_id, int L, int R, vector<int>& positions) {
    auto& block = blocks[block_id];

    // Binary search for range [L, R]
    auto it_start = lower_bound(block.begin(), block.end(), make_pair(L, -1));
    auto it_end = upper_bound(block.begin(), block.end(), make_pair(R, n));

    for(auto it = it_start; it != it_end; ++it) {
        positions.push_back(it->second);
    }
}

ll solve_query(int L, int R) {
    vector<int> valid_positions;
    valid_positions.reserve(n);

    // Collect all valid positions from all blocks
    for(int b = 0; b < num_blocks; b++) {
        get_valid_in_block(b, L, R, valid_positions);
    }

    // Sort by original position
    sort(valid_positions.begin(), valid_positions.end());

    // Calculate sum
    ll sum = 0;
    for(size_t i = 1; i < valid_positions.size(); i++) {
        ll diff = (ll)h[valid_positions[i]] - h[valid_positions[i-1]];
        sum += (diff ^ (diff >> 63)) - (diff >> 63);
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

    // Adaptive: use brute force for small inputs
    if((ll)n * q <= 5000000LL) {
        while(q--) {
            int L, R;
            cin >> L >> R;
            ll sum = 0;
            int prev = -1;
            for(int i = 0; i < n; i++) {
                if(h[i] >= L && h[i] <= R) {
                    if(prev != -1) {
                        ll diff = (ll)h[i] - prev;
                        sum += (diff ^ (diff >> 63)) - (diff >> 63);
                    }
                    prev = h[i];
                }
            }
            cout << sum << '\n';
        }
        return 0;
    }

    build_blocks();

    while(q--) {
        int L, R;
        cin >> L >> R;
        cout << solve_query(L, R) << '\n';
    }

    return 0;
}
