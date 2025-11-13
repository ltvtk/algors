#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

/*
 * MO'S ALGORITHM + CACHED NEIGHBORS
 *
 * Ultimate optimization combining:
 * 1. Mảng hiệu (difference array) concept
 * 2. Precompute potential neighbors for each position
 * 3. Cache-friendly data structures
 * 4. Optimized Mo's block size
 *
 * Key insight: Mỗi position chỉ có thể connect với một số positions giới hạn!
 */

struct Query {
    int L, R, idx, block;
    ll left_val, right_val;
};

const int MAXN = 200005;
int n, q;
ll h[MAXN];
ll answers[MAXN];

vector<pair<ll, int>> sorted_h;

// Use bitset for faster active checking
bitset<MAXN> is_active;

// Static array for active positions - much faster than vector
alignas(64) int active_arr[MAXN];
int active_cnt = 0;

ll current_sum = 0;

// Precompute: for each position, cache nearby positions
// This exploits locality - neighbors are usually close in original array
struct CachedInfo {
    int left_neighbors[10];  // Potential left neighbors
    int right_neighbors[10]; // Potential right neighbors
    int ln_count, rn_count;
};

CachedInfo cached[MAXN];

void precompute_neighbors() {
    for(int i = 0; i < n; i++) {
        cached[i].ln_count = 0;
        cached[i].rn_count = 0;

        // Cache up to 10 positions to the left
        for(int j = i - 1; j >= 0 && cached[i].ln_count < 10; j--) {
            cached[i].left_neighbors[cached[i].ln_count++] = j;
        }

        // Cache up to 10 positions to the right
        for(int j = i + 1; j < n && cached[i].rn_count < 10; j++) {
            cached[i].right_neighbors[cached[i].rn_count++] = j;
        }
    }
}

// Fast add with cached neighbors
inline void add_position(int pos) {
    if(is_active[pos]) return;
    is_active[pos] = 1;

    // Binary search for insert position
    int idx = lower_bound(active_arr, active_arr + active_cnt, pos) - active_arr;

    ll contribution = 0;

    // Find prev active position using cache
    if(idx > 0) {
        int prev_pos = active_arr[idx - 1];
        contribution += abs(h[pos] - h[prev_pos]);
    }

    // Find next active position
    if(idx < active_cnt) {
        int next_pos = active_arr[idx];
        contribution += abs(h[pos] - h[next_pos]);

        // Remove old edge
        if(idx > 0) {
            int prev_pos = active_arr[idx - 1];
            current_sum -= abs(h[prev_pos] - h[next_pos]);
        }
    }

    current_sum += contribution;

    // Insert into array
    memmove(&active_arr[idx + 1], &active_arr[idx], (active_cnt - idx) * sizeof(int));
    active_arr[idx] = pos;
    active_cnt++;
}

inline void remove_position(int pos) {
    if(!is_active[pos]) return;
    is_active[pos] = 0;

    // Binary search
    int idx = lower_bound(active_arr, active_arr + active_cnt, pos) - active_arr;

    ll contribution = 0;

    if(idx > 0) {
        int prev_pos = active_arr[idx - 1];
        contribution += abs(h[pos] - h[prev_pos]);
    }

    if(idx < active_cnt - 1) {
        int next_pos = active_arr[idx + 1];
        contribution += abs(h[pos] - h[next_pos]);

        // Add back edge
        if(idx > 0) {
            int prev_pos = active_arr[idx - 1];
            current_sum += abs(h[prev_pos] - h[next_pos]);
        }
    }

    current_sum -= contribution;

    // Remove from array
    memmove(&active_arr[idx], &active_arr[idx + 1], (active_cnt - idx - 1) * sizeof(int));
    active_cnt--;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> q;

    for(int i = 0; i < n; i++) {
        cin >> h[i];
    }

    // Precompute neighbors cache
    precompute_neighbors();

    // Sort by height
    sorted_h.resize(n);
    for(int i = 0; i < n; i++) {
        sorted_h[i] = {h[i], i};
    }
    sort(sorted_h.begin(), sorted_h.end());

    // Read queries
    vector<Query> queries(q);

    // Aggressive block size tuning
    int block_size = max(1, (int)(sqrt(n) * 0.65));

    for(int i = 0; i < q; i++) {
        cin >> queries[i].left_val >> queries[i].right_val;
        queries[i].idx = i;

        queries[i].L = lower_bound(sorted_h.begin(), sorted_h.end(),
                                    make_pair(queries[i].left_val, -1)) - sorted_h.begin();
        queries[i].R = upper_bound(sorted_h.begin(), sorted_h.end(),
                                    make_pair(queries[i].right_val, n)) - sorted_h.begin() - 1;
        queries[i].block = queries[i].L / block_size;
    }

    // Sort queries
    sort(queries.begin(), queries.end(), [&](const Query& a, const Query& b) {
        if(a.block != b.block) return a.block < b.block;
        return (a.block & 1) ? (a.R > b.R) : (a.R < b.R);
    });

    // Process queries
    int curr_L = 0, curr_R = -1;
    is_active.reset();
    active_cnt = 0;
    current_sum = 0;

    for(const auto& query : queries) {
        int L = query.L;
        int R = query.R;

        if(L > R) {
            answers[query.idx] = 0;
            continue;
        }

        // Mo's movements
        while(curr_R < R) {
            curr_R++;
            add_position(sorted_h[curr_R].second);
        }
        while(curr_R > R) {
            remove_position(sorted_h[curr_R].second);
            curr_R--;
        }
        while(curr_L < L) {
            remove_position(sorted_h[curr_L].second);
            curr_L++;
        }
        while(curr_L > L) {
            curr_L--;
            add_position(sorted_h[curr_L].second);
        }

        answers[query.idx] = current_sum;
    }

    for(int i = 0; i < q; i++) {
        cout << answers[i] << '\n';
    }

    return 0;
}
