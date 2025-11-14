#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, q;
    cin >> n >> q;

    vector<int> a(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
    }

    while (q--) {
        int type;
        cin >> type;

        if (type == 1) {
            int idx, v;
            cin >> idx >> v;
            a[idx] = v;
        } else {
            int l, r, k;
            cin >> l >> r >> k;

            // Compute prefix sums for range [l-1, r]
            vector<int> prefix(r - l + 2);
            prefix[0] = 0;
            for (int i = l; i <= r; i++) {
                prefix[i - l + 1] = prefix[i - l] + a[i];
            }

            // Use hash map to store prefix values and their positions
            // For each y, find if there exists x such that prefix[y] - prefix[x-1] = k
            unordered_map<int, int> prefixMap;
            prefixMap[0] = l - 1;  // prefix[l-1] = 0

            bool found = false;
            for (int y = l; y <= r && !found; y++) {
                int currentSum = prefix[y - l + 1];
                int target = currentSum - k;

                // Check if we've seen this prefix value before
                if (prefixMap.find(target) != prefixMap.end()) {
                    int x = prefixMap[target] + 1;
                    if (x >= l && x <= y) {
                        cout << x << " " << y << "\n";
                        found = true;
                    }
                }

                // Store current position if not already stored
                if (!found && prefixMap.find(currentSum) == prefixMap.end()) {
                    prefixMap[currentSum] = y;
                }
            }

            if (!found) {
                cout << "-1\n";
            }
        }
    }

    return 0;
}
