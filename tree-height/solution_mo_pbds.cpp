#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace std;
using namespace __gnu_pbds;
using ll = long long;

#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

// PBDS ordered set - faster than std::set
template<typename T>
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<int> h(n);
    for(int i = 0; i < n; i++) cin >> h[i];

    // Brute force for small cases
    if((ll)n * q <= 2000000LL) {
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

    // Mo's algorithm with PBDS
    vector<pair<int,int>> sorted_h;
    sorted_h.reserve(n);
    for(int i = 0; i < n; i++)
        sorted_h.push_back({h[i], i});
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

    // Optimal block size - tuned for performance
    int block = max(1, (int)(n / sqrt(q * 0.666)));

    sort(queries.begin(), queries.end(), [&](const Query &a, const Query &b) {
        int ba = a.l_idx / block;
        int bb = b.l_idx / block;
        if(ba != bb) return ba < bb;
        return (ba & 1) ? (a.r_idx < b.r_idx) : (a.r_idx > b.r_idx);
    });

    ordered_set<int> active_positions;
    ll current_sum = 0;
    vector<ll> answers(q);

    auto add_tree = [&](int sorted_idx) {
        int pos = sorted_h[sorted_idx].second;
        active_positions.insert(pos);

        int idx = active_positions.order_of_key(pos);
        int sz = active_positions.size();

        // Get predecessor
        if(idx > 0) {
            int prev_pos = *active_positions.find_by_order(idx - 1);
            current_sum += abs((ll)h[pos] - (ll)h[prev_pos]);

            // Remove old edge if successor exists
            if(idx + 1 < sz) {
                int next_pos = *active_positions.find_by_order(idx + 1);
                current_sum -= abs((ll)h[prev_pos] - (ll)h[next_pos]);
            }
        }

        // Get successor
        if(idx + 1 < sz) {
            int next_pos = *active_positions.find_by_order(idx + 1);
            current_sum += abs((ll)h[pos] - (ll)h[next_pos]);
        }
    };

    auto remove_tree = [&](int sorted_idx) {
        int pos = sorted_h[sorted_idx].second;

        int idx = active_positions.order_of_key(pos);
        int sz = active_positions.size();

        // Get predecessor and successor before removal
        int prev_pos = -1, next_pos = -1;
        if(idx > 0) {
            prev_pos = *active_positions.find_by_order(idx - 1);
            current_sum -= abs((ll)h[pos] - (ll)h[prev_pos]);
        }
        if(idx + 1 < sz) {
            next_pos = *active_positions.find_by_order(idx + 1);
            current_sum -= abs((ll)h[pos] - (ll)h[next_pos]);
        }

        // Add edge between predecessor and successor if both exist
        if(prev_pos != -1 && next_pos != -1) {
            current_sum += abs((ll)h[prev_pos] - (ll)h[next_pos]);
        }

        active_positions.erase(pos);
    };

    int curL = 0, curR = -1;

    for(const auto &qq : queries) {
        if(qq.l_idx > qq.r_idx) {
            answers[qq.id] = 0;
            continue;
        }

        while(curL > qq.l_idx) add_tree(--curL);
        while(curR < qq.r_idx) add_tree(++curR);
        while(curL < qq.l_idx) remove_tree(curL++);
        while(curR > qq.r_idx) remove_tree(curR--);

        answers[qq.id] = current_sum;
    }

    for(ll ans : answers)
        cout << ans << '\n';

    return 0;
}
