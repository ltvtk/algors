def solve():
    n, q = map(int, input().split())
    a = [0] + list(map(int, input().split()))  # 1-indexed

    for _ in range(q):
        query = list(map(int, input().split()))

        if query[0] == 1:
            # Type 1: Update a[i] = v
            i, v = query[1], query[2]
            a[i] = v
        else:
            # Type 2: Find subarray [x, y] in [l, r] with sum = k
            l, r, k = query[1], query[2], query[3]

            # Compute prefix sums for range [l, r]
            prefix = [0]
            for i in range(l, r + 1):
                prefix.append(prefix[-1] + a[i])

            # Use dictionary to store prefix values and their positions
            # For each y, find if there exists x such that prefix[y] - prefix[x-1] = k
            prefix_map = {0: l - 1}  # prefix[l-1] = 0
            found = False

            for y in range(l, r + 1):
                current_sum = prefix[y - l + 1]
                target = current_sum - k

                # Check if we've seen this prefix value before
                if target in prefix_map:
                    x = prefix_map[target] + 1
                    if l <= x <= y:
                        print(x, y)
                        found = True
                        break

                # Store current position if not already stored
                if current_sum not in prefix_map:
                    prefix_map[current_sum] = y

            if not found:
                print(-1)


if __name__ == "__main__":
    solve()
