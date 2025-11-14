# Giải Thích Chi Tiết Test Cases

## Test Input
```
5 5
3 1 5 2 4
2 5
1 4
1 3
3 5
4 5
```

## Dữ liệu
- n = 5 cây
- Chiều cao: h = [3, 1, 5, 2, 4] (indexed từ 0)
  - h[0] = 3
  - h[1] = 1
  - h[2] = 5
  - h[3] = 2
  - h[4] = 4
- q = 5 truy vấn

## Chi Tiết Từng Query

### Query 1: L=2, R=5
**Cây được giữ lại** (chiều cao trong [2, 5]):
- h[0] = 3 ✓
- h[1] = 1 ✗ (< 2)
- h[2] = 5 ✓
- h[3] = 2 ✓
- h[4] = 4 ✓

**Cây theo thứ tự**: 3, 5, 2, 4

**Tính chênh lệch**:
- |3 - 5| = 2
- |5 - 2| = 3
- |2 - 4| = 2
- **Tổng = 2 + 3 + 2 = 7** ✓

### Query 2: L=1, R=4
**Cây được giữ lại** (chiều cao trong [1, 4]):
- h[0] = 3 ✓
- h[1] = 1 ✓
- h[2] = 5 ✗ (> 4)
- h[3] = 2 ✓
- h[4] = 4 ✓

**Cây theo thứ tự**: 3, 1, 2, 4

**Tính chênh lệch**:
- |3 - 1| = 2
- |1 - 2| = 1
- |2 - 4| = 2
- **Tổng = 2 + 1 + 2 = 5** ✓

### Query 3: L=1, R=3
**Cây được giữ lại** (chiều cao trong [1, 3]):
- h[0] = 3 ✓
- h[1] = 1 ✓
- h[2] = 5 ✗ (> 3)
- h[3] = 2 ✓
- h[4] = 4 ✗ (> 3)

**Cây theo thứ tự**: 3, 1, 2

**Tính chênh lệch**:
- |3 - 1| = 2
- |1 - 2| = 1
- **Tổng = 2 + 1 = 3** ✓

### Query 4: L=3, R=5
**Cây được giữ lại** (chiều cao trong [3, 5]):
- h[0] = 3 ✓
- h[1] = 1 ✗ (< 3)
- h[2] = 5 ✓
- h[3] = 2 ✗ (< 3)
- h[4] = 4 ✓

**Cây theo thứ tự**: 3, 5, 4

**Tính chênh lệch**:
- |3 - 5| = 2
- |5 - 4| = 1
- **Tổng = 2 + 1 = 3** ✓

### Query 5: L=4, R=5
**Cây được giữ lại** (chiều cao trong [4, 5]):
- h[0] = 3 ✗ (< 4)
- h[1] = 1 ✗ (< 4)
- h[2] = 5 ✓
- h[3] = 2 ✗ (< 4)
- h[4] = 4 ✓

**Cây theo thứ tự**: 5, 4

**Tính chênh lệch**:
- |5 - 4| = 1
- **Tổng = 1** ✓

## Output
```
7
5
3
3
1
```

## Lưu Ý Quan Trọng

1. **Thứ tự cây**: Luôn giữ nguyên thứ tự ban đầu của cây (theo index), KHÔNG sắp xếp theo chiều cao
2. **Điều kiện lọc**: Giữ lại cây khi và chỉ khi L ≤ h[i] ≤ R
3. **Tính chênh lệch**: Tính |h[i] - h[j]| cho mọi cặp cây liên tiếp THEO THỨ TỰ BAN ĐẦU

## Độ Phức Tạp

- **Thời gian**: O(n) cho mỗi query → O(n*q) tổng
- **Không gian**: O(n) để lưu chiều cao cây
