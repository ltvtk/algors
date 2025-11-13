#include <bits/stdc++.h>
using namespace std;

// Solution using bitset for small values (A_i <= 64)
long long solve_with_bitmask(int n, vector<int>& a) {
    long long count = 0;

    // For each j (boundary between X and Y)
    for (int j = 1; j < n; j++) {
        map<unsigned long long, int> left_sets;
        unsigned long long current_set = 0;

        // Build all left segments ending at j
        for (int i = j; i >= 1; i--) {
            current_set |= (1ULL << a[i]);
            left_sets[current_set]++;
        }

        // Build all right segments starting at j+1
        current_set = 0;
        for (int k = j + 1; k <= n; k++) {
            current_set |= (1ULL << a[k]);
            if (left_sets.count(current_set)) {
                count += left_sets[current_set];
            }
        }
    }

    return count;
}

// General solution using set and hash
long long solve_general(int n, vector<int>& a) {
    long long count = 0;

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

    // For each j (boundary between X and Y)
    for (int j = 1; j < n; j++) {
        unordered_map<vector<int>, int, VectorHash> left_sets;
        set<int> current_set;

        // Build all left segments ending at j
        for (int i = j; i >= 1; i--) {
            current_set.insert(a[i]);
            vector<int> vec(current_set.begin(), current_set.end());
            left_sets[vec]++;
        }

        // Build all right segments starting at j+1
        current_set.clear();
        for (int k = j + 1; k <= n; k++) {
            current_set.insert(a[k]);
            vector<int> vec(current_set.begin(), current_set.end());
            if (left_sets.count(vec)) {
                count += left_sets[vec];
            }
        }
    }

    return count;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> a(n + 1);
    int max_val = 0;
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
        max_val = max(max_val, a[i]);
    }

    long long result;

    // Choose algorithm based on constraints
    if (max_val <= 63) {
        // Can use bitmask (unsigned long long has 64 bits)
        result = solve_with_bitmask(n, a);
    } else {
        // Use general solution with hash
        result = solve_general(n, a);
    }

    cout << result << endl;

    return 0;
}
