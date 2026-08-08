# Classic DP Problems 

## 1. Longest Increasing Subsequence (LIS)
*   **Trick (LCS to LIS Reduction):** 
    Turns an $\mathcal{O}(N^2)$ LCS problem into an $\mathcal{O}(N \log N)$ LIS problem when array $A$ has distinct elements.
    ```cpp
    map<int, int> pos;
    for (int i = 0; i < A.size(); ++i) pos[A[i]] = i;
    vector<int> B_mapped;
    for (int x : B) {
        if (pos.count(x)) B_mapped.push_back(pos[x]);
    }
    // Now run O(N log N) LIS on B_mapped
    ```

## 2. Knapsack
*   **Trick (Flipping the State):**
    If weights are massive but values are small: $dp[v]$ = minimum weight to achieve value $v$.
    ```cpp
    vector<long long> dp(MAX_VAL + 1, INF);
    dp[0] = 0;
    for(int i = 0; i < N; ++i) {
        for(int v = MAX_VAL; v >= val[i]; --v) {
            dp[v] = min(dp[v], dp[v - val[i]] + weight[i]);
        }
    }
    ```

*   **Bitset Knapsack:**
    Subset sum in $\mathcal{O}(\frac{N \cdot \text{Sum}}{64})$.
    ```cpp
    bitset<MAX_SUM> bs;
    bs[0] = 1;
    for(int w : weights) bs |= (bs << w);
    if(bs[target]) cout << "Possible";
    ```

*   **Bounded Knapsack (Powers of 2):**
    Transform $C$ copies of an item into $\mathcal{O}(\log C)$ items.
    ```cpp
    vector<Item> new_items;
    for(auto item : items) {
        int c = item.count;
        int k = 1;
        while(c >= k) {
            new_items.push_back({item.w * k, item.v * k});
            c -= k;
            k *= 2;
        }
        if(c > 0) new_items.push_back({item.w * c, item.v * c});
    }
    // Now run standard 0/1 knapsack on new_items
    ```

## 3. Tree DP
*   **In-Out DP (Rerooting Technique):**
    ```cpp
    void dfs_in(int u, int p) {
        for(int v : adj[u]) {
            if(v == p) continue;
            dfs_in(v, u);
            dp_in[u] = combine(dp_in[u], dp_in[v]); // e.g., max depth
        }
    }
    void dfs_out(int u, int p, long long out_val) {
        dp_out[u] = out_val;
        // Precompute prefix/suffix arrays of children's dp_in values here
        // to pass the correct 'out_val' down to each child efficiently
        for(int i=0; i<adj[u].size(); ++i) {
            int v = adj[u][i];
            if(v == p) continue;
            long long next_out = combine(out_val, combine(pref[i-1], suff[i+1]));
            dfs_out(v, u, next_out);
        }
    }
    ```

## 4. Range / Interval DP
*   **Bracket Sequences:**
    ```cpp
    for (int L = 2; L <= N; ++L) {
        for (int i = 0, j = L - 1; j < N; ++i, ++j) {
            dp[i][j] = INF;
            if (is_match(S[i], S[j])) dp[i][j] = dp[i+1][j-1]; // Outer match
            for (int k = i; k < j; ++k) {
                dp[i][j] = min(dp[i][j], dp[i][k] + dp[k+1][j]); // Split
            }
        }
    }
    ```
*   **Zuma Game (Block Removal):**
    Add a third parameter $k$ for identical elements to the right.
    ```cpp
    // dp[l][r][k] = min cost to clear [l, r] given k identical elements to the right of r
    long long solve(int l, int r, int k) {
        if (l > r) return 0;
        if (dp[l][r][k] != -1) return dp[l][r][k];
        
        long long res = solve(l, r - 1, 0) + cost(k + 1); // Remove A[r] along with the k elements
        for (int i = l; i < r; ++i) {
            if (A[i] == A[r]) {
                // Merge inside
                res = min(res, solve(l, i, k + 1) + solve(i + 1, r - 1, 0));
            }
        }
        return dp[l][r][k] = res;
    }
    ```

## 5. Probability and Expected Value DP
*   **Handling Cycles (Gaussian Elimination):**
    Forms matrix $A$ where $A * x = B$.
    ```cpp
    void gauss(vector<vector<double>>& A, vector<double>& B) {
        int n = A.size();
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                double factor = A[j][i] / A[i][i];
                for (int k = i; k < n; ++k) A[j][k] -= factor * A[i][k];
                B[j] -= factor * B[i];
            }
        }
        vector<double> x(n);
        for (int i = n - 1; i >= 0; --i) {
            x[i] = B[i];
            for (int j = i + 1; j < n; ++j) x[i] -= A[i][j] * x[j];
            x[i] /= A[i][i];
        }
    }
    ```
