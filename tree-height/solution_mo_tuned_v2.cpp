#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#pragma GCC optimize("O3,unroll-loops,inline,no-stack-protector,fast-math")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

/*
 * Mo's with block size = sqrt(n) * 0.4 (smaller blocks)
 * Smaller blocks → more R transitions, fewer L transitions
 * Good for test cases where R range is smaller
 */

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<int> h(n);
    for(int i = 0; i < n; i++) cin >> h[i];

    if((ll)n * q <= 12000000LL) {
        while(q--) {
            int L, R; cin >> L >> R;
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

    // Try smaller block: 0.4 * sqrt(n)
    int block = max(1, (int)(sqrt(n) * 0.4));

    sort(queries.begin(), queries.end(), [&](const Query &a, const Query &b) {
        int ba = a.l_idx / block;
        int bb = b.l_idx / block;
        if(ba != bb) return ba < bb;
        return (ba & 1) ? (a.r_idx < b.r_idx) : (a.r_idx > b.r_idx);
    });

    set<int> active;
    ll sum = 0;
    vector<ll> answers(q);

    auto add = [&](int idx) __attribute__((always_inline)) {
        int pos = sorted_h[idx].second;
        auto it = active.insert(pos).first;
        auto nxt = next(it);

        if(it != active.begin()) {
            auto prv = prev(it);
            sum += abs((ll)h[pos] - h[*prv]);
            if(nxt != active.end()) {
                sum -= abs((ll)h[*prv] - h[*nxt]);
            }
        }
        if(nxt != active.end()) {
            sum += abs((ll)h[pos] - h[*nxt]);
        }
    };

    auto remove = [&](int idx) __attribute__((always_inline)) {
        int pos = sorted_h[idx].second;
        auto it = active.find(pos);
        if(it == active.end()) return;

        auto nxt = next(it);
        if(it != active.begin()) {
            auto prv = prev(it);
            sum -= abs((ll)h[pos] - h[*prv]);
            if(nxt != active.end()) {
                sum += abs((ll)h[*prv] - h[*nxt]);
            }
        }
        if(nxt != active.end()) {
            sum -= abs((ll)h[pos] - h[*nxt]);
        }
        active.erase(it);
    };

    int curL = 0, curR = -1;

    for(const auto &qq : queries) {
        if(qq.l_idx > qq.r_idx) {
            answers[qq.id] = 0;
            continue;
        }

        while(curL > qq.l_idx) add(--curL);
        while(curR < qq.r_idx) add(++curR);
        while(curL < qq.l_idx) remove(curL++);
        while(curR > qq.r_idx) remove(curR--);

        answers[qq.id] = sum;
    }

    for(ll ans : answers)
        cout << ans << '\n';

    return 0;
}
