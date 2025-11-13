#include <bits/stdc++.h>
using namespace std;

struct Query {
    long long L, R;
    int idx;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<long long> h(n);
    for (int i = 0; i < n; i++) {
        cin >> h[i];
    }

    // Preprocessing: Create indices sorted by height
    vector<pair<long long, int>> sorted_h(n);
    for (int i = 0; i < n; i++) {
        sorted_h[i] = {h[i], i};
    }
    sort(sorted_h.begin(), sorted_h.end());

    vector<Query> queries(q);
    for (int i = 0; i < q; i++) {
        cin >> queries[i].L >> queries[i].R;
        queries[i].idx = i;
    }

    // For small datasets, use simple approach
    if (n * q <= 1e7) {
        vector<long long> answers(q);
        for (int i = 0; i < q; i++) {
            long long L = queries[i].L;
            long long R = queries[i].R;
            long long total = 0;
            long long prev = -1;

            for (int j = 0; j < n; j++) {
                if (h[j] >= L && h[j] <= R) {
                    if (prev != -1) {
                        total += abs(h[j] - prev);
                    }
                    prev = h[j];
                }
            }
            cout << total << '\n';
        }
    } else {
        // For larger datasets, use binary search to find valid range
        vector<long long> answers(q);
        for (int i = 0; i < q; i++) {
            long long L = queries[i].L;
            long long R = queries[i].R;

            // Find indices of trees in [L, R]
            vector<int> valid_indices;
            valid_indices.reserve(n);

            for (int j = 0; j < n; j++) {
                if (h[j] >= L && h[j] <= R) {
                    valid_indices.push_back(j);
                }
            }

            long long total = 0;
            for (size_t j = 1; j < valid_indices.size(); j++) {
                total += abs(h[valid_indices[j]] - h[valid_indices[j-1]]);
            }

            cout << total << '\n';
        }
    }

    return 0;
}
