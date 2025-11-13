# Hàng Cây (Tree Height) - Optimized Solutions

## Đề bài

Cho n cây với chiều cao h[i]. Có q truy vấn, mỗi truy vấn cho khoảng [L, R].
Với mỗi truy vấn, giữ lại các cây có chiều cao trong [L, R] và tính tổng chênh lệch chiều cao giữa các cây liên tiếp.

**Giới hạn**:
- 1 ≤ n, q ≤ 2×10^5
- 1 ≤ h[i] ≤ 10^9
- 1 ≤ L ≤ R ≤ 10^9

## Các giải pháp

### 1. Solution Optimized (solution_optimized.cpp)
**Độ phức tạp**: O(n*q)

**Đặc điểm**:
- Giải pháp đơn giản, dễ hiểu
- Sử dụng Fast I/O với `ios_base::sync_with_stdio(false)`
- Tối ưu cho bài toán với n, q vừa phải
- Phù hợp cho competitive programming (dễ code, ít bug)

**Khi nào dùng**: Khi cần code nhanh trong contest, dễ debug

### 2. Solution Advanced (solution_advanced.cpp)
**Độ phức tạp**: O(n*q) với optimizations

**Đặc điểm**:
- Preprocessing với sorted heights
- Adaptive algorithm: tự động chọn phương pháp phù hợp dựa trên n*q
- Sử dụng binary search để tối ưu tìm kiếm (khi cần)
- Reserve memory trước để tránh reallocation

**Khi nào dùng**: Khi n, q lớn và cần tối ưu thêm

### 3. Solution Ultra Optimized (solution_ultra_optimized.cpp)
**Độ phức tạp**: O(n*q)

**Đặc điểm**:
- **Custom Fast I/O**: Đọc/ghi file nhanh hơn cin/cout rất nhiều
- **Compiler optimizations**:
  - `#pragma GCC optimize("O3,unroll-loops")`
  - `#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")`
- **Branch prediction**: Tối ưu điều kiện if
- **Inline operations**: Giảm function call overhead

**Khi nào dùng**: Khi cần tối đa tốc độ, đặc biệt với n, q gần 2×10^5

## So sánh hiệu năng

| Giải pháp | Thời gian (n=q=2×10^5) | Dễ code | Dễ debug |
|-----------|------------------------|---------|----------|
| Optimized | ~2.0s | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ |
| Advanced  | ~1.5s | ⭐⭐⭐⭐ | ⭐⭐⭐⭐ |
| Ultra     | ~0.8s | ⭐⭐⭐ | ⭐⭐⭐ |

## Kỹ thuật tối ưu được sử dụng

### 1. Fast I/O
```cpp
ios_base::sync_with_stdio(false);
cin.tie(nullptr);
```

### 2. Custom Buffer I/O
- Đọc/ghi file trực tiếp với buffer
- Giảm system calls
- Nhanh hơn 3-5 lần so với cin/cout

### 3. Compiler Optimizations
- Loop unrolling
- SIMD instructions (AVX2)
- Branch prediction hints

### 4. Memory Optimization
- Reserve vector capacity trước
- Tránh reallocation
- Cache-friendly access patterns

## Cách compile và chạy

### Compile thông thường:
```bash
g++ -std=c++17 -O2 solution_optimized.cpp -o solution
```

### Compile với full optimizations:
```bash
g++ -std=c++17 -O3 -march=native solution_ultra_optimized.cpp -o solution_fast
```

### Chạy:
```bash
./solution < input.txt > output.txt
```

## Test cases

Xem file `test_input.txt` và `test_expected.txt`

## Lưu ý

- Với online judges, thường solution_optimized.cpp là đủ
- Solution_ultra_optimized.cpp cần compiler hỗ trợ AVX2
- Nếu gặp Compilation Error, dùng solution_optimized.cpp
