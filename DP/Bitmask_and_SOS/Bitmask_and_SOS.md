# Bitmask DP & Sum Over Subsets (SOS) 

## 1. Submask Enumeration
**Signature:** Iterate over all masks, and for each, iterate over all valid submasks in $\mathcal{O}(3^N)$.
*   **Code:**
    ```cpp
    for (int mask = 0; mask < (1 << N); ++mask) {
        for (int sub = mask; sub > 0; sub = (sub - 1) & mask) {
            // Process submask
        }
    }
    ```

## 2. Sum Over Subsets (SOS DP)
**Signature:** Compute $F[mask] = \sum_{submask \subset mask} A[submask]$ in $\mathcal{O}(N 2^N)$.
*   **Code:**
    ```cpp
    for(int i = 0; i < N; ++i) {
        for(int mask = 0; mask < (1 << N); ++mask) {
            if(mask & (1 << i)) {
                dp[mask] += dp[mask ^ (1 << i)];
            }
        }
    }
    ```
*   **Superset Sum:**
    ```cpp
    for(int i = 0; i < N; ++i) {
        for(int mask = (1 << N) - 1; mask >= 0; --mask) {
            if(!(mask & (1 << i))) { // Invert the bit check
                dp[mask] += dp[mask ^ (1 << i)];
            }
        }
    }
    ```

## 3. Fast Walsh-Hadamard Transform (FWT)
**Signature:** Compute bitwise convolution $C[k] = \sum_{i \oplus j = k} A[i] \times B[j]$.
*   **Code (XOR Convolution):**
    ```cpp
    void fwt_xor(vector<long long>& a, bool inv) {
        int n = a.size();
        for (int len = 1; 2 * len <= n; len <<= 1) {
            for (int i = 0; i < n; i += 2 * len) {
                for (int j = 0; j < len; j++) {
                    long long u = a[i + j];
                    long long v = a[i + len + j];
                    a[i + j] = u + v;
                    a[i + len + j] = u - v;
                }
            }
        }
        if (inv) for (long long& x : a) x /= n;
    }
    // Usage: fwt_xor(A, false); fwt_xor(B, false); 
    // for(int i=0; i<N; ++i) C[i] = A[i]*B[i]; 
    // fwt_xor(C, true);
    ```

## 4. Subset Convolution
**Signature:** OR convolution with disjoint subsets: $C[k] = \sum_{i | j = k, i \ \& \ j = 0} A[i] \times B[j]$.
*   **Code Structure:**
    ```cpp
    long long A[N+1][1<<N], B[N+1][1<<N], C[N+1][1<<N];
    // Populate A and B based on __builtin_popcount
    for(int i=0; i<=N; ++i) {
        sos_dp(A[i]); sos_dp(B[i]);
    }
    for(int mask=0; mask<(1<<N); ++mask) {
        for(int i=0; i<=N; ++i) {
            for(int j=0; j<=i; ++j) {
                C[i][mask] += A[j][mask] * B[i-j][mask];
            }
        }
    }
    for(int i=0; i<=N; ++i) inverse_sos_dp(C[i]);
    // Answer for mask is C[__builtin_popcount(mask)][mask]
    ```

## 5. Profile DP (Broken Profile)
**Signature:** Tiling a grid $N \times M$ where $M \le 20$.
*   **Code Structure:**
    ```cpp
    for(int r = 0; r < N; ++r) {
        for(int c = 0; c < M; ++c) {
            for(int mask = 0; mask < (1 << M); ++mask) {
                // Shift mask by 1 bit for the next cell state
                int next_mask = (mask << 1) & ((1 << M) - 1);
                // Transitions based on whether current cell is filled
            }
        }
    }
    ```
