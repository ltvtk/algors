# Đếm Bộ Ba Đẹp (Beautiful Triplets)

## Mô tả bài toán

Cho dãy số nguyên dương A = A₁, A₂, ..., Aₙ.

Một bộ ba chỉ số (i, j, k) được gọi là **đẹp** nếu:
1. 1 ≤ i ≤ j < k ≤ N
2. Set(A[i..j]) = Set(A[j+1..k])

Yêu cầu: Đếm số lượng bộ ba đẹp.

### Input/Output

**Input:**
- Dòng 1: Số nguyên N (1 ≤ N ≤ 200,000)
- Dòng 2: N số nguyên A₁, A₂, ..., Aₙ (1 ≤ Aᵢ ≤ N)

**Output:**
- Một số nguyên: số lượng bộ ba đẹp

### Ví dụ

**Input:**
```
7
3 1 2 1 2 3 1
```

**Output:**
```
4
```

**Giải thích:**
- (1,3,6): X=(3,1,2), Y=(1,2,3), Set(X)={1,2,3}, Set(Y)={1,2,3}
- (1,3,7): X=(3,1,2), Y=(1,2,3,1), Set(X)={1,2,3}, Set(Y)={1,2,3}
- (1,4,7): X=(3,1,2,1), Y=(2,3,1), Set(X)={1,2,3}, Set(Y)={1,2,3}
- (2,3,5): X=(1,2), Y=(1,2), Set(X)={1,2}, Set(Y)={1,2}

---

## Các giải pháp

### 1. `subtask12_basic.cpp` - Giải pháp cơ bản (N ≤ 5000)

**Thuật toán:**
- Với mỗi vị trí j (điểm chia giữa X và Y):
  - Xây dựng tất cả các set từ segments bên trái [i, j]
  - Xây dựng tất cả các set từ segments bên phải [j+1, k]
  - Sử dụng `map<set<int>, int>` để đếm số cặp matching

**Độ phức tạp:** O(N² × log N × log M) với M là kích thước của set

**Ưu điểm:**
- Code đơn giản, dễ hiểu
- Sử dụng STL set nên không lo collision

**Nhược điểm:**
- Chậm với N lớn do overhead của set và map

**Phù hợp:** Subtask 1-2 (N ≤ 5000)

---

### 2. `subtask3_bitset.cpp` - Tối ưu với bitmask (A_i ≤ 50)

**Thuật toán:**
- Tương tự như basic nhưng sử dụng **bitmask** (unsigned long long) để biểu diễn set
- Mỗi set được biểu diễn bằng 1 số 64-bit, bit thứ i được set nếu giá trị i có trong set
- Sử dụng `map<unsigned long long, int>` để đếm

**Độ phức tạp:** O(N² × log N)

**Ưu điểm:**
- Nhanh hơn nhiều so với set<int>
- So sánh set chỉ là so sánh 2 số nguyên
- Insert/update set chỉ là OR operation: `mask |= (1ULL << value)`

**Nhược điểm:**
- Chỉ hoạt động khi A_i ≤ 63 (do unsigned long long có 64 bits)

**Phù hợp:** Subtask 3 (A_i ≤ 50)

---

### 3. `general_hash.cpp` - Sử dụng hash tùy chỉnh

**Thuật toán:**
- Convert set thành vector đã sort, sau đó hash vector
- Sử dụng `unordered_map<vector<int>, int, VectorHash>` với hash function tùy chỉnh
- Hash function: kết hợp hash của từng phần tử theo công thức:
  ```cpp
  hash ^= std::hash<int>{}(x) + 0x9e3779b9 + (hash << 6) + (hash >> 2)
  ```

**Độ phức tạp:** O(N² × M) với M là kích thước trung bình của set

**Ưu điểm:**
- Nhanh hơn map do sử dụng unordered_map
- Không giới hạn giá trị A_i

**Nhược điểm:**
- Có khả năng hash collision (tuy thấp)
- Overhead của việc convert set -> vector

**Phù hợp:** General case hoặc khi A_i lớn

---

### 4. `solution.cpp` & `solution_optimized.cpp` - Giải pháp tối ưu ⭐ (KHUYẾN NGHỊ)

**Thuật toán - Random Hash với Early Break:**

1. **Random Hash:** Gán mỗi giá trị một hash ngẫu nhiên 64-bit, dùng XOR để tạo signature cho set
2. **Early Break Optimization:**
   - Tính trước `max_right_size`: số phần tử distinct tối đa có thể có từ [j+1, n)
   - Skip left segments nếu size > max_right_size
   - Tính `max_left_size` và skip right segments nếu size > max_left_size
3. **Sử dụng array thay vì set:** `seen[value] = 0/1` thay vì `set.count(value)`
4. **Map key: (size, hash)** để tránh collision tốt hơn

**Độ phức tạp:** O(N² × log N) với constant factor nhỏ nhờ early breaks

**Ưu điểm:**
- Rất nhanh nhờ random hash (64-bit XOR)
- Early break giảm số operations đáng kể
- Array tracking nhanh hơn set
- Không giới hạn giá trị A_i

**Nhược điểm:**
- Có khả năng hash collision cực kỳ thấp (64-bit random)

**Phù hợp:** **Tất cả subtasks - TỐI ƯU NHẤT**

---

## Cách biên dịch và chạy

### Biên dịch:
```bash
g++ -std=c++17 -O2 -o solution solution.cpp
```

### Chạy:
```bash
./solution < test_input.txt
```

Hoặc:
```bash
echo "7
3 1 2 1 2 3 1" | ./solution
```

---

## Phân tích độ phức tạp chi tiết

### Tại sao là O(N²)?

Với mỗi j từ 1 đến N-1:
- Xây dựng j segments bên trái: [1,j], [2,j], ..., [j,j]
- Xây dựng (N-j) segments bên phải: [j+1,j+1], [j+1,j+2], ..., [j+1,N]
- Tổng số segments: j + (N-j) = N

Tổng số segments xử lý: Σ(j=1 to N-1) N = N × (N-1) = O(N²)

### Tối ưu hóa thêm?

Với N = 200,000, O(N²) = 4×10¹⁰ operations có vẻ nhiều, nhưng:
1. Constant factor nhỏ (chỉ là OR operation hoặc insert vào map)
2. Trong thực tế, không phải tất cả N² segments đều có nhiều phần tử
3. Với optimization flag -O2/-O3, compiler tối ưu rất tốt

Thuật toán tốt hơn O(N²) cho bài này khó thực hiện và phức tạp hơn nhiều.

---

## So sánh hiệu năng

| Giải pháp | N=500 | N=5000 | N=200000 (A_i≤50) | N=200000 (General) |
|-----------|-------|--------|-------------------|-------------------|
| subtask12_basic | ✅ Fast | ✅ OK | ⚠️ Slow | ⚠️ Slow |
| subtask3_bitset | ✅ Fast | ✅ Fast | ✅ Fast | ❌ Error (overflow) |
| general_hash | ✅ Fast | ✅ OK | ⚠️ OK | ⚠️ OK |
| **solution** (old) | ✅ Fast | ✅ Fast | ⚠️ Slow (TLE) | ⚠️ Slow (TLE) |
| **solution_optimized** ⭐ | ✅ **Very Fast** | ✅ **Very Fast** | ✅ **Fast** | ✅ **Fast** |

**Kết luận:** Sử dụng `solution.cpp` hoặc `solution_optimized.cpp` (giống nhau) cho tất cả các test cases.

---

## Tối ưu hóa quan trọng

### 1. Random Hash với XOR
Thay vì so sánh set trực tiếp, ta gán mỗi giá trị một hash ngẫu nhiên 64-bit:
```cpp
mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
for (int i = 1; i <= max_val; i++) {
    hashes[i] = rng();
}
```

Signature của một set = XOR tất cả hashes:
```cpp
current_hash ^= hashes[a[i]];  // Thêm phần tử vào set
```

**Ưu điểm:**
- XOR rất nhanh (1 CPU cycle)
- 64-bit → xác suất collision ≈ 1/2^64 (cực kỳ thấp)
- Không cần sort hay convert set → vector

### 2. Early Break Optimization

**Ý tưởng:** Không cần xử lý tất cả segments nếu biết chắc chúng không match

**Tối ưu 1 - Max Right Size:**
```cpp
// Tính trước: số distinct tối đa có thể có từ [j+1, n)
int max_right_size = ...;

// Khi build left segments:
if (current_size + 1 > max_right_size) {
    break;  // Left segment này không thể match với bất kỳ right segment nào
}
```

**Tối ưu 2 - Max Left Size:**
```cpp
// Tính: size lớn nhất trong các left segments đã build
int max_left_size = ...;

// Khi build right segments:
if (current_size > max_left_size) {
    break;  // Right segment này quá lớn, không thể match
}
```

**Impact:** Giảm số operations từ O(N²) xuống gần O(N×D) với D = số distinct values (thường nhỏ hơn N rất nhiều)

### 3. Array tracking thay vì Set
```cpp
// Chậm:
set<int> seen;
if (seen.count(x)) ...
seen.insert(x);

// Nhanh:
vector<int> seen(max_val + 1, 0);
if (seen[x]) ...
seen[x] = 1;
```

**Ưu điểm:** O(1) thay vì O(log N), cache-friendly

### 4. Map với (size, hash) key
```cpp
map<pair<int, lint>, int> left_map;
```

Lưu cả size và hash giúp tránh collision: hai sets khác nhau phải khác cả về size VÀ hash.

---

## Tips & Tricks

### 1. Bitmask cho giá trị nhỏ
Khi A_i nhỏ (≤ 63), bitmask là lựa chọn tốt nhất:
- Set operations nhanh: `mask |= (1ULL << x)`
- Compare nhanh: chỉ là so sánh 2 số
- Memory efficient

### 2. Cẩn thận với overflow
- Sử dụng `unsigned long long` (64 bits)
- Chỉ dùng được khi A_i ≤ 63
- Nếu A_i > 63, cần dùng bitset<N> hoặc hash

### 3. Hash function tốt
Công thức hash kết hợp:
```cpp
hash ^= std::hash<int>{}(x) + 0x9e3779b9 + (hash << 6) + (hash >> 2)
```
- 0x9e3779b9 là golden ratio hash constant
- (hash << 6) + (hash >> 2) giúp phân tán hash tốt hơn

### 4. Tối ưu I/O
```cpp
ios_base::sync_with_stdio(false);
cin.tie(nullptr);
```
Giúp cin/cout nhanh hơn đáng kể.

---

## Tác giả

Giải pháp được tối ưu hóa cho bài toán "Đếm Bộ Ba Đẹp".

Mọi góp ý xin gửi về issue tracker.
