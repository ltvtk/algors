#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#pragma GCC optimize("O3,unroll-loops,inline")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

/*
 * MATHEMATICAL INSIGHT: Contribution Counting
 *
 * Key idea: Instead of "for each query, compute sum",
 * think "for each pair (i,j), when does it contribute?"
 *
 * Pair (i,j) with i < j contributes |h[i] - h[j]| to query [L,R] when:
 * 1. h[i] ∈ [L, R]
 * 2. h[j] ∈ [L, R]
 * 3. ALL trees between i and j are NOT in [L, R]
 *
 * Condition 3 is key: (i,j) are "adjacent" in filtered array
 */

const int MAXN = 200005;
int h[MAXN];
int n, q;

// For each position i, precompute potential "next" positions
// next_when_filtered[i][condition] = next position after i that satisfies condition
struct Contribution {
    int pos1, pos2;        // positions
    int min_h, max_h;      // both must be in [L, R]
    int exclude_min, exclude_max; // all between must be outside [L, R]
    ll value;              // |h[pos1] - h[pos2]|
};

vector<Contribution> contributions;

void precompute_contributions() {
    // For each pair of adjacent positions in SOME filtered view
    // We need to know when they become adjacent

    // Brute force for now: check all pairs (i, j) where i < j
    // and determine conditions for them to be adjacent

    for(int i = 0; i < n; i++) {
        for(int j = i + 1; j < n; j++) {
            // (i, j) are adjacent in filter [L, R] if:
            // - h[i] ∈ [L, R]
            // - h[j] ∈ [L, R]
            // - For all k: i < k < j, h[k] ∉ [L, R]

            Contribution c;
            c.pos1 = i;
            c.pos2 = j;
            c.min_h = min(h[i], h[j]);
            c.max_h = max(h[i], h[j]);

            // Find the range of heights between i and j
            int min_between = INT_MAX;
            int max_between = INT_MIN;

            for(int k = i + 1; k < j; k++) {
                min_between = min(min_between, h[k]);
                max_between = max(max_between, h[k]);
            }

            c.exclude_min = min_between;
            c.exclude_max = max_between;
            c.value = abs((ll)h[i] - h[j]);

            contributions.push_back(c);
        }
    }
}

ll query_with_contributions(int L, int R) {
    ll sum = 0;

    for(const auto& c : contributions) {
        // Check if this contribution applies
        bool i_in = (h[c.pos1] >= L && h[c.pos1] <= R);
        bool j_in = (h[c.pos2] >= L && h[c.pos2] <= R);

        if(!i_in || !j_in) continue;

        // Check if all between are excluded
        bool all_excluded = true;
        for(int k = c.pos1 + 1; k < c.pos2; k++) {
            if(h[k] >= L && h[k] <= R) {
                all_excluded = false;
                break;
            }
        }

        if(all_excluded) {
            sum += c.value;
        }
    }

    return sum;
}

// Standard fast computation for comparison
ll compute_standard(int L, int R) {
    ll sum = 0;
    int prev = -1;

    for(int i = 0; i < n; i++) {
        if(h[i] >= L && h[i] <= R) {
            if(prev != -1) {
                sum += abs((ll)h[i] - prev);
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

    // Decide strategy based on n
    if(n <= 1000) {
        // Use contribution approach for small n
        precompute_contributions();

        while(q--) {
            int L, R;
            cin >> L >> R;
            cout << query_with_contributions(L, R) << '\n';
        }
    } else {
        // Use standard for larger n
        while(q--) {
            int L, R;
            cin >> L >> R;
            cout << compute_standard(L, R) << '\n';
        }
    }

    return 0;
}
