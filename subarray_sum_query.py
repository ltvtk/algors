import sys

def solve():
    n, q = map(int, sys.stdin.readline().split())
    a = [0] + list(map(int, sys.stdin.readline().split()))

    for _ in range(q):
        line = sys.stdin.readline().strip()
        if not line:
            continue
        query = list(map(int, line.split()))

        if query[0] == 1:
            i, v = query[1], query[2]
            a[i] = v
        else:
            l, r, k = query[1], query[2], query[3]
            length = r - l + 1

            # Early checks for impossible cases
            # 1. |k| cannot exceed length
            # 2. k and length must have same parity
            if abs(k) > length or (k & 1) != (length & 1):
                print(-1)
                continue

            # For small ranges, brute force is faster
            if length <= 100:
                found = False
                for i in range(l, r + 1):
                    total = 0
                    for j in range(i, r + 1):
                        total += a[j]
                        if total == k:
                            print(i, j)
                            found = True
                            break
                    if found:
                        break
                if not found:
                    print(-1)
                continue

            # For larger ranges, use hash map
            first_pos = {0: l - 1}
            prefix = 0
            found = False

            for i in range(l, r + 1):
                prefix += a[i]
                target = prefix - k

                if target in first_pos:
                    print(first_pos[target] + 1, i)
                    found = True
                    break

                if prefix not in first_pos:
                    first_pos[prefix] = i

            if not found:
                print(-1)


if __name__ == "__main__":
    solve()
