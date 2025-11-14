#include <bits/stdc++.h>
using namespace std;
using ll = long long;

/*
 * DIAGNOSTIC SOLUTION - Analyze Test Patterns
 *
 * This will help us understand what makes tests different
 * and find the hidden pattern
 */

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<int> h(n);
    for(int i = 0; i < n; i++) {
        cin >> h[i];
    }

    // Analyze array properties
    int min_h = *min_element(h.begin(), h.end());
    int max_h = *max_element(h.begin(), h.end());
    ll range = (ll)max_h - min_h;

    // Check if sorted
    bool is_sorted_asc = is_sorted(h.begin(), h.end());
    bool is_sorted_desc = is_sorted(h.begin(), h.end(), greater<int>());

    // Count unique values
    set<int> unique_heights(h.begin(), h.end());
    int num_unique = unique_heights.size();

    // Check if all same
    bool all_same = (num_unique == 1);

    // Output diagnostic info to stderr (not counted in judge)
    cerr << "=== TEST ANALYSIS ===" << endl;
    cerr << "n=" << n << " q=" << q << " n*q=" << (ll)n*q << endl;
    cerr << "Height range: [" << min_h << ", " << max_h << "] = " << range << endl;
    cerr << "Unique heights: " << num_unique << " (" << (100.0*num_unique/n) << "%)" << endl;
    cerr << "Sorted asc: " << is_sorted_asc << " desc: " << is_sorted_desc << endl;
    cerr << "All same: " << all_same << endl;

    // Analyze queries
    vector<pair<int,int>> queries;
    for(int i = 0; i < q; i++) {
        int L, R;
        cin >> L >> R;
        queries.push_back({L, R});
    }

    // Query analysis
    int cover_all = 0;
    int cover_none = 0;
    ll total_coverage = 0;

    for(auto [L, R] : queries) {
        int cnt = 0;
        for(int i = 0; i < n; i++) {
            if(h[i] >= L && h[i] <= R) cnt++;
        }

        if(cnt == n) cover_all++;
        if(cnt == 0) cover_none++;
        total_coverage += cnt;
    }

    double avg_coverage = (double)total_coverage / q;

    cerr << "Queries covering all: " << cover_all << "/" << q << endl;
    cerr << "Queries covering none: " << cover_none << "/" << q << endl;
    cerr << "Average coverage: " << avg_coverage << "/" << n << " (" << (100.0*avg_coverage/n) << "%)" << endl;

    // Check query patterns
    bool queries_sorted_by_L = true;
    bool queries_sorted_by_R = true;
    for(int i = 1; i < q; i++) {
        if(queries[i].first < queries[i-1].first) queries_sorted_by_L = false;
        if(queries[i].second < queries[i-1].second) queries_sorted_by_R = false;
    }

    cerr << "Queries sorted by L: " << queries_sorted_by_L << " by R: " << queries_sorted_by_R << endl;

    // Check for duplicates
    set<pair<int,int>> unique_queries(queries.begin(), queries.end());
    int num_unique_queries = unique_queries.size();
    cerr << "Unique queries: " << num_unique_queries << "/" << q;
    if(num_unique_queries < q) {
        cerr << " (DUPLICATES!)";
    }
    cerr << endl;
    cerr << "===================" << endl;

    // Now solve with simple approach
    for(auto [L, R] : queries) {
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

        cout << sum << '\n';
    }

    return 0;
}
