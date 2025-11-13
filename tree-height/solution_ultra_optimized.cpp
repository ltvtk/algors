#pragma GCC optimize("O3,unroll-loops")
// #pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt") // Disabled for compatibility
#include <bits/stdc++.h>
using namespace std;

namespace FastIO {
    static constexpr int BUFFER_SIZE = 1 << 18;
    char input_buffer[BUFFER_SIZE];
    char output_buffer[BUFFER_SIZE];
    int input_pos = 0, input_len = 0;
    int output_pos = 0;

    inline char next_char() {
        if (input_pos == input_len) {
            input_len = fread(input_buffer, 1, BUFFER_SIZE, stdin);
            input_pos = 0;
            if (input_len == 0) return EOF;
        }
        return input_buffer[input_pos++];
    }

    inline void read_int(int& x) {
        char c = next_char();
        while (c <= ' ') c = next_char();
        int sign = 1;
        if (c == '-') {
            sign = -1;
            c = next_char();
        }
        x = 0;
        while (c >= '0' && c <= '9') {
            x = x * 10 + (c - '0');
            c = next_char();
        }
        x *= sign;
    }

    inline void read_long(long long& x) {
        char c = next_char();
        while (c <= ' ') c = next_char();
        int sign = 1;
        if (c == '-') {
            sign = -1;
            c = next_char();
        }
        x = 0;
        while (c >= '0' && c <= '9') {
            x = x * 10 + (c - '0');
            c = next_char();
        }
        x *= sign;
    }

    inline void write_char(char c) {
        if (output_pos == BUFFER_SIZE) {
            fwrite(output_buffer, 1, output_pos, stdout);
            output_pos = 0;
        }
        output_buffer[output_pos++] = c;
    }

    inline void write_long(long long x) {
        if (x == 0) {
            write_char('0');
            return;
        }
        if (x < 0) {
            write_char('-');
            x = -x;
        }
        char temp[20];
        int len = 0;
        while (x > 0) {
            temp[len++] = '0' + (x % 10);
            x /= 10;
        }
        for (int i = len - 1; i >= 0; i--) {
            write_char(temp[i]);
        }
    }

    inline void flush() {
        fwrite(output_buffer, 1, output_pos, stdout);
        output_pos = 0;
    }
}

int main() {
    int n, q;
    FastIO::read_int(n);
    FastIO::read_int(q);

    vector<long long> h(n);
    for (int i = 0; i < n; i++) {
        FastIO::read_long(h[i]);
    }

    while (q--) {
        long long L, R;
        FastIO::read_long(L);
        FastIO::read_long(R);

        long long total = 0;
        long long prev = -1;
        bool found_first = false;

        for (int i = 0; i < n; i++) {
            if (h[i] >= L && h[i] <= R) {
                if (found_first) {
                    long long diff = h[i] - prev;
                    total += (diff >= 0) ? diff : -diff;
                } else {
                    found_first = true;
                }
                prev = h[i];
            }
        }

        FastIO::write_long(total);
        FastIO::write_char('\n');
    }

    FastIO::flush();
    return 0;
}
