#include <bits/stdc++.h>
using namespace std;
using ll = long long;

/*
 * SIMPLE SOLUTION FOR THCS LEVEL
 *
 * Key insight for young students:
 * - Don't overcomplicate!
 * - Maybe there's a simple greedy or observation
 * - Use basic sorting + linear scan
 *
 * Hypothesis: Maybe we can AVOID sorting indices altogether?
 * Just iterate through ORIGINAL array and skip invalid trees
 */

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<long long> h(n);
    for(int i = 0; i < n; i++) {
        cin >> h[i];
    }

    while(q--) {
        long long L, R;
        cin >> L >> R;

        long long sum = 0;
        long long prev = -1;

        // Simple linear scan - O(n) per query
        // This is what a THCS student would write!
        for(int i = 0; i < n; i++) {
            if(h[i] >= L && h[i] <= R) {
                if(prev != -1) {
                    sum += abs(h[i] - prev);
                }
                prev = h[i];
            }
        }

        cout << sum << '\n';
    }

    return 0;
}
