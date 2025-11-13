#include <bits/stdc++.h>
using namespace std;

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
        // Map to store bitset representations from left segments [i, j]
        // Using unsigned long long to represent bitset since A_i <= 50
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
            // Count how many left segments have the same set
            if (left_sets.count(current_set)) {
                count += left_sets[current_set];
            }
        }
    }

    cout << count << endl;

    return 0;
}
