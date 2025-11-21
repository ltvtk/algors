#include <bits/stdc++.h>
using namespace std;

// Custom hash for vector<int>
struct VectorHash {
    size_t operator()(const vector<int>& v) const {
        size_t hash = 0;
        for (int x : v) {
            hash ^= std::hash<int>{}(x) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        }
        return hash;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> a(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
    }

    long long count = 0;

    // For each j (boundary between X and Y)
    for (int j = 1; j < n; j++) {
        // Use unordered_map with custom hash for better performance
        unordered_map<vector<int>, int, VectorHash> left_sets;

        set<int> current_set;

        // Build all left segments ending at j
        for (int i = j; i >= 1; i--) {
            current_set.insert(a[i]);
            // Convert set to sorted vector for hashing
            vector<int> vec(current_set.begin(), current_set.end());
            left_sets[vec]++;
        }

        // Build all right segments starting at j+1
        current_set.clear();
        for (int k = j + 1; k <= n; k++) {
            current_set.insert(a[k]);
            // Convert set to sorted vector for hashing
            vector<int> vec(current_set.begin(), current_set.end());
            // Count how many left segments have the same set
            if (left_sets.count(vec)) {
                count += left_sets[vec];
            }
        }
    }

    cout << count << endl;

    return 0;
}
