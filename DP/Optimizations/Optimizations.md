# DP Optimizations

## 1. Convex Hull Trick (CHT) & Li-Chao Tree
**Signature:** `dp[i] = min(m_j * x_i + c_j)`
*   **The Deque Trick ($\mathcal{O}(N)$):** Use this when **BOTH** the slopes of the added lines $m_j$ are monotonic (e.g. sorted in increasing order) **AND** the query coordinates $x_i$ are monotonic. This allows you to append lines to the back of a deque and advance queries from the front of the deque in strictly $\mathcal{O}(1)$ amortized time.
    ```cpp
    struct Line { long long m, c; };
    deque<Line> dq;
    auto intersect = [](Line a, Line b) { return 1.0 * (b.c - a.c) / (a.m - b.m); };
    // Add line
    while (dq.size() >= 2 && intersect(dq.back(), line) <= intersect(dq[dq.size()-2], dq.back()))
        dq.pop_back();
    dq.push_back(line);
    // Query x
    while (dq.size() >= 2 && intersect(dq[0], dq[1]) <= x)
        dq.pop_front();
    long long ans = dq.front().m * x + dq.front().c;
    ```
*   **Binary Search CHT ($\mathcal{O}(N \log N)$):** Use this when slopes $m_j$ are monotonic, but queries $x_i$ are **random**. You can still build the convex hull using a deque/vector (only pushing to the back), but to query, you must binary search over the segments to find the optimal line for $x_i$.
*   **Li-Chao Tree ($\mathcal{O}(N \log (\text{Range}))$):** Use this when **NEITHER** slopes nor queries are monotonic. Both are completely random. 
    *   *Why Li-Chao?* You could technically use a dynamic CHT with `std::set` to maintain the hull in $\mathcal{O}(N \log N)$, but it requires over 100 lines of extremely complex iterator logic that is prone to bugs. Li-Chao Tree accomplishes the exact same thing using a simple Segment Tree structure. 
    *   *Note:* If the domain of $X$ is massive (e.g., up to $10^9$), implement it implicitly using a sparse segment tree.
    ```cpp
    struct Line { long long m, c; long long eval(long long x) { return m * x + c; } };
    Line tree[4 * MAX_X];
    void add_line(int node, int l, int r, Line line) {
        int mid = (l + r) / 2;
        bool left_better = line.eval(l) < tree[node].eval(l);
        bool mid_better = line.eval(mid) < tree[node].eval(mid);
        if (mid_better) swap(tree[node], line);
        if (l == r) return;
        if (left_better != mid_better) add_line(2*node, l, mid, line);
        else add_line(2*node+1, mid+1, r, line);
    }
    ```

## 2. Divide and Conquer Optimization
**Signature:** `dp[i][j] = min_{k < j}(dp[i-1][k] + C(k, j))`
*   **Trick (Blackbox Testing):** Check Quadrangle Inequality quickly:
    ```cpp
    bool is_qi() {
        // C(a,c) + C(b,d) <= C(a,d) + C(b,c) for a < b < c < d
        return C(1, 3) + C(2, 4) <= C(1, 4) + C(2, 3); 
    }
    ```
*   **Implementation:**
    ```cpp
    void compute(int l, int r, int optl, int optr) {
        if (l > r) return;
        int mid = (l + r) / 2;
        pair<long long, int> best = {INF, -1};
        for (int k = optl; k <= min(mid, optr); k++) {
            best = min(best, {dp_prev[k] + C(k, mid), k});
        }
        dp_cur[mid] = best.first;
        compute(l, mid - 1, optl, best.second);
        compute(mid + 1, r, best.second, optr);
    }
    ```

## 3. 1D1D DP (Monotonic Queue Optimization)
**Signature:** `dp[i] = min_{j < i}(dp[j] + C(j, i))` where $C$ satisfies Quadrangle Inequality.
*   **The Trick:** Maintain a `std::deque` of intervals sharing the same optimal transition index $j$.
    ```cpp
    struct Interval { int opt_k, l, r; };
    deque<Interval> dq;
    // Insert new candidate k=i and find the intersection point using Binary Search,
    // popping intervals from the back if the new k dominates them completely.
    ```

## 4. Knuth Optimization
**Signature:** `dp[i][j] = min_{i < k < j}(dp[i][k] + dp[k][j]) + C(i, j)`
*   **Implementation:** Iterate length $L$.
    ```cpp
    for (int i = 0; i < N; i++) { opt[i][i] = i; dp[i][i] = 0; }
    for (int L = 2; L <= N; L++) {
        for (int i = 0, j = L - 1; j < N; i++, j++) {
            dp[i][j] = INF;
            for (int k = opt[i][j-1]; k <= opt[i+1][j]; k++) {
                long long cost = dp[i][k] + dp[k+1][j] + C(i, j);
                if (cost < dp[i][j]) {
                    dp[i][j] = cost;
                    opt[i][j] = k;
                }
            }
        }
    }
    ```

## 5. Slope Trick
**Signature:** DP transitions involve adding convex piecewise linear functions.
*   **Implementation:** Maintain slope-change points in a PQ.
    ```cpp
    priority_queue<long long> pq;
    long long cost = 0;
    for (long long x : A) {
        pq.push(x); pq.push(x);
        cost += pq.top() - x;
        pq.pop(); // Shift the slope
    }
    ```

## 6. Alien's Trick (WQS Binary Search)
**Signature:** Find the minimum cost using EXACTLY $K$ operations.
*   **Implementation:** Binary search the penalty $\lambda$.
    ```cpp
    long long low = -INF, high = INF, ans = -1;
    while (low <= high) {
        long long penalty = low + (high - low) / 2;
        auto [cost, ops] = solve_unconstrained(penalty);
        if (ops >= K) { // We used too many ops, need higher penalty
            ans = cost - K * penalty; // Remove penalty contribution
            low = penalty + 1;
        } else {
            high = penalty - 1;
        }
    }
    ```
