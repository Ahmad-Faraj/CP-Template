# General Game Theory Heuristics (Grandmaster Level)

While Sprague-Grundy handles impartial games under normal play perfectly, many Codeforces Div. 1 C/D/E problems cannot be solved with direct SG DP due to state explosion ($10^5$ to $10^9$ states). Instead, they rely on profound mathematical observations, strategy stealing, parity invariants, or mirroring.

## 1. The Symmetry (Copycat) Strategy
Often, a game state is perfectly symmetric, or the First Player can make exactly one move to *make* it perfectly symmetric. Once symmetric, whatever move Player 2 makes, Player 1 applies the exact topological or algebraic mirror move.

- **The Circular Table (Classic):** Players place non-overlapping coins on a round table. First player places a coin perfectly in the center. The board is now rotationally symmetric. Whatever Player 2 does, Player 1 mirrors it exactly on the opposite side. **First Player Wins.**
- **Nim with Two Equal Piles:** $P_1 = P_2$. XOR sum is 0. Second player wins by copying the first player's move on the other pile.
- **Matrix / Grid Mirroring:** If the grid is $N \times M$, and players place dominoes, if $N$ or $M$ is even, you can split the board in half. If $N$ is even, the Second player mirrors the first player's move across the horizontal axis. 
- **Polygonal Cutting Game:** Players cut a polygon into smaller polygons by connecting diagonals. First player draws a diagonal that splits the polygon into two identical smaller polygons. First Player then mirrors all subsequent moves.

## 2. Strategy Stealing (Zermelo's Argument)
Used to prove the First Player MUST win, even if we don't know the exact strategy. The logic: Assume the Second Player has a winning strategy. What if the First Player makes a "harmless" random move, then adopts the Second Player's winning strategy? If having an extra move/piece can never hurt, the assumption leads to a contradiction. Therefore, the First Player must have a winning strategy.

- **Chomp (Chocolate Bar Game):** A grid of chocolate. Bottom-left is poisoned. You pick a square $(x, y)$ and eat it and everything above and to its right. 
  - *Proof:* Assume P2 wins. P1 eats ONLY the top-right square $(N, M)$. If P2 has a winning response to this, P1 could have just played that winning response on their *first* turn instead. Thus, P1 always wins for any rectangle $> 1 \times 1$.
- **Hex:** Players place colored tiles to connect opposite sides of a rhombus board. Extra tiles can never hurt your connection. Thus, P1 always wins. (Note: does not give you the algorithm to win, just the boolean answer).

## 3. Invariants & Parity Arguments
If every valid move strictly affects an invariant (like parity of empty spaces, or parity of inversions), the winner is predetermined regardless of player choices.

- **Parity of Empty Spaces:** Grid with $K$ empty cells. You must color exactly 1 cell. Game ends when no cells are left. 
  - *Winner:* Determined entirely by $N \pmod 2$.

```cpp
// O ( 1 )
bool parity_of_spaces_game(long long empty_cells) {
    return empty_cells % 2 == 1; // First player wins if odd
}
```
- **15-Puzzle Parity Invariant:** Sliding tile puzzles preserve the parity of inversions plus the Manhattan distance of the empty square. If a game asks if state A can reach state B, check if this invariant matches.
- **Euler Tour Parity:** Game played on a tree where you delete leaves. The number of moves is always equal to $V - 1$.
- **Matching Pair Trick:** Elements are paired $(A_1, B_1), (A_2, B_2)$. A move can only change elements within their pairs. If you can maintain $A_i = B_i$, then you mirror moves internally.

## 4. The Bipartite Matching Trick (Tartan Theorem)
A token is on a starting vertex $S$ of an undirected graph. Players take turns moving it to an adjacent, strictly UNVISITED vertex. The last player to move wins.
- **The Grandmaster Reduction:** Find the Maximum Bipartite Matching of the graph.
- **Winning Condition:** If the starting vertex $S$ is strictly required for **ALL** maximum matchings (i.e., removing $S$ strictly decreases the maximum matching size), then the **First Player Wins**. 
  - *Strategy:* The First Player just moves the token along the matching edge. Since $S$ is essential, the new vertex $V$ must NOT be essential, so the Second Player is forced to move to another essential vertex, allowing the First Player to use the matching edge again.
- **Losing Condition:** If there exists at least one maximum matching that does NOT use $S$, the **Second Player Wins**.

```cpp
// O ( V * E ) Bipartite Matching Trick
// Returns true if the first player wins (i.e., start_node is in ALL max matchings)
bool bipartite_game_winner(int start_node, int n, const vector<vector<int>>& adj) {
    vector<int> match(n + 1, -1);
    vector<bool> vis(n + 1, false);
    
    // Standard DFS matching
    function<bool(int)> dfs = [&](int u) {
        for (int v : adj[u]) {
            if (vis[v]) continue;
            vis[v] = true;
            if (match[v] < 0 || dfs(match[v])) {
                match[v] = u;
                match[u] = v;
                return true;
            }
        }
        return false;
    };
    
    // Find initial max matching omitting the start_node completely
    for (int i = 1; i <= n; ++i) {
        if (i != start_node && match[i] == -1) {
            fill(vis.begin(), vis.end(), false);
            vis[start_node] = true; // Prevent using start_node
            dfs(i);
        }
    }
    
    // If we can NOW find an augmenting path FOR the start_node,
    // it means it strictly increases the matching size.
    // Thus, it MUST be in ALL maximum matchings.
    fill(vis.begin(), vis.end(), false);
    return dfs(start_node); // True = First Player Wins
}
```

## 5. Potential Functions
Instead of looking at the board as independent piles (Nim), assign a "potential" to the board state. Every move strictly decreases the potential. If the potential hits 0 at the terminal state, you can determine the maximum or minimum number of moves.
- **Example:** You have a string. You can replace `ab` with `bba`. 
  - *Trick:* Assign weight $2^i$ to `a`s. Each replacement decreases the total potential, converting the game into a finite bounded state machine.

## 6. End-Game Forcing (Zugzwang)
In some games, passing is not allowed, and any move worsens your position (Zugzwang in Chess). 
- **CF Trick:** Count the number of "free moves" (moves that don't flip the state into a losing one). Treat the game as Nim where piles are the number of free moves. Once free moves are exhausted, a player is forced to jump into the "Zugzwang" state and immediately lose.

## 7. Palindrome & String Games
**Game Rules:** Players take turns adding, removing, or rearranging characters in a string. The game ends when the string becomes a palindrome (or players are trying to force/avoid it).
- **The Heuristic:** Ignore the string structure and ONLY look at the **frequencies of each character**.
- **Parity Check:** A string can only be a palindrome if at most one character has an ODD frequency. The game almost always reduces to:
  - If the number of characters with ODD frequencies is $0$ or $1$, the game is over.
  - Otherwise, each move either increases or decreases the number of ODD frequencies by exactly 1.
  - This perfectly reduces to the parity of the length of the string or the number of odd frequencies!

```cpp
// O ( N )
// Often, the winner is strictly determined by whether the initial odd count is even or odd
bool palindrome_game_winner(const string& s) {
    vector<int> count(26, 0);
    for (char c : s) count[c - 'a']++;
    int odds = 0;
    for (int c : count) if (c % 2 != 0) odds++;
    if (odds <= 1) return true; // Already a palindrome, first player might immediately win/lose depending on rules
    return (odds % 2) == 0; // Depends on exact rules, but parity strictly dictates the winner
}
```

## 8. Zero-Sum Continuous Games (Minimax / Nash Equilibrium)
**Game Rules:** Alice and Bob choose strategies $i$ and $j$ simultaneously (or probabilistically). A payoff matrix $A_{i,j}$ dictates how much Alice wins from Bob.
- **The Trick:** If it's a probabilistic (mixed strategy) game, the optimal strategy is found using **Linear Programming (Simplex Algorithm)**.
- For ICPC, if you see a $2 \times 2$ or $3 \times 3$ payoff matrix where players choose moves simultaneously, you find the probability $P$ such that Alice's expected value is identical regardless of what Bob does.
- *Expected Value = $P \times A_{0,0} + (1-P) \times A_{0,1} = P \times A_{1,0} + (1-P) \times A_{1,1}$*

## 9. Geometric Nim (Chomp on Grids/Divisors)
**Game Rules (Divisor Chomp):** A number $N$ is given. Players choose a divisor $D$ and remove it and all multiples of $D$. 
- **The Reduction:** Treat the prime factorization of $N$ as a multi-dimensional grid. Choosing a divisor is equivalent to choosing a coordinate in this grid and removing everything "above and to the right". 
- **Fact:** For any $N > 1$, the First Player **ALWAYS WINS** (by Zermelo's strategy stealing), though finding the winning move might require bruteforce DFS or DP.

## 10. The "Second Player Advantage" Trick
If a game feels completely chaotic and you cannot find a winning strategy for the First Player (and $N$ is huge), test if the Second Player can just maintain an invariant.
- **Example:** "Given an array, players can replace two elements with their sum or difference." 
- **Trick:** The Parity of the array sum might NEVER change, or might strictly flip. If the game must end in a single element, the Second Player often just ensures the final element has a specific parity. Always look at the invariants of the mathematical operation!
