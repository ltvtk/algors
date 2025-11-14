#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#pragma GCC optimize("O3,unroll-loops,inline,no-stack-protector,fast-math")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

constexpr int MAXN = 200005;
int h[MAXN];
pair<int, int> sorted_h[MAXN];

struct Query {
    int l_idx, r_idx, id;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    for(int i = 0; i < n; i++) {
        cin >> h[i];
        sorted_h[i] = {h[i], i};
    }

    // Adaptive threshold
    if((ll)n * q <= 4000000LL) {
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

    sort(sorted_h, sorted_h + n);

    vector<Query> queries;
    queries.reserve(q);

    for(int i = 0; i < q; i++) {
        int L, R;
        cin >> L >> R;
        int l_idx = lower_bound(sorted_h, sorted_h + n, make_pair(L, -1)) - sorted_h;
        int r_idx = upper_bound(sorted_h, sorted_h + n, make_pair(R, n)) - sorted_h - 1;
        queries.push_back({l_idx, r_idx, i});
    }

    // Optimized block size - tested empirically
    int block = max(1, (int)sqrt(n * 2.0 / 3.0));
    if(q > n) block = max(1, (int)(n / sqrt(q * 0.6)));

    sort(queries.begin(), queries.end(), [&](const Query &a, const Query &b) {
        int ba = a.l_idx / block;
        int bb = b.l_idx / block;
        if(ba != bb) return ba < bb;
        return (ba & 1) ? (a.r_idx < b.r_idx) : (a.r_idx > b.r_idx);
    });

    set<int> active;
    ll sum = 0;
    vector<ll> answers(q);

    int curL = 0, curR = -1;

    for(const Query &qq : queries) {
        if(qq.l_idx > qq.r_idx) {
            answers[qq.id] = 0;
            continue;
        }

        // Move pointers
        while(curL > qq.l_idx) {
            --curL;
            int pos = sorted_h[curL].second;
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
        }

        while(curR < qq.r_idx) {
            ++curR;
            int pos = sorted_h[curR].second;
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
        }

        while(curL < qq.l_idx) {
            int pos = sorted_h[curL].second;
            auto it = active.find(pos);
            if(it != active.end()) {
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
            }
            ++curL;
        }

        while(curR > qq.r_idx) {
            int pos = sorted_h[curR].second;
            auto it = active.find(pos);
            if(it != active.end()) {
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
            }
            --curR;
        }

        answers[qq.id] = sum;
    }

    for(ll ans : answers)
        cout << ans << '\n';

    return 0;
}
