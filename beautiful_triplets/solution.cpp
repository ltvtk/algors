#pragma GCC optimize("O3,unroll-loops")
#include <bits/stdc++.h>
using namespace std;
using lint = long long;

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

    // Reusable arrays
    vector<char> seen_left(max_val + 1, 0);
    vector<char> seen_right(max_val + 1, 0);
    vector<int> changed_left, changed_right;
    changed_left.reserve(n);
    changed_right.reserve(n);

    lint beautiful_count = 0;

    // For each j (boundary between X and Y)
    for (int j = 0; j < n - 1; j++) {
        int max_right_size = distinct_from[j + 1];

        // Build left segments using map (more stable than unordered_map)
        map<pair<int, lint>, int> left_map;
        changed_left.clear();
        lint current_hash = 0;
        int current_size = 0;

        for (int i = j; i >= 0; i--) {
            if (!seen_left[a[i]]) {
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

        int max_left_size = current_size;

        // Build right segments
        changed_right.clear();
        current_hash = 0;
        current_size = 0;

        for (int k = j + 1; k < n; k++) {
            if (!seen_right[a[k]]) {
                seen_right[a[k]] = 1;
                changed_right.push_back(a[k]);
                current_size++;
                current_hash ^= hashes[a[k]];

                if (current_size > max_left_size) {
                    for (int x : changed_right) {
                        seen_right[x] = 0;
                    }
                    break;
                }
            }

            auto it = left_map.find({current_size, current_hash});
            if (it != left_map.end()) {
                beautiful_count += it->second;
            }
        }

        if (current_size <= max_left_size) {
            for (int x : changed_right) {
                seen_right[x] = 0;
            }
        }
    }

    cout << beautiful_count << endl;

    return 0;
}
