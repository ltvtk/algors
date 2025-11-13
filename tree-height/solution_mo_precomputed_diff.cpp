#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

/*
 * MO'S ALGORITHM + PRECOMPUTED DIFFERENCES
 *
 * Key optimization:
 * 1. Precompute ALL pairwise differences for positions that might be neighbors
 * 2. Use static arrays instead of dynamic set
 * 3. Cache frequently accessed values
 *
 * Mảng hiệu insight: Nếu ta maintain active positions sorted,
 * mỗi add/remove chỉ affect at most 2 edges trong chain!
 */

struct Query {
    int L, R, idx, block;
    ll left_val, right_val;
};

const int MAXN = 200005;
int n, q;
ll h[MAXN];
ll answers[MAXN];

// Sorted by height
vector<pair<ll, int>> sorted_h;

// Use array instead of set for better cache performance
int active_count = 0;
int active_list[MAXN]; // Active positions in original order
bool is_active[MAXN];

ll current_sum = 0;

// Fast binary search in active_list
int find_insert_pos(int pos) {
    int left = 0, right = active_count;
    while(left < right) {
        int mid = (left + right) / 2;
        if(active_list[mid] < pos) left = mid + 1;
        else right = mid;
    }
    return left;
}

void add_position(int pos) {
    if(is_active[pos]) return;
    is_active[pos] = true;

    int insert_idx = find_insert_pos(pos);

    // Calculate contribution
    ll contribution = 0;

    // Previous position
    if(insert_idx > 0) {
        int prev_pos = active_list[insert_idx - 1];
        contribution += abs(h[pos] - h[prev_pos]);
    }

    // Next position
    if(insert_idx < active_count) {
        int next_pos = active_list[insert_idx];
        contribution += abs(h[pos] - h[next_pos]);

        // Remove old edge between prev and next
        if(insert_idx > 0) {
            int prev_pos = active_list[insert_idx - 1];
            current_sum -= abs(h[prev_pos] - h[next_pos]);
        }
    }

    current_sum += contribution;

    // Insert into active_list
    for(int i = active_count; i > insert_idx; i--) {
        active_list[i] = active_list[i - 1];
    }
    active_list[insert_idx] = pos;
    active_count++;
}

void remove_position(int pos) {
    if(!is_active[pos]) return;
    is_active[pos] = false;

    // Find position in active_list
    int idx = lower_bound(active_list, active_list + active_count, pos) - active_list;

    // Calculate removed contribution
    ll contribution = 0;

    if(idx > 0) {
        int prev_pos = active_list[idx - 1];
        contribution += abs(h[pos] - h[prev_pos]);
    }

    if(idx < active_count - 1) {
        int next_pos = active_list[idx + 1];
        contribution += abs(h[pos] - h[next_pos]);

        // Add back edge between prev and next
        if(idx > 0) {
            int prev_pos = active_list[idx - 1];
            current_sum += abs(h[prev_pos] - h[next_pos]);
        }
    }

    current_sum -= contribution;

    // Remove from active_list
    for(int i = idx; i < active_count - 1; i++) {
        active_list[i] = active_list[i + 1];
    }
    active_count--;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> q;

    for(int i = 0; i < n; i++) {
        cin >> h[i];
    }

    // Sort by height
    sorted_h.resize(n);
    for(int i = 0; i < n; i++) {
        sorted_h[i] = {h[i], i};
    }
    sort(sorted_h.begin(), sorted_h.end());

    // Read queries
    vector<Query> queries(q);
    int block_size = max(1, (int)(sqrt(n) * 0.7));

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
    sort(queries.begin(), queries.end(), [](const Query& a, const Query& b) {
        if(a.block != b.block) return a.block < b.block;
        return (a.block & 1) ? (a.R > b.R) : (a.R < b.R);
    });

    // Process queries
    int curr_L = 0, curr_R = -1;
    memset(is_active, false, sizeof(is_active));
    active_count = 0;
    current_sum = 0;

    for(const auto& query : queries) {
        int L = query.L;
        int R = query.R;

        if(L > R) {
            answers[query.idx] = 0;
            continue;
        }

        // Mo's algorithm movements
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
