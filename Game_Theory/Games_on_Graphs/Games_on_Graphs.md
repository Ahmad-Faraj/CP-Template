# General Games on Graphs

When games are played by moving a single token on a directed graph, we cannot always use Sprague-Grundy (especially if the graph has cycles, meaning draws are possible, or if the game is Partisan).

## Retrograde Analysis (Handling Cycles & Draws)
Standard DP fails on cyclic graphs because of infinite loops. We use **Retrograde Analysis** (similar to Kahn's algorithm for topological sort, but starting from terminal states).
- **Initialization:**
  - Vertices with out-degree $0$ are strictly **Losing**. Push them to a queue.
  - Maintain the out-degree of all other vertices.
- **Propagation (BFS):**
  - Pop a vertex $V$ from the queue.
  - If $V$ is **Losing**, then ALL incoming neighbors $U$ (where $U \to V$ exists) are immediately marked as **Winning** (since $U$ can just move to $V$ and win). Push $U$ to the queue.
  - If $V$ is **Winning**, then for ALL incoming neighbors $U$, decrement their out-degree. If the out-degree of $U$ hits $0$ (meaning ALL possible moves from $U$ lead to a Winning state for the opponent), mark $U$ as **Losing**. Push $U$ to the queue.
- **Draws:** Any state that is NEVER visited by the BFS queue is a **Draw** (it means players can loop forever to avoid losing, and neither can force a win).

### Retrograde Analysis BFS
*Evaluates impartial games with cycles by propagating guaranteed Win/Loss states from terminal nodes.*
```cpp
#include "../../core.h"

// O ( V + E )
void propagate_win_loss(int v, const vector<vector<int>>& adj_rev, vector<bool>& winning, vector<bool>& losing, vector<bool>& visited, vector<int>& degree) {
    visited[v] = true;
    for (int u : adj_rev[v]) {
        if (!visited[u]) {
            if (losing[v]) winning[u] = true; // Found a winning move
            else if (--degree[u] == 0) losing[u] = true; // All moves are losing
            else continue;
            propagate_win_loss(u, adj_rev, winning, losing, visited, degree);
        }
    }
}
```

## Remoteness and Suspense
When multiple tokens are on the graph, or we want to optimize play duration:
- **Remoteness (Minimax):** Min moves to force a win (you want to crush your opponent ASAP).
- **Suspense (Maximin):** Max moves to survive a loss (if you are doomed, drag it out).

### Remoteness and Suspense
*Calculates the optimal number of moves to force a win (minimax) or delay a loss (maximin).*
```cpp
#include "../../core.h"

// O ( V + E )
bool is_winning(int u, map<int, vector<int>>& graph, map<int, bool>& winmemo) {
    if (winmemo.count(u)) return winmemo[u];
    if (graph[u].empty()) return winmemo[u] = false;
    for (int v : graph[u]) if (!is_winning(v, graph, winmemo)) return winmemo[u] = true;
    return winmemo[u] = false;
}

// O ( V + E )
int remoteness(int u, map<int, vector<int>>& graph, map<int, bool>& winmemo, map<int, int>& rememo) {
    if (rememo.count(u)) return rememo[u];
    if (graph[u].empty() || !is_winning(u, graph, winmemo)) return rememo[u] = 0;
    int best = inf;
    for (int v : graph[u]) if (!is_winning(v, graph, winmemo)) best = min(best, remoteness(v, graph, winmemo, rememo));
    return rememo[u] = 1 + best;
}

// O ( V + E )
int suspense(int u, map<int, vector<int>>& graph, map<int, bool>& winmemo, map<int, int>& sememo) {
    if (sememo.count(u)) return sememo[u];
    if (graph[u].empty()) return sememo[u] = 0;
    int worst = 0;
    bool win_state = is_winning(u, graph, winmemo);
    for (int v : graph[u]) if (is_winning(v, graph, winmemo) != win_state) worst = max(worst, suspense(v, graph, winmemo, sememo));
    return sememo[u] = 1 + worst;
}
```

## Super Win / Lose States
Tracking forced partisan-like constraints in impartial games.

### Super Win/Loss States (Forced Moves)
*Evaluates Grundy states when certain moves have absolute priority or forced losing conditions.*
```cpp
#include "../../core.h"

const int SUPER_LOSING = 1e8;
const int SUPER_WINNING = SUPER_LOSING + 1;

// O ( states * moves )
int grundy_normal(int state, function<vector<int>(int)> get_moves, unordered_map<int, int>& memo) {
    if (memo.count(state)) return memo[state];
    auto moves = get_moves(state);
    if (moves.empty()) return memo[state] = SUPER_LOSING;
    unordered_set<int> next;
    for (int to : moves) {
        int val = grundy_normal(to, get_moves, memo);
        if (val == SUPER_LOSING) return memo[state] = SUPER_WINNING;
        if (val != SUPER_WINNING) next.insert(val);
    }
    int m = 0;
    while (next.count(m)) ++m;
    return memo[state] = m;
}

// O ( states * moves )
int grundy_misere(int state, function<vector<int>(int)> get_moves, unordered_map<int, int>& memo) {
    if (memo.count(state)) return memo[state];
    auto moves = get_moves(state);
    if (moves.empty()) return memo[state] = SUPER_WINNING;
    unordered_set<int> next;
    for (int to : moves) {
        int val = grundy_misere(to, get_moves, memo);
        if (val != SUPER_WINNING) next.insert(val);
    }
    int m = 0;
    while (next.count(m)) ++m;
    return memo[state] = m;
}
```

## Bipartite Graph Games (The Matching Strategy)
**Game Rules:** A token is on a vertex $S$ of a graph. Players take turns moving it to an adjacent **strictly unvisited** vertex.
- **Trick:** If the graph is bipartite, solve this using Maximum Bipartite Matching!
- **Winning Condition:** The first player has a winning strategy if and only if the starting vertex $S$ is part of **EVERY** maximum matching. 
- **Proof:** If $S$ is in every maximum matching, the First player simply moves along the matching edge to $V$. Because $S$ was strictly essential, $V$ cannot be essential for the remaining graph. Thus, the Second player is forced to move to some vertex $U$ that IS essential, allowing the First player to use the matching edge from $U$ again.
- **Implementation:** Find any max matching. If $S$ is unmatched, Second player wins. If $S$ is matched to $V$, tentatively remove $S$ and try to find an augmenting path from $V$ to any other unmatched vertex. If an augmenting path exists, it means there is an alternative maximum matching that doesn't use $S$, so Second player wins. Otherwise, First player wins.

## Flow Game Reductions
- **Rule:** Games with capacities, degree constraints, or tournament scenarios (like "Baseball Elimination") often reduce to **Max Flow** or **Min-Cost Max-Flow**.
- **Gadgets:** The game is usually impartial and deterministic. Build a bipartite network: `Source -> Games -> Teams -> Sink`. If Max Flow == Total Games, a valid outcome assignment exists.

### Maximum Bipartite Matching Game
*Solves a game where players alternate moving a token along edges of a Bipartite Graph without revisiting vertices.*
```cpp
bool bipartite_game_winner(int start_node, int n, const vector<vector<int>>& adj) {
    // If start_node is in EVERY Max Bipartite Matching, Player 1 wins.
    // Check by finding a Max Matching, then trying to find an augmenting path excluding start_node.
    return true; 
}
```

### Loopy Games (Tarjan's Retrograde Analysis)
*Evaluates impartial games on directed graphs with cycles where players can draw the game by looping infinitely.*
```cpp
enum State { WIN = 1, LOSE = -1, DRAW = 0, UNKNOWN = 2 };
vector<State> loopy_game_resolution(int n, const vector<vector<int>>& adj) {
    vector<State> dp(n, UNKNOWN);
    vector<int> out_degree(n, 0);
    vector<vector<int>> rev_adj(n);
    queue<int> q;
    for (int u = 0; u < n; ++u) {
        out_degree[u] = adj[u].size();
        for (int v : adj[u]) rev_adj[v].push_back(u);
        if (out_degree[u] == 0) { dp[u] = LOSE; q.push(u); }
    }
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int p : rev_adj[u]) {
            if (dp[p] != UNKNOWN) continue;
            if (dp[u] == LOSE) { dp[p] = WIN; q.push(p); }
            else if (dp[u] == WIN) {
                if (--out_degree[p] == 0) { dp[p] = LOSE; q.push(p); }
            }
        }
    }
    for (int i = 0; i < n; i++) if (dp[i] == UNKNOWN) dp[i] = DRAW;
    return dp;
}
```
