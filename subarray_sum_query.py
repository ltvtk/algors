import sys

def main():
    input_data = sys.stdin.read().strip().split()
    idx = 0

    n = int(input_data[idx])
    q = int(input_data[idx + 1])
    idx += 2

    a = [0] + [int(input_data[idx + i]) for i in range(n)]
    idx += n

    # Dictionary-based hash with timestamping (simulate array-based hash)
    pos = {}
    last_query = {}
    current_query = 0

    output = []

    for _ in range(q):
        if idx >= len(input_data):
            break
        query_type = int(input_data[idx])
        idx += 1

        if query_type == 1:
            i = int(input_data[idx])
            v = int(input_data[idx + 1])
            idx += 2
            a[i] = v
        else:
            l = int(input_data[idx])
            r = int(input_data[idx + 1])
            k = int(input_data[idx + 2])
            idx += 3

            current_query += 1
            OFFSET = 100005

            # Initialize: prefix sum 0 at position l-1
            offset_key = OFFSET
            last_query[offset_key] = current_query
            pos[offset_key] = l - 1

            prefix_sum = 0
            found = False

            for i in range(l, r + 1):
                prefix_sum += a[i]

                # Look for target prefix sum
                target = prefix_sum - k + OFFSET
                if target in last_query and last_query[target] == current_query:
                    output.append(f"{pos[target] + 1} {i}")
                    found = True
                    break

                # Store current prefix sum (only first occurrence)
                curr_key = prefix_sum + OFFSET
                if curr_key not in last_query or last_query[curr_key] != current_query:
                    last_query[curr_key] = current_query
                    pos[curr_key] = i

            if not found:
                output.append("-1")

    print('\n'.join(output))


if __name__ == "__main__":
    main()
