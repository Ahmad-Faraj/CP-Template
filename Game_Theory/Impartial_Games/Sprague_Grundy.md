# 1. Fundamentals & Sprague-Grundy Theorem (Grandmaster Level)

## Impartial vs Partisan Games
- **Impartial Games:** The set of valid moves from any given position is exactly the same for both players. (CP almost exclusively deals with impartial games).
- **Partisan Games:** The valid moves depend on whose turn it is (e.g., Chess, Hackenbush with colors). Sprague-Grundy DOES NOT APPLY. You must use standard Minimax DP, Alpha-Beta pruning, or Ad-hoc logic.

## Normal vs Misère Play
- **Normal Play:** The last player to make a valid move **wins**. Sprague-Grundy handles this perfectly.
- **Misère Play:** The last player to make a valid move **loses**. Sprague-Grundy **BREAKS DOWN** for sums of games under Misère play! (Except for standard Nim where Bouton's Theorem works). In CP, Misère games on graphs or sums of games are usually avoided unless they are purely DP/Ad-hoc or reduce directly to standard Misère Nim.

## Sprague-Grundy Theorem
Every impartial game under normal play is equivalent to a Nim pile of a certain size (Grundy value).
- **MEX (Minimum Excluded Value):** $G(S) = \text{MEX}( \{ G(S_1), G(S_2), \dots, G(S_k) \} )$
- **Combining Games:** $G_{\text{total}} = G(\text{game}_1) \oplus G(\text{game}_2) \oplus \dots \oplus G(\text{game}_n)$

```cpp
#include "../../core.h"

// O ( N )
int nim_value(const vector<int>& piles) {
    int x = 0;
    for (int p : piles) x ^= p;
    return x;
}

// O ( N )
int get_mex(const unordered_set<int>& S) {
    int m = 0;
    while (S.count(m)) ++m;
    return m;
}

// O ( states * moves )
int compute_grundy(int h, const vector<int>& moves, vector<int>& dp) {
    if (dp[h] != -1) return dp[h];
    unordered_set<int> S;
    for (int mv : moves) if (h >= mv) S.insert(compute_grundy(h - mv, moves, dp));
    return dp[h] = get_mex(S);
}
```

## Advanced SG Optimizations for CP
1. **MEX Optimization:** If calculating MEX over many intervals or dynamically changing states, use a **Trie** or a **Segment Tree**. If you want $\text{MEX}(S \oplus X)$ where you XOR all values in a set by $X$, a Binary Trie can find the MEX in $O(\log(\text{MAX\_VAL}))$ time!
2. **Periodic Grundy Values:** If a game has states defined by an integer $N$ and moves involve subtracting bounded constants (e.g., subtract $x \in \{a, b, c\}$), the Grundy values $G(N)$ **will eventually become periodic**! (Due to finite states bounding the MEX). Find the cycle using Brent's or Floyd's algorithm, and answer $O(1)$ queries for $N=10^{18}$.

## When to ignore Sprague-Grundy
- **Loops/Draws (Loopy Games):** Sprague-Grundy assumes games are finite and must end. If players can move in circles forever (causing a Draw), you cannot use standard MEX. You must use the Remoteness/Suspense graph algorithms.
- **Interacting Subgames:** SG theorem ONLY works if the subgames are completely independent. If a move in Game 1 restricts or forces a move in Game 2, SG fails.

## Common Grundy Value Patterns (Cheat Sheet)
Instead of running $O(N^2)$ DP for MEX, many standard game rules produce predictable, $O(1)$ mathematical patterns for $G(N)$. Always print the first 100 values of your DP to look for these patterns!

### 1. Subtraction Rules
- **Take any number of stones (Standard Nim):** $G(N) = N$
- **Take $1$ to $K$ stones (Bachet's Game):** $G(N) = N \pmod{K + 1}$
- **Take any ODD number of stones:** $G(N) = N \pmod 2$
- **Take any EVEN number of stones:** $G(N) = 0$ if $N$ is odd, $G(N) = N / 2$ if $N$ is even.
- **Take a Power of $K$ stones ($1, K, K^2, \dots$):**
  - If $K$ is **odd**: $G(N) = N \pmod 2$
  - If $K$ is **even**: $G(N) = N \pmod{K + 1}$

```cpp
// O ( 1 )
int bachets_game_grundy(int n, int k) { return n % (k + 1); }
int even_stones_grundy(int n) { return (n % 2 == 1) ? 0 : (n / 2); }
int power_k_stones_grundy(int n, int k) { return (k % 2 == 1) ? (n % 2) : (n % (k + 1)); }
```

### 2. Division & Factor Rules
- **Divide $N$ by any of its divisors $D > 1$ (replace $N$ with $N/D$):**
  $G(N) = \text{Total number of prime factors of } N \text{ (with multiplicity)}$. 
  *(Example: $N = 12 = 2^2 \times 3^1 \implies G(12) = 2 + 1 = 3$. Dividing by $D$ is just removing prime factors, making it exactly Standard Nim).*
- **Take at most half the stones ($1 \le X \le \lfloor N/2 \rfloor$):**
  $G(N) = \lfloor \log_2 N \rfloor$. (The position of the most significant bit).

```cpp
// O ( sqrt(N) )
int division_game_grundy(long long n) {
    int factors = 0;
    for (long long i = 2; i * i <= n; i++) {
        while (n % i == 0) { factors++; n /= i; }
    }
    if (n > 1) factors++;
    return factors;
}

// O ( 1 )
int half_stones_grundy(long long n) { return 63 - __builtin_clzll(n); }
```

### 3. Splitting Rules (Lasker's Nim)
- **Take any number of stones OR split a pile into two non-empty piles:**
  - $N \equiv 1 \text{ or } 2 \pmod 4 \implies G(N) = N$
  - $N \equiv 3 \pmod 4 \implies G(N) = N + 1$
  - $N \equiv 0 \pmod 4 \implies G(N) = N - 1$

```cpp
// O ( 1 )
int laskers_nim_grundy(int n) {
    if (n % 4 == 1 || n % 4 == 2) return n;
    if (n % 4 == 3) return n + 1;
    return n - 1;
}
```

## SG on Trees (Coin Sliding Games)
You have a tree with coins on some vertices. A move consists of sliding a coin toward the root.
- **Trick:** The game is exactly equivalent to Nim where each coin at depth $D$ acts as a pile of size $D$. XOR the depths of all coins.
- **Edge cases:** What if the capacity of nodes is limited? What if you slide away from root? Often reduces to Bipartite matching or parity arguments.
