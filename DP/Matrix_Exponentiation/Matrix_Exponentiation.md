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

*   **Berlekamp-Massey Algorithm (The Black Magic):**
    If you know the DP is a linear recurrence but constructing the matrix is impossibly complicated (e.g., weird combinatorial rules):
    ```cpp
    // BM returns the shortest linear recurrence coefficients for sequence S
    vector<long long> berlekamp_massey(const vector<long long>& s) {
        // ... Standard O(N^2) BM implementation ...
        // Returns {c_1, c_2, ..., c_k} such that X_n = sum(c_i * X_{n-i})
    }
    ```

*   **Kitamasa Method:**
    An alternative to explicit matrix exponentiation for evaluating $K$-order linear recurrences directly using polynomial modular arithmetic.
    ```cpp
    // Combines polynomials A and B modulo the characteristic polynomial of the recurrence
    vector<long long> multiply_poly(vector<long long>& A, vector<long long>& B, vector<long long>& C_poly) {
        // ... O(K^2) or O(K log K) polynomial multiplication ...
    }
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

*   **Dynamic DP (DDP) / Matrix Chain on Trees:**
    Formulate transitions as matrices. Use HLD + Segment Tree.
    ```cpp
    // Segment tree holds transition matrices for heavy paths
    void update(int node, int l, int r, int target, Matrix M) {
        if(l == r) { tree[node] = M; return; }
        // ... standard seg tree update
        tree[node] = multiply(tree[2*node], tree[2*node+1]); 
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

*   **String Automaton (DFA) DP:**
    "Count strings of length $N$ not containing substring $P$."
    Build a DFA (e.g., using KMP's failure function) for the pattern $P$.
    The state vector is the number of strings currently ending at DFA node $u$.
    The transition matrix $M[u][v]$ is the number of characters that transition the DFA from node $u$ to node $v$. 
    Answer is the sum of the resulting vector after multiplying by $M^N$.

*   **Grid Tiling (Profile DP + Matrix Exp):**
    "Ways to tile a $2 	imes N$ or $3 	imes N$ grid with dominos, where $N \le 10^{18}$."
    The state is a bitmask of the column profile (size $2^M$).
    Construct a transition matrix of size $2^M 	imes 2^M$ where $M[u][v] = 1$ if profile $u$ can legally transition to profile $v$.
    Exponentiate the matrix by $N$.

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

*   **Circulant Matrices (State depends on offset):**
    If the transition from state $u$ to state $v$ only depends on $(u - v) \pmod M$, the matrix is a Circulant Matrix. 
    You do NOT need $\mathcal{O}(M^3)$ matrix multiplication! You only need to maintain the first row of the matrix. The product of two circulant matrices can be computed in $\mathcal{O}(M^2)$ by just convoluting their first rows, or $\mathcal{O}(M \log M)$ using FFT.

*   **Markov Chains & Probabilities:**
    "Find the probability of being in state $v$ after $N$ steps."
    The matrix $M$ is the transition probability matrix (stochastic matrix where each row/col sums to 1). The answer is simply $M^N \times \text{Initial Probabilities}$.
    *Absorbing States:* If the game ends when reaching state $v$, set $M[v][v] = 1$ and all other transitions out of $v$ to $0$. 

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

*   **Counting Closed Walks (Trace):**
    "Find the number of cycles / closed walks of exactly length $K$."
    The number of closed walks of length $K$ is precisely the trace of the adjacency matrix raised to the $K$-th power: $\text{Trace}(A^K) = \sum_{i} (A^K)_{i,i}$.
