#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

/*
 * MO'S ALGORITHM + DIFFERENCE ARRAY + PRECOMPUTE
 *
 * Key insight combining THCS techniques with Mo's:
 * 1. Precompute sorted order by HEIGHT
 * 2. For each position, precompute its neighbors in sorted order
 * 3. Use difference array to optimize add/remove operations
 *
 * Instead of dynamic set operations, use precomputed structure!
 */

struct Query {
    int L, R, idx, block;
    ll left_val, right_val;
};

const int MAXN = 200005;
int n, q;
ll h[MAXN];
ll answers[MAXN];

// Precomputed sorted structure
vector<pair<ll, int>> sorted_by_height; // (height, position)
int pos_in_sorted[MAXN]; // position i is at index pos_in_sorted[i] in sorted array

// Active positions tracking
bool active[MAXN];
vector<int> active_positions; // Maintain sorted by original position

ll current_sum = 0;

// Precompute: for each position, find its prev/next in sorted order
int prev_in_sorted[MAXN]; // prev active position in sorted order
int next_in_sorted[MAXN]; // next active position in sorted order

void precompute_sorted() {
    sorted_by_height.resize(n);
    for(int i = 0; i < n; i++) {
        sorted_by_height[i] = {h[i], i};
    }
    sort(sorted_by_height.begin(), sorted_by_height.end());

    for(int i = 0; i < n; i++) {
        int pos = sorted_by_height[i].second;
        pos_in_sorted[pos] = i;
    }
}

void add_position(int pos) {
    if(active[pos]) return;
    active[pos] = true;

    // Find prev and next active positions in ORIGINAL array order
    auto it = lower_bound(active_positions.begin(), active_positions.end(), pos);

    ll contribution = 0;

    // Check previous position
    if(it != active_positions.begin()) {
        auto prev_it = prev(it);
        int prev_pos = *prev_it;
        ll diff = abs(h[pos] - h[prev_pos]);
        contribution += diff;
    }

    // Check next position
    if(it != active_positions.end()) {
        int next_pos = *it;
        ll diff = abs(h[pos] - h[next_pos]);
        contribution += diff;

        // Remove old connection between prev and next
        if(it != active_positions.begin()) {
            auto prev_it = prev(it);
            int prev_pos = *prev_it;
            current_sum -= abs(h[prev_pos] - h[next_pos]);
        }
    }

    current_sum += contribution;
    active_positions.insert(it, pos);
}

void remove_position(int pos) {
    if(!active[pos]) return;
    active[pos] = false;

    auto it = lower_bound(active_positions.begin(), active_positions.end(), pos);

    // Remove contributions from this position
    ll contribution = 0;

    if(it != active_positions.begin()) {
        auto prev_it = prev(it);
        int prev_pos = *prev_it;
        contribution += abs(h[pos] - h[prev_pos]);
    }

    auto next_it = next(it);
    if(next_it != active_positions.end()) {
        int next_pos = *next_it;
        contribution += abs(h[pos] - h[next_pos]);

        // Add back connection between prev and next
        if(it != active_positions.begin()) {
            auto prev_it = prev(it);
            int prev_pos = *prev_it;
            current_sum += abs(h[prev_pos] - h[next_pos]);
        }
    }

    current_sum -= contribution;
    active_positions.erase(it);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> q;

    for(int i = 0; i < n; i++) {
        cin >> h[i];
    }

    // Precompute sorted structure
    precompute_sorted();

    // Read queries
    vector<Query> queries(q);
    int block_size = max(1, (int)sqrt(n * 0.7)); // Tuned block size

    for(int i = 0; i < q; i++) {
        cin >> queries[i].left_val >> queries[i].right_val;
        queries[i].idx = i;

        // Find L, R as positions in sorted array
        queries[i].L = lower_bound(sorted_by_height.begin(), sorted_by_height.end(),
                                    make_pair(queries[i].left_val, -1)) - sorted_by_height.begin();
        queries[i].R = upper_bound(sorted_by_height.begin(), sorted_by_height.end(),
                                    make_pair(queries[i].right_val, n)) - sorted_by_height.begin() - 1;
        queries[i].block = queries[i].L / block_size;
    }

    // Sort queries using Mo's order
    sort(queries.begin(), queries.end(), [](const Query& a, const Query& b) {
        if(a.block != b.block) return a.block < b.block;
        return (a.block & 1) ? (a.R > b.R) : (a.R < b.R);
    });

    // Process queries
    int curr_L = 0, curr_R = -1;
    memset(active, false, sizeof(active));
    active_positions.clear();
    current_sum = 0;

    for(const auto& query : queries) {
        int L = query.L;
        int R = query.R;

        if(L > R) {
            answers[query.idx] = 0;
            continue;
        }

        // Expand/contract range
        while(curr_R < R) {
            curr_R++;
            add_position(sorted_by_height[curr_R].second);
        }
        while(curr_R > R) {
            remove_position(sorted_by_height[curr_R].second);
            curr_R--;
        }
        while(curr_L < L) {
            remove_position(sorted_by_height[curr_L].second);
            curr_L++;
        }
        while(curr_L > L) {
            curr_L--;
            add_position(sorted_by_height[curr_L].second);
        }

        answers[query.idx] = current_sum;
    }

    // Output answers
    for(int i = 0; i < q; i++) {
        cout << answers[i] << '\n';
    }

    return 0;
}
