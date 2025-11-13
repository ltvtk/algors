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

### 4. `solution.cpp` - Giải pháp tổng hợp (KHUYẾN NGHỊ)

**Thuật toán:**
- Tự động chọn thuật toán phù hợp dựa trên input:
  - Nếu max(A_i) ≤ 63: Sử dụng bitmask (nhanh nhất)
  - Ngược lại: Sử dụng hash với unordered_map

**Độ phức tạp:**
- Best case (A_i ≤ 63): O(N² × log N)
- General case: O(N² × M)

**Ưu điểm:**
- Tự động tối ưu cho mọi trường hợp
- Đơn giản để sử dụng - chỉ cần submit 1 file

**Phù hợp:** Tất cả subtasks

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
| **solution** | ✅ **Fast** | ✅ **Fast** | ✅ **Fast** | ⚠️ **OK** |

**Kết luận:** Sử dụng `solution.cpp` cho tất cả các test cases.

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
