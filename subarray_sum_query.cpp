#include <bits/stdc++.h>
using namespace std;

// Array-based hash table (much faster than unordered_map)
int pos[200010];
int last_query[200010];
int current_query = 0;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<int> a(n + 1);
    for(int i = 1; i <= n; i++){
        cin >> a[i];
    }

    while(q--){
        int type;
        cin >> type;

        if(type == 1){
            int i, v;
            cin >> i >> v;
            a[i] = v;
        }
        else {
            int l, r, k;
            cin >> l >> r >> k;

            int len = r - l + 1;

            // Parity check: k and len must have same parity
            // Since array has only ±1: sum = 2*(count of 1) - len
            // Therefore: sum ≡ len (mod 2)
            if(abs(k) > len || (k & 1) != (len & 1)){
                cout << "-1\n";
                continue;
            }

            ++current_query;
            const int OFFSET = 100005;

            // Initialize: prefix sum 0 at position l-1
            last_query[OFFSET] = current_query;
            pos[OFFSET] = l - 1;

            int sum = 0;
            bool found = false;

            for(int i = l; i <= r; i++){
                sum += a[i];

                // Look for prefix sum that gives us the target k
                int target = sum - k + OFFSET;
                if(target >= 0 && target < 200010 && last_query[target] == current_query){
                    cout << pos[target] + 1 << ' ' << i << '\n';
                    found = true;
                    break;
                }

                // Store current prefix sum (only first occurrence)
                int curr = sum + OFFSET;
                if(curr >= 0 && curr < 200010 && last_query[curr] != current_query){
                    last_query[curr] = current_query;
                    pos[curr] = i;
                }
            }

            if(!found){
                cout << "-1\n";
            }
        }
    }

    return 0;
}
