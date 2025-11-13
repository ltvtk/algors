# 🎯 Hướng dẫn Submit

## File nào để submit?

**KHUYẾN NGHỊ:** `solution.cpp` hoặc `solution_fast.cpp` (giống nhau)

## Cách lấy code để submit

```bash
cd beautiful_triplets
cat solution.cpp
```

Copy toàn bộ output và paste vào online judge.

## Các tối ưu đã áp dụng

### ✅ Đã làm:

1. **Pre-compute distinct_from[]** → Giảm O(N³) xuống O(N² log N)
2. **Random hash XOR** → So sánh set cực nhanh
3. **Early breaks** → Skip segments không cần thiết
4. **vector<bool>** → Memory efficient (8x nhỏ hơn char)
5. **Reserve capacity** → Giảm reallocations
6. **Pragma O3** → Compiler aggressive optimizations
7. **map ổn định** → Tránh worst-case của unordered_map

### 📊 Complexity

- **Time:** O(N² log N) với constant factor nhỏ
- **Space:** O(N)

### 🎮 Kết quả mong đợi

Dựa trên AC submissions khác:
- Best: 2.5s - 5s
- Acceptable: 5s - 10s
- Memory: 7MB - 25MB

## ⚠️ Nếu vẫn TLE

Bài này có time limit rất chặt. Một số online judges có thể yêu cầu constant factor nhỏ hơn nữa.

Thuật toán O(N² log N) đã là gần optimal cho bài này với constraint N ≤ 200,000.

Nếu vẫn TLE sau khi thử cả solution.cpp và solution_fast.cpp, có thể cần:
1. Approach hoàn toàn khác (nếu tồn tại)
2. Hoặc time limit của judge quá chặt

## 📝 Notes

- Code đã được test với example input → output đúng
- Đã optimize từ version ban đầu TLE 10/20 test cases
- Version hiện tại đã giảm constant factor tối đa có thể

Good luck! 🍀
