#include <iostream>
using namespace std;

// Tối ưu: dùng số học thay vì string
inline int sum_digits(long long n) {
    int sum = 0;
    while (n > 0) {
        sum += n % 10;
        n /= 10;
    }
    return sum;
}

int main() {
    // Tối ưu I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int t;
    cin >> t;

    while (t--) {
        long long x;
        cin >> x;

        bool found = false;
        long long limit = min(x - 1, 5000LL);

        for (long long a = 1; a <= limit; ++a) {
            long long b = x - a;

            if (sum_digits(a) == sum_digits(b)) {
                cout << a << " " << b << "\n";
                found = true;
                break;
            }
        }

        if (!found) {
            cout << "-1\n";
        }
    }

    return 0;
}
