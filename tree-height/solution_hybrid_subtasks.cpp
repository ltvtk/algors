#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

/*
 * HYBRID SOLUTION - AUTO-DETECT SUBTASK
 *
 * Subtask 1 (20%): n,q ≤ 5000           → O(n×q) brute force
 * Subtask 2 (20%): h_i ≤ 400           → Precompute O(m²) answers
 * Subtask 3 (20%): Sorted queries       → Incremental processing
 * Subtask 4 (20%): n,q ≤ 70000         → Mo's algorithm
 * Subtask 5 (20%): Full constraints     → Mo's algorithm
 *
 * Strategy: Read all queries, detect pattern, choose algorithm!
 */

struct Query {
    ll L, R;
    int idx;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<ll> h(n);
    ll max_h = 0;
    for(int i = 0; i < n; i++) {
        cin >> h[i];
        max_h = max(max_h, h[i]);
    }

    // Read all queries
    vector<Query> queries(q);
    bool queries_sorted = true;
    for(int i = 0; i < q; i++) {
        cin >> queries[i].L >> queries[i].R;
        queries[i].idx = i;

        if(i > 0) {
            if(queries[i].L < queries[i-1].L || queries[i].R < queries[i-1].R) {
                queries_sorted = false;
            }
        }
    }

    vector<ll> answers(q);

    // DETECTION: Choose best algorithm

    // Subtask 1: Small n, q ≤ 5000
    if(n <= 5000 && q <= 5000) {
        // Simple O(n×q) brute force
        for(int qidx = 0; qidx < q; qidx++) {
            ll L = queries[qidx].L;
            ll R = queries[qidx].R;

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
            answers[qidx] = sum;
        }
    }
    // Subtask 2: h_i ≤ 400
    else if(max_h <= 400) {
        // Precompute all answers
        set<ll> unique_set(h.begin(), h.end());
        vector<ll> unique_heights(unique_set.begin(), unique_set.end());
        int m = unique_heights.size();

        vector<vector<ll>> answer(m, vector<ll>(m, 0));

        for(int i = 0; i < m; i++) {
            for(int j = i; j < m; j++) {
                ll L = unique_heights[i];
                ll R = unique_heights[j];

                ll sum = 0;
                ll prev = -1;
                for(int k = 0; k < n; k++) {
                    if(h[k] >= L && h[k] <= R) {
                        if(prev != -1) {
                            sum += abs(h[k] - prev);
                        }
                        prev = h[k];
                    }
                }
                answer[i][j] = sum;
            }
        }

        for(int qidx = 0; qidx < q; qidx++) {
            ll L = queries[qidx].L;
            ll R = queries[qidx].R;

            int L_idx = lower_bound(unique_heights.begin(), unique_heights.end(), L) - unique_heights.begin();
            int R_idx = upper_bound(unique_heights.begin(), unique_heights.end(), R) - unique_heights.begin() - 1;

            if(L_idx > R_idx || L_idx >= m || R_idx < 0) {
                answers[qidx] = 0;
            } else {
                answers[qidx] = answer[L_idx][R_idx];
            }
        }
    }
    // Subtask 3: Sorted queries
    else if(queries_sorted) {
        // Incremental processing
        vector<pair<ll, int>> sorted_h(n);
        for(int i = 0; i < n; i++) {
            sorted_h[i] = {h[i], i};
        }
        sort(sorted_h.begin(), sorted_h.end());

        int curr_L_idx = 0;
        int curr_R_idx = -1;
        set<int> active;
        ll current_sum = 0;

        auto add_position = [&](int pos) {
            auto it = active.insert(pos).first;
            ll contribution = 0;
            if(it != active.begin()) {
                auto prev_it = prev(it);
                contribution += abs(h[pos] - h[*prev_it]);
            }
            auto next_it = next(it);
            if(next_it != active.end()) {
                contribution += abs(h[pos] - h[*next_it]);
                if(it != active.begin()) {
                    auto prev_it = prev(it);
                    current_sum -= abs(h[*prev_it] - h[*next_it]);
                }
            }
            current_sum += contribution;
        };

        auto remove_position = [&](int pos) {
            auto it = active.find(pos);
            ll contribution = 0;
            if(it != active.begin()) {
                auto prev_it = prev(it);
                contribution += abs(h[pos] - h[*prev_it]);
            }
            auto next_it = next(it);
            if(next_it != active.end()) {
                contribution += abs(h[pos] - h[*next_it]);
                if(it != active.begin()) {
                    auto prev_it = prev(it);
                    current_sum += abs(h[*prev_it] - h[*next_it]);
                }
            }
            current_sum -= contribution;
            active.erase(it);
        };

        for(int qidx = 0; qidx < q; qidx++) {
            ll L = queries[qidx].L;
            ll R = queries[qidx].R;

            int target_L_idx = lower_bound(sorted_h.begin(), sorted_h.end(), make_pair(L, -1)) - sorted_h.begin();
            int target_R_idx = upper_bound(sorted_h.begin(), sorted_h.end(), make_pair(R, n)) - sorted_h.begin() - 1;

            if(target_L_idx > target_R_idx) {
                answers[qidx] = 0;
                continue;
            }

            while(curr_R_idx < target_R_idx) {
                curr_R_idx++;
                add_position(sorted_h[curr_R_idx].second);
            }
            while(curr_L_idx < target_L_idx) {
                remove_position(sorted_h[curr_L_idx].second);
                curr_L_idx++;
            }

            answers[qidx] = current_sum;
        }
    }
    // Subtask 4 & 5: Use Mo's algorithm
    else {
        // Mo's algorithm with optimized implementation
        vector<pair<ll, int>> sorted_h(n);
        for(int i = 0; i < n; i++) {
            sorted_h[i] = {h[i], i};
        }
        sort(sorted_h.begin(), sorted_h.end());

        // Convert queries to sorted height indices
        struct MoQuery {
            int L, R, idx, block;
        };

        vector<MoQuery> mo_queries(q);
        int block_size = max(1, (int)sqrt(n));

        for(int i = 0; i < q; i++) {
            mo_queries[i].L = lower_bound(sorted_h.begin(), sorted_h.end(), make_pair(queries[i].L, -1)) - sorted_h.begin();
            mo_queries[i].R = upper_bound(sorted_h.begin(), sorted_h.end(), make_pair(queries[i].R, n)) - sorted_h.begin() - 1;
            mo_queries[i].idx = i;
            mo_queries[i].block = mo_queries[i].L / block_size;
        }

        sort(mo_queries.begin(), mo_queries.end(), [](const MoQuery& a, const MoQuery& b) {
            if(a.block != b.block) return a.block < b.block;
            return (a.block & 1) ? (a.R > b.R) : (a.R < b.R);
        });

        int curr_L = 0, curr_R = -1;
        set<int> active;
        ll current_sum = 0;

        auto add_position = [&](int pos) {
            auto it = active.insert(pos).first;
            ll contribution = 0;
            if(it != active.begin()) {
                auto prev_it = prev(it);
                contribution += abs(h[pos] - h[*prev_it]);
            }
            auto next_it = next(it);
            if(next_it != active.end()) {
                contribution += abs(h[pos] - h[*next_it]);
                if(it != active.begin()) {
                    auto prev_it = prev(it);
                    current_sum -= abs(h[*prev_it] - h[*next_it]);
                }
            }
            current_sum += contribution;
        };

        auto remove_position = [&](int pos) {
            auto it = active.find(pos);
            ll contribution = 0;
            if(it != active.begin()) {
                auto prev_it = prev(it);
                contribution += abs(h[pos] - h[*prev_it]);
            }
            auto next_it = next(it);
            if(next_it != active.end()) {
                contribution += abs(h[pos] - h[*next_it]);
                if(it != active.begin()) {
                    auto prev_it = prev(it);
                    current_sum += abs(h[*prev_it] - h[*next_it]);
                }
            }
            current_sum -= contribution;
            active.erase(it);
        };

        for(const auto& query : mo_queries) {
            int L = query.L;
            int R = query.R;

            if(L > R) {
                answers[query.idx] = 0;
                continue;
            }

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
    }

    // Output answers
    for(int i = 0; i < q; i++) {
        cout << answers[i] << '\n';
    }

    return 0;
}
