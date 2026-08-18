# Matrix Exponentiation DP

## 1. Tricks & Heuristics

*   **Vector-Matrix Optimization (Multiple Queries):**
    Precompute powers to answer queries in $\mathcal{O}(Q \cdot S^2 \log N)$.
    ```cpp
    // Precompute P[k] = M^(2^k)
    for(int k = 1; k < LOG; ++k) P[k] = multiply(P[k-1], P[k-1]);
    
    // Answer query for N
    vector<long long> V = initial_state; // Size S
    for(int k = 0; k < LOG; ++k) {
        if((N >> k) & 1) {
            vector<long long> next_V(S, 0);
            for(int i = 0; i < S; ++i)
                for(int j = 0; j < S; ++j)
                    next_V[j] = (next_V[j] + V[i] * P[k][i][j]) % MOD;
            V = next_V;
        }
    }
    ```

*   **Handling Constants in Recurrences:**
    E.g., $dp[i] = 2 \cdot dp[i-1] + 3$. Add a dummy state!
    ```cpp
    // Vector: [dp[i-1], 1]
    Matrix M(2, vector<long long>(2));
    M[0][0] = 2; M[0][1] = 3; // Multiplies dp[i-1] by 2 and 1 by 3
    M[1][0] = 0; M[1][1] = 1; // Carries the constant 1 forward
    ```

*   **Paths of Exactly Length $K$:**
    Shortest path of exactly length $K$ using $(\min, +)$ semi-ring:
    ```cpp
    Matrix multiply_min_plus(Matrix A, Matrix B) {
        Matrix C(S, vector<long long>(S, INF));
        for(int i=0; i<S; ++i)
            for(int k=0; k<S; ++k)
                for(int j=0; j<S; ++j)
                    C[i][j] = min(C[i][j], A[i][k] + B[k][j]);
        return C;
    }
    ```

## 2. Common Problem Formulations

*   **Fibonacci & Linear Recurrences:**
    Standard representation for $F_N = aF_{n-1} + bF_{n-2}$:
    ```cpp
    // Vector: [F_{n-1}, F_{n-2}]
    // Matrix:
    // [ a, b ]
    // [ 1, 0 ]
    ```
    **General Rule for an $M$-order recurrence:** $F_n = c_1 F_{n-1} + c_2 F_{n-2} + \dots + c_M F_{n-M}$
    ```cpp
    // Vector: [ F_{n-1}, F_{n-2}, F_{n-3}, ..., F_{n-M} ]
    // Matrix:
    // [ c_1, c_2, c_3, ..., c_M ] // Row 0: coefficients
    // [   1,   0,   0, ...,   0 ] // Row 1: shifts F_{n-1} down
    // [   0,   1,   0, ...,   0 ] // Row 2: shifts F_{n-2} down
    // [ ...  ...  ...  ..., ... ]
    // [   0,   0, ...,   1,   0 ] // Row M-1: shifts F_{n-M+1} down
    ```

*   **Prefix Sums of Recurrences:**
    If you need the sum of the first $N$ terms: $S_N = \sum_{i=1}^N F_i$.
    Add a state $S_{i-1}$ to the vector, and transition it as $S_i = S_{i-1} + F_i$.
    ```cpp
    // Vector: [S_{i-1}, F_{i-1}, F_{i-2}]
    // Matrix:
    // [ 1, a, b ] // S_i = S_{i-1} + a*F_{i-1} + b*F_{i-2}
    // [ 0, a, b ] // F_i = a*F_{i-1} + b*F_{i-2}
    // [ 0, 1, 0 ] // F_{i-1} = F_{i-1}
    ```
    **General Rule for Prefix Sum of $M$-order recurrence:**
    ```cpp
    // Vector: [ S_{n-1}, F_{n-1}, F_{n-2}, ..., F_{n-M} ]
    // Matrix: (Size (M+1) x (M+1))
    // [   1, c_1, c_2, ..., c_M ] // Row 0: S_n = S_{n-1} + c_1 F_{n-1} + ...
    // [   0, c_1, c_2, ..., c_M ] // Row 1: F_n = c_1 F_{n-1} + ...
    // [   0,   1,   0, ...,   0 ] // Row 2: shifts F_{n-1} down
    // [   0,   0,   1, ...,   0 ] // Row 3: shifts F_{n-2} down
    // [ ...  ...  ...  ..., ... ]
    // [   0,   0, ...,   1,   0 ] // Row M: shifts F_{n-M+1} down
    ```

*   **Polynomial Constants (e.g., $+ i^2$):**
    If the recurrence involves adding a polynomial, like $dp[i] = dp[i-1] + i^2$.
    You must maintain the variables needed to compute $(i+1)^2 = i^2 + 2i + 1$.
    ```cpp
    // Vector: [ dp_{i-1}, i^2, i, 1 ]
    // Matrix:
    // [ 1, 1, 2, 1 ] // dp_i = dp_{i-1} + i^2 + 2i + 1
    // [ 0, 1, 2, 1 ] // (i+1)^2 = i^2 + 2i + 1
    // [ 0, 0, 1, 1 ] // (i+1) = i + 1
    // [ 0, 0, 0, 1 ] // 1 = 1
    ```


*   **Reachability & Boolean Matrix Multiplication:**
    If you only care whether a path of length $N$ *exists* (not the count), operations become Boolean $(\text{OR}, \text{AND})$.
    Optimize the matrix multiplication using `std::bitset` to achieve $\mathcal{O}(\frac{V^3}{64})$.
    ```cpp
    bitset<MAX_V> M[MAX_V], res[MAX_V], tmp[MAX_V];
    // To multiply A and B:
    for(int i = 0; i < V; ++i)
        for(int j = 0; j < V; ++j)
            if(A[i][j]) tmp[i] |= B[j]; 
    ```