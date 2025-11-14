#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

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

            int len = r - l + 1;

            // Early checks for impossible cases
            if (abs(k) > len || (k & 1) != (len & 1)) {
                cout << "-1\n";
                continue;
            }

            // For small ranges, brute force is faster (cache-friendly)
            if (len <= 100) {
                bool found = false;
                for (int i = l; i <= r && !found; i++) {
                    int sum = 0;
                    for (int j = i; j <= r; j++) {
                        sum += a[j];
                        if (sum == k) {
                            cout << i << " " << j << "\n";
                            found = true;
                            break;
                        }
                    }
                }
                if (!found) cout << "-1\n";
                continue;
            }

            // For larger ranges, use hash map approach
            unordered_map<int, int> first_pos;
            first_pos.reserve(len);  // Avoid rehashing
            first_pos[0] = l - 1;

            int prefix = 0;
            bool found = false;

            for (int i = l; i <= r; i++) {
                prefix += a[i];
                int target = prefix - k;

                auto it = first_pos.find(target);
                if (it != first_pos.end()) {
                    cout << (it->second + 1) << " " << i << "\n";
                    found = true;
                    break;
                }

                // Only store first occurrence
                if (first_pos.find(prefix) == first_pos.end()) {
                    first_pos[prefix] = i;
                }
            }

            if (!found) {
                cout << "-1\n";
            }
        }
    }

    return 0;
}
