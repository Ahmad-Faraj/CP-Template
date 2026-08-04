# 🏆 The Ultimate Dynamic Programming (DP) Guide

Dynamic Programming is one of the most vital paradigms in Competitive Programming. It revolves around solving complex optimization problems by breaking them down into simpler, overlapping subproblems.

This guide provides a comprehensive overview of DP paradigms, canonical subproblems, and advanced optimizations (many of which have implementations in this folder).

---

## 🧠 Core Concepts

1. **Overlapping Subproblems**: The problem can be broken down into subproblems which are reused several times. We solve them once and store the result (Memoization).
2. **Optimal Substructure**: The optimal solution to a problem can be constructed from optimal solutions of its subproblems.
3. **State Formulation**: Defining `dp[...]`. This is the hardest part. Ask yourself: *What parameters uniquely identify the current situation so I can make a decision?*
4. **Transitions**: How does a state move to the next state? 
5. **Base Cases**: The trivial subproblems at the edge of our formulation that return a fixed value.

### Approaches:
- **Top-Down (Memoization):** Recursive approach. Easier to write, naturally avoids computing unreachable states. (Use `memset(dp, -1, sizeof dp);`).
- **Bottom-Up (Tabulation):** Iterative approach. Faster constant factor (no recursion overhead), easier to optimize space (rolling arrays).

---

## 🧩 Canonical Subproblems (The DP Dictionary)

### 1. 1D Sequence DP
* **Maximum Subarray Sum (Kadane's Algorithm):** `dp[i] = max(A[i], dp[i-1] + A[i])`
* **Longest Increasing Subsequence (LIS):** 
  * $O(N^2)$: `dp[i] = 1 + max(dp[j])` for `j < i` and `A[j] < A[i]`.
  * $O(N \log N)$ Trick: Maintain an array `tails` where `tails[i]` stores the smallest tail of all increasing subsequences of length `i+1`. Use binary search (`lower_bound`) to update it.

### 2. Knapsack & Subset DP
* **0/1 Knapsack:** Choose items with weight $W_i$ and value $V_i$ to maximize value without exceeding capacity $W$.
  `dp[i][w] = max(dp[i-1][w], dp[i-1][w-W_i] + V_i)`
* **Unbounded Knapsack / Coin Change:** Same, but items can be picked infinitely.
  `dp[w] = max(dp[w], dp[w-W_i] + V_i)`
* **Subset Sum:** Boolean DP to check if a subset sums to exactly $S$. (Can be heavily optimized using `std::bitset`).

### 3. Grid & 2D String DP
* **Grid Paths:** Finding min/max cost to travel from top-left to bottom-right.
  `dp[i][j] = cost[i][j] + max(dp[i-1][j], dp[i][j-1])`
* **Longest Common Subsequence (LCS):** 
  `dp[i][j] = A[i]==B[j] ? 1 + dp[i-1][j-1] : max(dp[i-1][j], dp[i][j-1])`
* **Edit Distance (Levenshtein):** Min operations to convert string A to B.

### 4. Range / Interval DP
* **Concept:** Solving for a subarray `[L, R]`. The length of the interval strictly increases.
* **Matrix Chain Multiplication:** 
  `dp[L][R] = min_{L \le k < R} (dp[L][k] + dp[k+1][R] + cost(L, k, R))`
* **Palindrome Partitioning:** Min cuts to split a string into palindromes.

### 5. Tree DP
* **Concept:** DP over the subtrees of a rooted tree. Post-order traversal (DFS).
* **In-Out DP (Rerooting):** First DFS computes DP inside the subtree (bottom-up). Second DFS computes DP outside the subtree (top-down), allowing $O(1)$ transitions when moving the root.
* **Vertex Cover / Independent Set:** `dp[u][0]` (u is not taken) and `dp[u][1]` (u is taken).

### 6. Bitmask DP
* **Concept:** Representing a subset of $N$ items (where $N \le 20$) as an integer bitmask.
* **Traveling Salesperson Problem (TSP):** Minimum cost to visit all nodes.
  `dp[mask][last_node] = min(dp[mask ^ (1<<last_node)][prev] + dist[prev][last_node])`
* **Matching in Bipartite Graphs:** When one side is small (e.g., $N=20$).

### 7. Digit DP
* **Concept:** Counting numbers in a range `[L, R]` that satisfy a property. Build the number digit by digit.
* **State:** `dp[idx][is_smaller][is_greater][... property conditions]`
* **Trick (Big Queries):** Only memoize states where `!is_smaller && !is_greater` (the free states). This allows reusing the DP table across multiple testcases without a costly `memset`.

### 8. Probability / Expected Value DP
* **Concept:** DP transitions are based on probabilities of future events.
* **Rule of Thumb:** Probability DP is usually computed forward (from start to end), while Expected Value DP is computed backward (from end to start, where `E[end] = 0`).

---

## 🚀 Advanced DP Optimizations (Tips & Tricks)

When a DP state transition is too slow (e.g., $O(N^2)$ transitioning to $O(N^3)$), we use these tricks to shave off a factor of $N$. Many of these are implemented in this repository.

### 1. Convex Hull Trick (CHT) / Li Chao Tree
* **When to use:** `dp[i] = min(m_j * x_i + c_j)`
* **Concept:** You are querying the minimum/maximum of a set of linear functions. CHT maintains the lower/upper envelope of these lines.
* **Complexity:** Reduces $O(N^2)$ to $O(N \log N)$ or $O(N)$ amortized.

### 2. Divide and Conquer Optimization
* **When to use:** `dp[i][j] = min_{k < j}(dp[i-1][k] + C(k, j))`
* **Condition:** The cost function $C(k, j)$ must satisfy the **Quadrangle Inequality** (wider is worse). The optimal split point $opt(i, j)$ must be monotonic.
* **Complexity:** Reduces $O(K N^2)$ to $O(K N \log N)$.

### 3. Knuth Optimization
* **When to use:** `dp[i][j] = min_{i < k < j}(dp[i][k] + dp[k][j]) + C(i, j)` (Common in Interval DP).
* **Condition:** Cost function satisfies quadrangle inequality and monotonicity.
* **Concept:** Constrain the search space of the optimal split point $K$: `opt[i][j-1] <= opt[i][j] <= opt[i+1][j]`.
* **Complexity:** Reduces $O(N^3)$ to $O(N^2)$.

### 4. Deque Trick (Sliding Window Min/Max)
* **When to use:** `dp[i] = max_{i-K \le j < i}(dp[j]) + A[i]`
* **Concept:** Use a double-ended queue (`std::deque`) to maintain a monotonic sequence of values in the current sliding window of size $K$.
* **Complexity:** Reduces $O(N \cdot K)$ to $O(N)$.

### 5. SOS DP (Sum Over Subsets)
* **When to use:** You need to compute $F[mask] = \sum_{submask \subset mask} A[submask]$.
* **Concept:** Compute the sum iteratively by turning on bits one by one.
* **Complexity:** Reduces $O(3^N)$ to $O(N 2^N)$.

### 6. Slope Trick
* **When to use:** DP transitions involve adding convex piecewise linear functions (e.g., $f(x) = |x - A[i]|$).
* **Concept:** Maintain the points where the slope changes using a `std::priority_queue`.
* **Complexity:** Reduces $O(N \cdot \text{Range})$ to $O(N \log N)$.

### 7. Matrix Exponentiation
* **When to use:** You have a linear DP recurrence (like Fibonacci) but $N$ is massive (e.g., $N = 10^{18}$).
* **Concept:** Represent the transition as a transformation matrix and compute $Matrix^N$ in $O(\log N)$ time using binary exponentiation.
* **Complexity:** Reduces $O(N)$ to $O(K^3 \log N)$ where $K$ is the number of states.

### 8. Alien's Trick (WQS Binary Search)
* **When to use:** You need to find the optimal DP value subject to choosing *exactly* $K$ items.
* **Concept:** Remove the $K$ constraint, but attach a "penalty" (or cost) $\lambda$ to picking an item. Binary search the penalty $\lambda$ until the optimal solution picks exactly $K$ items.

---

## 🛠️ General DP Hacks for CP

1. **Space Optimization (Rolling Arrays):** If `dp[i][j]` only depends on `dp[i-1][j]`, you don't need a $N \times M$ matrix. You just need a $2 \times M$ matrix. Access it via `dp[i % 2][j]`.
2. **Modulo Arithmetic:** If the answer requires `% MOD`, take the modulo at **every single addition/multiplication** step to avoid overflow. Remember that `(A - B) % MOD` in C++ can be negative, so write `(A - B % MOD + MOD) % MOD`.
3. **Initialization:** 
   - Maximization problems: initialize with `-INF` (`-1e18` or `memset(dp, -0x3f, sizeof dp)`).
   - Minimization problems: initialize with `INF` (`1e18` or `memset(dp, 0x3f, sizeof dp)`).
4. **Reconstructing the Answer:** Don't try to trace back through the DP blindly. Keep a `choice[state]` array that stores the optimal transition taken to reach that state, and follow it backward.
