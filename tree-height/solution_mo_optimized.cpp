#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#pragma GCC optimize("O3,unroll-loops,inline")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<int> h(n);
    for(int i = 0; i < n; i++) cin >> h[i];

    // Brute force for small cases
    if((ll)n * q <= 3000000LL) {
        while(q--) {
            int L, R; cin >> L >> R;
            ll sum = 0;
            int prev = -1;
            for(int i = 0; i < n; i++) {
                if(h[i] >= L && h[i] <= R) {
                    if(prev != -1) sum += abs((ll)h[i] - (ll)prev);
                    prev = h[i];
                }
            }
            cout << sum << '\n';
        }
        return 0;
    }

    vector<pair<int,int>> sorted_h;
    sorted_h.reserve(n);
    for(int i = 0; i < n; i++)
        sorted_h.emplace_back(h[i], i);
    sort(sorted_h.begin(), sorted_h.end());

    struct Query {
        int l_idx, r_idx, id;
    };
    vector<Query> queries;
    queries.reserve(q);

    for(int i = 0; i < q; i++) {
        int L, R; cin >> L >> R;
        int l_idx = lower_bound(sorted_h.begin(), sorted_h.end(), make_pair(L, -1)) - sorted_h.begin();
        int r_idx = upper_bound(sorted_h.begin(), sorted_h.end(), make_pair(R, n)) - sorted_h.begin() - 1;
        queries.push_back({l_idx, r_idx, i});
    }

    // Tuned block size
    int block = max(1, min(n, (int)(n / sqrt(max(1, q) * 0.7))));

    sort(queries.begin(), queries.end(), [&](const Query &a, const Query &b) {
        int ba = a.l_idx / block;
        int bb = b.l_idx / block;
        if(ba != bb) return ba < bb;
        return (ba & 1) ? (a.r_idx < b.r_idx) : (a.r_idx > b.r_idx);
    });

    set<int> active_positions;
    ll current_sum = 0;
    vector<ll> answers(q);

    // Optimized add with reduced set operations
    auto add_tree = [&](int sorted_idx) __attribute__((always_inline)) {
        int pos = sorted_h[sorted_idx].second;

        auto [it, inserted] = active_positions.insert(pos);
        if(!inserted) return;

        auto next_it = it;
        ++next_it;

        if(it != active_positions.begin()) {
            auto prev_it = prev(it);
            int prev_pos = *prev_it;
            current_sum += abs((ll)h[pos] - (ll)h[prev_pos]);

            if(next_it != active_positions.end()) {
                int next_pos = *next_it;
                current_sum -= abs((ll)h[prev_pos] - (ll)h[next_pos]);
            }
        }

        if(next_it != active_positions.end()) {
            int next_pos = *next_it;
            current_sum += abs((ll)h[pos] - (ll)h[next_pos]);
        }
    };

    auto remove_tree = [&](int sorted_idx) __attribute__((always_inline)) {
        int pos = sorted_h[sorted_idx].second;

        auto it = active_positions.find(pos);
        if(it == active_positions.end()) return;

        auto next_it = next(it);

        if(it != active_positions.begin()) {
            auto prev_it = prev(it);
            int prev_pos = *prev_it;
            current_sum -= abs((ll)h[pos] - (ll)h[prev_pos]);

            if(next_it != active_positions.end()) {
                int next_pos = *next_it;
                current_sum += abs((ll)h[prev_pos] - (ll)h[next_pos]);
            }
        }

        if(next_it != active_positions.end()) {
            int next_pos = *next_it;
            current_sum -= abs((ll)h[pos] - (ll)h[next_pos]);
        }

        active_positions.erase(it);
    };

    int curL = 0, curR = -1;

    for(const auto &qq : queries) {
        if(qq.l_idx > qq.r_idx) {
            answers[qq.id] = 0;
            continue;
        }

        // Optimized transition order
        if(curR < qq.r_idx) {
            while(curR < qq.r_idx) add_tree(++curR);
            while(curL < qq.l_idx) remove_tree(curL++);
            while(curL > qq.l_idx) add_tree(--curL);
        } else {
            while(curL > qq.l_idx) add_tree(--curL);
            while(curR > qq.r_idx) remove_tree(curR--);
            while(curL < qq.l_idx) remove_tree(curL++);
        }

        answers[qq.id] = current_sum;
    }

    for(ll ans : answers)
        cout << ans << '\n';

    return 0;
}
