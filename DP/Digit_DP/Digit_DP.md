# Digit DP 

## 1. The Core State
`dp[idx][is_smaller][is_greater][... properties]`

## 2. Tricks & Heuristics

*   **The "Free State" Memoization (Crucial for $T$ Queries):**
    Only memoize when choices are completely unbounded by $L$ and $R$.
    ```cpp
    long long solve(int idx, bool is_smaller, bool is_greater, ...) {
        if (idx == N) return valid_property();
        
        bool is_free = is_smaller && is_greater;
        if (is_free && dp[idx][...] != -1) return dp[idx][...];
        
        long long res = 0;
        int limit_up = is_smaller ? 9 : R[idx] - '0';
        int limit_down = is_greater ? 0 : L[idx] - '0';
        
        for (int d = limit_down; d <= limit_up; ++d) {
            res += solve(idx + 1, is_smaller || (d < limit_up), is_greater || (d > limit_down), ...);
        }
        
        if (is_free) dp[idx][...] = res;
        return res;
    }
    ```

*   **Handling Leading Zeros:**
    Pass `is_leading_zero` into the state.
    ```cpp
    for (int d = limit_down; d <= limit_up; ++d) {
        bool next_lz = is_leading_zero && (d == 0);
        // Only count '0' towards your property if next_lz is false!
    }
    ```

*   **The Subtraction Trick:**
    Instead of dealing with `is_greater`, compute $Solve(R) - Solve(L-1)$.
    ```cpp
    // Simple state: dp[idx][is_smaller][...]
    long long ans = solve(R) - solve(subtract_one_string(L));
    ```

*   **Divisibility State:**
    ```cpp
    int next_mod = (current_mod * 10 + d) % M;
    ```

*   **Automaton / DFA State Transition:**
    If the condition is "does not contain substring '13'", use KMP DFA.
    ```cpp
    // dfa[node][digit] tells us which DFA state we move to if we append 'digit'
    int next_dfa_node = dfa[current_dfa_node][d];
    if (next_dfa_node == BAD_NODE) continue; // Prune invalid paths
    ```

*   **Palindromic Digit DP:**
    You must build the number from the outside in (leftmost and rightmost digits simultaneously). 
    ```cpp
    // State tracks whether the mirrored inner digits have broken the bounds yet
    int next_status = current_status;
    if (d > R[N - 1 - idx] - '0') next_status = GREATER;
    else if (d < R[N - 1 - idx] - '0') next_status = SMALLER;
    ```

*   **Base-K Digit DP:**
    Works exactly the same, but the limit is $K-1$ instead of 9.
    ```cpp
    int K = 2; // e.g., Binary
    int limit_up = is_smaller ? (K - 1) : (R[idx] - '0'); // Assuming R is in base K
    ```
