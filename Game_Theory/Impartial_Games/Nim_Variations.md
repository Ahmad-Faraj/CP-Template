# Nim and its Variations

Nim is the atomic unit of combinatorial game theory. A massive portion of CP Game Theory problems are simply disguised versions of Nim.

## Standard Nim & Misère Nim
**Game Rules:** $N$ piles of stones. A player can remove any positive number of stones from exactly one pile.
- **Normal Play:** A position is losing if the XOR sum $S = P_1 \oplus P_2 \dots = 0$.
  - **Winning Move ($O(1)$):** If $S \ne 0$, find any pile $P_i$ where $P_i \oplus S < P_i$. This condition is true **if and only if** the Most Significant Bit (MSB) of $S$ is also set (is `1`) in $P_i$. Reduce that pile to $P_i \oplus S$.
- **Misère Play (Bouton's Theorem):** Play exactly like Normal Nim, UNLESS the move leaves ONLY piles of size 1 or 0.
  - **The Edge Case ($O(1)$):** Count the number of piles with size $> 1$. If this count is `0`, the first player wins if the number of piles of size $1$ is **EVEN** (forcing the opponent to take the last one). Otherwise, play normally ($S \ne 0$).

### Misère Nim Winner
*Evaluates the winner of a Misère Nim game in $O(N)$ time.*
```cpp
// O ( N )
bool misere_nim_winner(const vector<int>& piles) {
    int xor_sum = 0, count_gt_1 = 0;
    for (int p : piles) {
        xor_sum ^= p;
        if (p > 1) count_gt_1++;
    }
    // Edge case: all piles are size 1 (or 0)
    if (count_gt_1 == 0) return (piles.size() % 2) == 0; 
    // Normal case
    return xor_sum != 0;
}
```

### Nim-Sum Properties ($X \oplus X = 0$)
- If the XOR sum $X \neq 0$, there is *always* at least one pile $P_i$ such that $P_i \oplus X < P_i$.
- **Finding Winning Moves:** The number of different winning first moves in standard Nim is exactly the count of piles where $P_i \oplus X < P_i$.
- If you can add a pile to a Nim game, and you want the First Player to lose, you must add a pile of size exactly equal to the current XOR sum $X$ (because $X \oplus X = 0$).

## Staircase Nim (Extremely Common in CP)
**Game Rules:** There are stones on steps $1$ to $N$ of a staircase. A move consists of moving $K > 0$ stones from step $i$ to step $i-1$. Stones moved from step $1$ fall into step $0$ and are removed from the game.
- **Trick:** Only look at the stones on the **ODD numbered steps**.
- **Proof:** Treat stones on odd steps as regular Nim piles. Moving stones from Odd to Even is like removing them from a Nim pile. Moving stones from Even to Odd increases an Odd pile. However, if Player 1 moves stones from Even to Odd, Player 2 can just immediately push those exact same stones from the new Odd step down to the next Even step! The Even steps act as "garbage/buffer" spaces that don't affect the true game state.
- **Formula:** XOR sum of all pile sizes on indices $1, 3, 5, \dots$

### Staircase Nim
*Solves the game where stones are moved down steps, returning the winner by XORing only the odd-indexed steps.*
```cpp
// O ( N )
int staircase_nim(const vector<int>& steps) {
    int xor_sum = 0;
    // 1-indexed steps, we only XOR the odd steps
    for (int i = 1; i < steps.size(); i += 2) xor_sum ^= steps[i];
    return xor_sum ? 1 : 2; // 1 means First Player wins
}
```

## Nim with a Pass
**Game Rules:** Standard Nim, but each player is allowed to "pass" (do nothing) exactly ONCE per game.
- **Trick:** If a player is in a losing position in standard Nim, they will use their pass. Thus, the winning condition flips in complex ways.
- **Formula:** Actually, the First Player **always wins** as long as there is at least one stone! If the initial XOR sum $\neq 0$, they make a normal winning move. If the initial XOR sum $== 0$, they just use their PASS! Now it's the Second Player's turn with XOR sum $== 0$.

### Nim with a Pass
*Determines the winner of Nim when each player is allowed exactly one "pass" per game.*
```cpp
// O ( N )
bool nim_with_pass(const vector<int>& piles) {
    for (int p : piles) if (p > 0) return true; // First player always wins if game isn't already over
    return false;
}
```

## Moore's Nim_k
**Game Rules:** Remove from **at most $K$** piles simultaneously. (Standard Nim is $Nim_1$).
- **Rule:** Write the sizes of all piles in binary. For each bit position $j$, sum the $j$-th bits across all piles. If the sum at *every* bit position is a multiple of $(K+1)$, the position is **losing**. Otherwise, it's winning.

### Moore's Nim_k
*Evaluates a Nim state where a player can remove stones from at most $K$ piles simultaneously.*
```cpp
// O ( N log(Max_Pile) )
bool moores_nim_k(const vector<int>& piles, int k) {
    vector<int> bit_counts(32, 0);
    for (int p : piles) {
        for (int i = 0; i < 32; ++i) if ((p >> i) & 1) bit_counts[i]++;
    }
    for (int count : bit_counts) if (count % (k + 1) != 0) return true; // Winning state
    return false; // Losing state
}
```

## Fibonacci Nim
**Game Rules:** A single pile of $N$ stones. The first player can take $1$ to $N-1$ stones. In subsequent turns, a player can take up to **$2 \times$ the amount the previous player took**.
- **$O(1)$ Win Condition:** The first player loses if and only if $N$ is a **Fibonacci number**.
- **Winning Move (Zeckendorf's Theorem):** If $N$ is not Fibonacci, uniquely decompose $N$ into a sum of non-consecutive Fibonacci numbers. The winning move is to remove exactly the **smallest Fibonacci number** in this Zeckendorf representation.

### Fibonacci Nim
*Determines the optimal Zeckendorf decomposition winning move for a game restricted by Fibonacci sequences.*
```cpp
// O ( log(N) )
bool is_fibonacci(long long n) {
    long long a = 1, b = 2;
    while (b < n) { long long temp = b; b = a + b; a = temp; }
    return b == n || a == n;
}

// O ( log(N) )
long long fibonacci_nim_winning_move(long long n) {
    if (is_fibonacci(n)) return -1; // Losing state, no winning move
    
    // Zeckendorf decomposition to find the smallest Fibonacci number
    long long a = 1, b = 2;
    vector<long long> fib = {1, 2};
    while (a + b <= n) { fib.push_back(a + b); a = b; b = fib.back(); }
    
    long long smallest_fib = 0;
    for (int i = fib.size() - 1; i >= 0; --i) {
        if (n >= fib[i]) {
            n -= fib[i];
            smallest_fib = fib[i];
        }
    }
    return smallest_fib;
}
```

## Turning Turtles (Positional Nim)
**Game Rules:** A line of coins (Heads and Tails). You can pick a Head, flip it to Tail, and *optionally* flip any one coin to its left.
- **Logic:** Equivalent to playing Nim where every Head at 0-indexed position `i` acts as a Nim pile of size `i`. Flipping a Head at $i$ to Tail is reducing the pile. Flipping a coin at $j < i$ is changing another pile. Because Nim allows reducing one pile and we only care about XOR sum parity, this perfectly simulates Nim!

### Turning Turtles (Positional Nim)
*Reduces a 1D coin-flipping game (flip one Head, optionally flip one leftward coin) directly into Nim.*
```cpp
// O ( N )
int turning_turtles_game(const string &s) {
    int xor_sum = 0;
    for(int i = 1; i <= s.size(); ++i) if (s[i - 1] == 'H') xor_sum ^= (i - 1);
    return xor_sum ? 1 : 2; // 1 means first player wins, 2 means second wins
}
```

## Silver Dollar Game (Coin Gap Nim)
**Game Rules:** A semi-infinite tape of squares. Some squares have coins. You can move any coin leftwards, without jumping over other coins. No square can have >1 coin.
- **Logic:** Pairing the coins from the right side. The number of empty spaces between pairs acts exactly like Nim piles. Moving a right coin in a pair reduces the gap. Moving a left coin increases the gap, but the opponent can just push the right coin to restore the gap (exactly like Staircase Nim's even piles).

### Silver Dollar Game (Coin Gap Nim)
*Reduces a sliding coin game on a 1D tape into Nim by mapping empty spaces between paired coins to piles.*
```cpp
// O ( N )
int silver_dollar_game(vector<int>& locations) {
    if (locations.size() & 1) locations.insert(locations.begin(), 0);
    int xor_sum = 0;
    for (int i = 0; i < locations.size(); i += 2) xor_sum ^= (locations[i + 1] - locations[i] - 1);
    return xor_sum ? 1 : 2;
}
```

## White Knight / Black Knight Game
**Game Rules:** Two knights on a chessboard. White moves, then Black. They cannot land on each other. They move towards the bottom-left corner.
- **Trick:** Calculate the Manhattan-like distance in knight moves between them. The game often reduces to Nim where the distance is the pile size, or Nim with a pass. Look for parity invariants on the chess board colors!

### White Knight / Black Knight Game
*Determines the winner of a knight chase game by mapping board colors to Manhattan parity distance.*
```cpp
// O ( 1 )
bool knight_chase_game(int wx, int wy, int bx, int by) {
    // A simplified chase game usually depends strictly on the parity of the Manhattan distance
    int manhattan_dist = abs(wx - bx) + abs(wy - by);
    return manhattan_dist % 2 != 0; // First player wins if starting distance is odd
}
```

## Nim with Limited Takes (Arbitrary Subtraction Game)
If subtraction rules are arbitrary, we can use Bitsets to find cycles rapidly for massive constraints.

### Bitset Optimization for Nim Cycles
*Uses bitsets to rapidly find periods and pre-periods for bounded arbitrary subtraction games.*
```cpp
#include "../../core.h"

// Nim with limited takes (Arbitrary Subtraction Game)
const int MAX_SUBTRACTION = 1005;
using NimState = bitset<MAX_SUBTRACTION>;

// O ( |arr| )
NimState next_nim_state(const NimState& mask, const vector<int>& arr) {
    NimState new_mask = mask << 1;
    for (const auto& m : arr) {
        if (m > MAX_SUBTRACTION) continue;
        if (mask[m - 1] == 0) { new_mask[0] = 1; break; }
    }
    return new_mask;
}

// O ( pre_period + cycle_length * |arr| )
pair<int, int> find_nim_cycle(const vector<int>& arr) {
    NimState slow = 0, fast = 0;
    do {
        slow = next_nim_state(slow, arr);
        fast = next_nim_state(next_nim_state(fast, arr), arr);
    } while (fast != slow);
    int before = 0, cycle = 1;
    slow = 0;
    while (slow != fast) {
        before++;
        slow = next_nim_state(slow, arr);
        fast = next_nim_state(fast, arr);
    }
    fast = next_nim_state(fast, arr);
    while (slow != fast) {
        fast = next_nim_state(fast, arr);
        cycle++;
    }
    return {before, cycle};
}
```

## Non-Decreasing Nim (Sorted Piles)
**Game Rules:** A Nim game where the piles must ALWAYS remain sorted in non-decreasing order: $A_1 \le A_2 \le \dots \le A_n$. A move consists of decreasing $A_i$, as long as $A_{i-1} \le A_i$ is maintained.
- Transform the array into a difference array! Let $D_i = A_i - A_{i-1}$.
- When you decrease $A_i$ by $X$, you:
  - Decrease $D_i$ by $X$.
  - Increase $D_{i+1}$ by $X$.
- **Conclusion:** This is EXACTLY equivalent to **Staircase Nim** played on the difference array $D_i$! You move $X$ stones from step $i$ to step $i+1$. 

### Non-Decreasing Nim
*Determines the winner in Nim where piles must remain sorted, transforming it into Staircase Nim.*
```cpp
// O ( N )
bool non_decreasing_nim_winner(const vector<int>& sorted_piles) {
    int xor_sum = 0;
    int n = sorted_piles.size();
    // Calculate differences from the END of the array
    for (int i = n - 1; i >= 0; i -= 2) {
        int diff = sorted_piles[i] - (i > 0 ? sorted_piles[i - 1] : 0);
        xor_sum ^= diff;
    }
    return xor_sum != 0;
}
```

## Nim-Multiplication (2D Coin Turning Games)
**Game Rules:** A 2D grid of coins (heads/tails). A valid move consists of choosing the bottom-right corner of a rectangle $(x, y)$ that is currently Heads, flipping it to Tails, and optionally flipping any of the other 3 corners of the rectangle $(0, 0), (x, 0), (0, y)$. 
- **The Trick:** This 2D game decomposes into the **Nim-Multiplication** of the 1D games! 
- **Formula:** $G(x, y) = G(x) \otimes G(y)$, where $\otimes$ is the Nim-Multiplication operator.
- **Nim-Multiplication Rules:** 
  - $2^{2^n} \otimes 2^{2^n} = 2^{2^n} + 2^{2^n - 1}$ (Fermat powers wrap around).
  - Nim-multiplication distributes over Nim-addition (XOR): $A \otimes (B \oplus C) = (A \otimes B) \oplus (A \otimes C)$.

### Nim-Multiplication (2D Matrix Nim)
*Computes the exact Nim-Multiplication of two values $O(\log^2 N)$, used for evaluating rectangular 2D coin games.*
```cpp
// O ( log^2 N )
// Computes Nim-Multiplication of a and b
long long nim_mult(long long a, long long b) {
    if (a < 2 || b < 2) return a * b;
    long long half = 1;
    while (half <= a || half <= b) half <<= 1;
    half >>= 1;
    long long a1 = a / half, a2 = a % half;
    long long b1 = b / half, b2 = b % half;
    long long p1 = nim_mult(a1, b1);
    long long p2 = nim_mult(a2, b2);
    long long p3 = nim_mult(a1 ^ a2, b1 ^ b2);
    return (p1 ^ p2) * half ^ p2 ^ nim_mult(p1, half / 2);
}
```

### Treblecross / Dawson's Chess (Octal Games)
*Calculates the SG sequence for 1D placement games where placing an 'X' essentially blocks adjacent cells.*
```cpp
vector<int> generate_treblecross_grundy(int max_n) {
    vector<int> G(max_n + 1, 0);
    for (int n = 1; n <= max_n; n++) {
        unordered_set<int> mex_set;
        for (int i = 0; i <= n - 3; i++) mex_set.insert(G[i] ^ G[n - i - 3]);
        int m = 0;
        while (mex_set.count(m)) m++;
        G[n] = m;
    }
    return G;
}
```

### Bounded Nim (Different limits per pile)
*Evaluates Nim when each individual pile $P_i$ has a unique maximum subtraction limit $K_i$.*
```cpp
bool bounded_nim_winner(const vector<int>& piles, const vector<int>& limits) {
    int xor_sum = 0;
    for (int i = 0; i < (int)piles.size(); i++) {
        xor_sum ^= (piles[i] % (limits[i] + 1));
    }
    return xor_sum != 0;
}
```
