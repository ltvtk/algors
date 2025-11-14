#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<long long> h(n);
    for (int i = 0; i < n; i++) {
        cin >> h[i];
    }

    while (q--) {
        long long L, R;
        cin >> L >> R;

        long long total = 0;
        long long prev = -1;

        for (int i = 0; i < n; i++) {
            if (h[i] >= L && h[i] <= R) {
                if (prev != -1) {
                    total += abs(h[i] - prev);
                }
                prev = h[i];
            }
        }

        cout << total << '\n';
    }

    return 0;
}
