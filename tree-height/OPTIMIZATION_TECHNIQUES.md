# Kỹ Thuật Tối Ưu C++ cho Competitive Programming

## 1. Fast I/O

### Cơ bản (solution_optimized.cpp)
```cpp
ios_base::sync_with_stdio(false);
cin.tie(nullptr);
```

**Lợi ích**:
- Tắt đồng bộ giữa C stdio và C++ iostream
- Tăng tốc cin/cout lên 2-3 lần
- Đơn giản, dễ nhớ, ít bug

**Khi nào dùng**: Luôn luôn trong competitive programming

### Custom Buffer I/O (solution_ultra_optimized.cpp)
```cpp
namespace FastIO {
    char input_buffer[BUFFER_SIZE];
    inline char next_char() { /* ... */ }
    inline void read_int(int& x) { /* ... */ }
}
```

**Lợi ích**:
- Nhanh hơn cin/cout 5-10 lần
- Đọc/ghi trực tiếp từ buffer

**Nhược điểm**:
- Code dài, khó debug
- Dễ mắc lỗi implementation

**Khi nào dùng**: Khi I/O là bottleneck (n, q > 10^5)

## 2. Compiler Optimizations

### Pragma Optimizations
```cpp
#pragma GCC optimize("O3,unroll-loops")
```

**Tác dụng**:
- **O3**: Tối ưu hóa tích cực nhất
- **unroll-loops**: Mở vòng lặp để giảm overhead

**Tăng tốc**: 10-30%

### Target Specific Instructions
```cpp
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
```

**Tác dụng**:
- **avx2**: SIMD instructions (xử lý nhiều data cùng lúc)
- **bmi/bmi2**: Bit manipulation instructions
- **popcnt**: Đếm số bit 1 nhanh

**Lưu ý**: Không phải CPU nào cũng hỗ trợ!

## 3. Memory Optimizations

### Vector Reserve
```cpp
vector<int> v;
v.reserve(n);  // Cấp phát trước n phần tử
```

**Lợi ích**:
- Tránh reallocation (O(n) operations)
- Giảm memory fragmentation

### Array vs Vector
```cpp
// Nhanh hơn
int arr[MAX_N];

// Chậm hơn một chút
vector<int> v(n);
```

**Khi nào dùng array**: Khi kích thước biết trước và không quá lớn

## 4. Algorithm Optimizations

### Branch Prediction
```cpp
// Tốt: điều kiện dễ dự đoán
if (found_first) {  // true hầu hết thời gian
    total += abs(h[i] - prev);
}

// Xấu: điều kiện khó dự đoán
if (random_condition) { /* ... */ }
```

### Inline Functions
```cpp
inline long long abs_diff(long long a, long long b) {
    return (a > b) ? (a - b) : (b - a);
}
```

**Lợi ích**: Giảm function call overhead

### Avoid abs() for integers
```cpp
// Nhanh hơn
long long diff = (a > b) ? (a - b) : (b - a);

// Chậm hơn
long long diff = abs(a - b);
```

## 5. Complexity Optimizations

### Binary Search
Khi cần tìm phần tử trong mảng đã sắp xếp:
```cpp
// O(n)
for (int i = 0; i < n; i++) {
    if (arr[i] == target) break;
}

// O(log n)
int pos = lower_bound(arr, arr+n, target) - arr;
```

### Preprocessing
Tính toán trước thay vì tính lại nhiều lần:
```cpp
// Xấu: O(n*q)
for each query:
    sum = 0
    for i in range(L, R):
        sum += arr[i]

// Tốt: O(n + q)
prefix[0] = 0
for i in range(1, n+1):
    prefix[i] = prefix[i-1] + arr[i-1]

for each query:
    sum = prefix[R+1] - prefix[L]
```

## 6. So Sánh Hiệu Năng

### Ước lượng thời gian (với n = q = 2×10^5)

| Kỹ thuật | Speedup | Example Time |
|----------|---------|--------------|
| Base (cin/cout) | 1x | ~3.0s |
| + Fast I/O | 2-3x | ~1.5s |
| + Pragma O3 | 3-4x | ~1.0s |
| + Custom I/O | 5-10x | ~0.5s |
| + All optimizations | 10-15x | ~0.3s |

## 7. Checklist Tối Ưu

Khi cần tối ưu code C++:

- [ ] Sử dụng `ios_base::sync_with_stdio(false); cin.tie(nullptr);`
- [ ] Thêm `#pragma GCC optimize("O3,unroll-loops")`
- [ ] Reserve vector capacity khi biết trước kích thước
- [ ] Sử dụng const reference cho parameter lớn
- [ ] Tránh copy không cần thiết
- [ ] Sử dụng '\n' thay vì endl
- [ ] Tránh string concatenation trong vòng lặp
- [ ] Xem xét custom I/O nếu n > 10^5

## 8. Lưu Ý Quan Trọng

### Khi KHÔNG nên tối ưu quá mức:
1. Contest thời gian ngắn → code đơn giản, ít bug
2. Giới hạn nhỏ (n < 10^4) → tối ưu không cần thiết
3. Logic chưa đúng → fix logic trước!

### Quy tắc vàng:
> "Premature optimization is the root of all evil" - Donald Knuth

**Nghĩa là**: Viết code đúng trước, tối ưu sau!

## 9. Kết Luận

**Cho Contest/Interview**:
- Dùng solution_optimized.cpp (đơn giản, ít bug)

**Cho Online Judge (TLE)**:
- Dùng solution_advanced.cpp (cân bằng tốc độ/độ phức tạp)

**Cho Performance Critical**:
- Dùng solution_ultra_optimized.cpp (tối đa tốc độ)
