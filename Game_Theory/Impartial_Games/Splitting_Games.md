# Splitting Games & Octal Games

Splitting Games occur when a single move takes one component (pile/row) and splits it into two or more independent components. The Grundy value of the new state is the XOR sum of the Grundy values of the resulting components.

## Octal Games Notation
A mathematical classification for games played on heaps of beans where players can remove beans and optionally split the heap.
- An octal game is defined by a sequence of digits $0-7$: $d_0.d_1 d_2 d_3 \dots$
- **$d_k$:** Represents the rules for removing exactly $k$ beans from a heap.
- Convert $d_k$ to binary: $b_0, b_1, b_2$ (where $b_i$ is the $i$-th bit).
  - $b_0 = 1$: You can remove $k$ beans and leave **0** heaps (i.e., remove the entire heap of size $k$).
  - $b_1 = 1$: You can remove $k$ beans and leave exactly **1** heap (i.e., just shrink the heap).
  - $b_2 = 1$: You can remove $k$ beans and leave exactly **2** non-empty heaps (i.e., split the remainder into two heaps).

## Dawson's Chess & Dawson's Kayles (Octal .137)
- **Game Rules (Dawson's Chess):** Played on a $1 \times N$ chessboard with pawns. Capturing a pawn removes it and the adjacent squares become unplayable (due to pawn capture geometry).
- **Game Rules (Dawson's Kayles):** A row of $N$ bowling pins. You can knock down exactly 1 pin, and the adjacent pins also fall.
- **Grundy Transition:** A move of size 3 (removing the center and its two neighbors) splits the remaining $N-3$ space into two independent segments of size $i$ and $N - 3 - i$.
- $G(N) = \text{MEX}( \{ G(i) \oplus G(N - i - 3) \} )$ for $0 \le i \le N-3$.
- **Periodicity:** Dawson's Chess has an exact period length of **34**, which begins after a pre-period of 51.

### Dawson's Chess
*Evaluates the SG sequence for Dawson's Chess by splitting the remaining space.*
```cpp
// O ( N^2 ) DP for Dawson's Chess
int dawsons_chess(int n, vector<int>& memo) {
    if (n <= 0) return 0;
    if (memo[n] != -1) return memo[n];
    unordered_set<int> s;
    for (int i = 0; i <= n - 3; ++i) s.insert(dawsons_chess(i, memo) ^ dawsons_chess(n - i - 3, memo));
    int m = 0;
    while (s.count(m)) ++m;
    return memo[n] = m;
}
```

## Classic Kayles (Octal .77)
- **Game Rules:** A row of $N$ bowling pins. You can knock down 1 pin, or 2 adjacent pins. Knocking down pins splits the row into two smaller rows.
- $G(N) = \text{MEX}( \{ G(i) \oplus G(N-i-1) \} \cup \{ G(j) \oplus G(N-j-2) \} )$
- **Periodicity:** Exact period length of **12** (starting after a pre-period of length 70).

### Classic Kayles
*Evaluates the SG sequence for Kayles where 1 or 2 pins can be knocked down.*
```cpp
// O ( N^2 ) DP for Classic Kayles
int kayles(int n, vector<int>& memo) {
    if (n <= 0) return 0;
    if (memo[n] != -1) return memo[n];
    unordered_set<int> s;
    for (int i = 0; i <= n - 1; ++i) s.insert(kayles(i, memo) ^ kayles(n - i - 1, memo)); // knock 1
    for (int i = 0; i <= n - 2; ++i) s.insert(kayles(i, memo) ^ kayles(n - i - 2, memo)); // knock 2
    int m = 0;
    while (s.count(m)) ++m;
    return memo[n] = m;
}
```

## Grundy's Game
- **Game Rules:** A player takes a single pile of stones and MUST split it into exactly two **UNEQUAL** non-empty piles.
- $G(N) = \text{MEX}( \{ G(i) \oplus G(N - i) \} )$ for all $1 \le i < N/2$.
- **Periodicity:** Unlike Kayles, Grundy's game is currently NOT known to be periodic! However, for CP constraints, you usually only need to compute it up to $N = 10^5$, which can be done in $O(N^2)$ or optimized using Trie/Fast Walsh-Hadamard Transform tricks for extreme constraints.

### Grundy's Game
*Evaluates the SG sequence for Grundy's Game where a pile must be split into strictly unequal non-empty piles.*
```cpp
// O ( N^2 ) DP for Grundy's Game
int grundys_game(int n, vector<int>& memo) {
    if (n <= 2) return 0; // cannot split 1 or 2 into unequal non-empty piles
    if (memo[n] != -1) return memo[n];
    unordered_set<int> s;
    for (int i = 1; i < n / 2.0; ++i) { // strictly unequal
        s.insert(grundys_game(i, memo) ^ grundys_game(n - i, memo));
    }
    int m = 0;
    while (s.count(m)) ++m;
    return memo[n] = m;
}
```

## Periodicity in Splitting & Subtraction Games
In games where a pile of size $N$ splits into smaller piles, or decreases by specific sets of numbers (like primes, or powers of 2):
- The sequence of Grundy values $G(0), G(1), G(2), \dots$ will **almost always** eventually become periodic (except for pathological games like Grundy's Game).
- If $N$ is massive (e.g., $10^{18}$), you only need to compute $G(i)$ using DP up to a few thousand (e.g., $N=2000$). Then, write a loop to find the cycle length (period) and the pre-period.
- The answer for $N$ is then just $G(\text{pre\_period} + (N - \text{pre\_period}) \pmod{\text{period}})$.
- **Tip for CP:** If you see a game with $N = 10^{18}$ and states split, immediately write a brute force DP up to $N=500$, print the Grundy values, and look for repeating patterns of numbers!


### Standard Pile Splitting (Any Two Piles)
*Calculates the Grundy value when a pile of size $N$ can be split into any two strictly smaller, non-empty piles $A$ and $B$ where $A + B = N$.*
```cpp
int basic_splitting_grundy(int n, vector<int>& dp) {
    if (dp[n] != -1) return dp[n];
    unordered_set<int> mex_set;
    for (int i = 1; i <= n / 2; i++) {
        int a = i, b = n - i;
        if (a != b) mex_set.insert(basic_splitting_grundy(a, dp) ^ basic_splitting_grundy(b, dp));
    }
    int m = 0;
    while (mex_set.count(m)) m++;
    return dp[n] = m;
}
```
