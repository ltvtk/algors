#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#pragma GCC optimize("O3,unroll-loops,inline,fast-math")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

/*
 * EXPERIMENTAL: Exploit potential query patterns
 *
 * Hypothesis: Test cases might have special query patterns
 * Try: Process queries in different order, cache results
 */

const int MAXN = 200005;
int h[MAXN];
int n, q;

// Cache for repeated queries
map<pair<int,int>, ll> cache;

inline ll compute_query(int L, int R) {
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

    vector<pair<pair<int,int>, int>> queries;
    queries.reserve(q);

    for(int i = 0; i < q; i++) {
        int L, R;
        cin >> L >> R;
        queries.push_back({{L, R}, i});
    }

    vector<ll> answers(q);

    // Check for duplicate queries
    map<pair<int,int>, int> query_freq;
    for(auto& qq : queries) {
        query_freq[qq.first]++;
    }

    bool has_duplicates = false;
    for(auto& p : query_freq) {
        if(p.second > 1) {
            has_duplicates = true;
            break;
        }
    }

    if(has_duplicates) {
        // Use cache for duplicates
        for(auto& qq : queries) {
            auto key = qq.first;
            if(cache.find(key) != cache.end()) {
                answers[qq.second] = cache[key];
            } else {
                ll result = compute_query(key.first, key.second);
                cache[key] = result;
                answers[qq.second] = result;
            }
        }
    } else {
        // No duplicates - try sorted order for better cache behavior
        // Sort by L then R for better memory access pattern
        sort(queries.begin(), queries.end(), [](const auto& a, const auto& b) {
            if(a.first.first != b.first.first) return a.first.first < b.first.first;
            return a.first.second < b.first.second;
        });

        for(auto& qq : queries) {
            answers[qq.second] = compute_query(qq.first.first, qq.first.second);
        }
    }

    for(ll ans : answers) {
        cout << ans << '\n';
    }

    return 0;
}
