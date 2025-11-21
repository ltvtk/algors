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

    // Reusable arrays for tracking seen values
    vector<int> seen_left(max_val + 1, 0);
    vector<int> seen_right(max_val + 1, 0);
    vector<int> seen_temp(max_val + 1, 0);

    lint beautiful_count = 0;

    // For each j (boundary between X and Y)
    for (int j = 0; j < n - 1; j++) {
        // Calculate max possible distinct count on right side [j+1, n)
        vector<int> changed_temp;
        int max_right_size = 0;
        for (int k = j + 1; k < n; k++) {
            if (seen_temp[a[k]] == 0) {
                seen_temp[a[k]] = 1;
                changed_temp.push_back(a[k]);
                max_right_size++;
            }
        }
        // Clear seen_temp
        for (int x : changed_temp) {
            seen_temp[x] = 0;
        }

        // Build left segments ending at j
        // Map: (size, hash) -> count
        map<pair<int, lint>, int> left_map;
        vector<int> changed_left;
        lint current_hash = 0;
        int current_size = 0;

        for (int i = j; i >= 0; i--) {
            if (seen_left[a[i]] == 0) {
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

        // Find max left size
        int max_left_size = 0;
        for (const auto& p : left_map) {
            max_left_size = max(max_left_size, p.first.first);
        }

        // Build right segments starting at j+1 and count matches
        vector<int> changed_right;
        current_hash = 0;
        current_size = 0;

        for (int k = j + 1; k < n; k++) {
            if (seen_right[a[k]] == 0) {
                seen_right[a[k]] = 1;
                changed_right.push_back(a[k]);
                current_size++;
                current_hash ^= hashes[a[k]];
            }

            // Check if this (size, hash) exists in left_map
            auto it = left_map.find({current_size, current_hash});
            if (it != left_map.end()) {
                beautiful_count += it->second;
            }

            // Early break: if right size exceeds max left size
            if (current_size > max_left_size) {
                break;
            }
        }

        // Clear seen_right
        for (int x : changed_right) {
            seen_right[x] = 0;
        }
    }

    cout << beautiful_count << endl;

    return 0;
}
