#pragma GCC optimize("O3,unroll-loops")
#include <bits/stdc++.h>
using namespace std;
using lint = long long;

// Custom hash for pair<int, lint>
struct PairHash {
    inline size_t operator()(const pair<int, lint>& p) const {
        return hash<lint>{}(p.first ^ (p.second << 1));
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    // Generate random hashes for each value
    int max_val = *max_element(a.begin(), a.end());
    vector<lint> hashes(max_val + 1);
    mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
    for (int i = 1; i <= max_val; i++) {
        hashes[i] = rng();
    }

    // Pre-compute: For each position j, number of distinct values from j to n-1
    vector<int> distinct_from(n + 1, 0);
    {
        unordered_set<int> seen;
        for (int i = n - 1; i >= 0; i--) {
            seen.insert(a[i]);
            distinct_from[i] = seen.size();
        }
    }

    // Reusable arrays for tracking seen values
    vector<char> seen_left(max_val + 1, 0);
    vector<char> seen_right(max_val + 1, 0);

    // Pre-allocate space
    vector<int> changed_left, changed_right;
    changed_left.reserve(n);
    changed_right.reserve(n);

    lint beautiful_count = 0;

    // For each j (boundary between X and Y)
    for (int j = 0; j < n - 1; j++) {
        int max_right_size = distinct_from[j + 1];

        // Build left segments ending at j
        unordered_map<pair<int, lint>, int, PairHash> left_map;
        left_map.reserve(min(j + 1, max_right_size));

        changed_left.clear();
        lint current_hash = 0;
        int current_size = 0;

        for (int i = j; i >= 0; i--) {
            if (!seen_left[a[i]]) {
                // Early break: if left size exceeds max possible right size
                if (current_size + 1 > max_right_size) {
                    break;
                }
                seen_left[a[i]] = 1;
                changed_left.push_back(a[i]);
                current_size++;
                current_hash ^= hashes[a[i]];
            }
            left_map[{current_size, current_hash}]++;
        }

        // Clear seen_left
        for (int x : changed_left) {
            seen_left[x] = 0;
        }

        // Find max left size for early break
        int max_left_size = current_size;  // current_size at end is max

        // Build right segments starting at j+1 and count matches
        changed_right.clear();
        current_hash = 0;
        current_size = 0;

        for (int k = j + 1; k < n; k++) {
            if (!seen_right[a[k]]) {
                seen_right[a[k]] = 1;
                changed_right.push_back(a[k]);
                current_size++;
                current_hash ^= hashes[a[k]];

                // Early break: if right size exceeds max left size
                if (current_size > max_left_size) {
                    // Clear and break
                    for (int x : changed_right) {
                        seen_right[x] = 0;
                    }
                    break;
                }
            }

            // Check if this (size, hash) exists in left_map
            auto it = left_map.find({current_size, current_hash});
            if (it != left_map.end()) {
                beautiful_count += it->second;
            }
        }

        // Clear seen_right if not already cleared
        if (current_size <= max_left_size) {
            for (int x : changed_right) {
                seen_right[x] = 0;
            }
        }
    }

    cout << beautiful_count << endl;

    return 0;
}
