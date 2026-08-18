# Hackenbush (Green / Impartial)

**Game Rules:** A graph (or tree) is drawn with some vertices connected to the "ground". Two players take turns removing exactly one edge. Any subgraph that becomes completely disconnected from the ground immediately falls away and is deleted. The last player to move wins.

- **Colon Principle:** Stalks can be fused using XOR sum.
- **Fusion Principle:** Cycles can be contracted into a single vertex without changing the Grundy value.

## Hackenbush on General Graphs
By contracting cycles using Tarjan's Bridge finding algorithm, any graph becomes a tree.

### General Green Hackenbush (Cycles)
*Evaluates Hackenbush on general cyclic graphs by contracting cycles using Tarjan's bridge-finding algorithm.*
```cpp
#include "../../core.h"

const int MAX_NODES = 3e5 + 9;
vector<int> adj[MAX_NODES];
int targan_timer, low[MAX_NODES], dis[MAX_NODES], is_ground[MAX_NODES];

// O ( V + E )
int dfs_hackenbush_graph(int u, int pre = 0, int n = 0) {
    dis[u] = low[u] = ++targan_timer;
    int ans = 0;
    for (auto v : adj[u]) {
        if (v == pre) { pre += 2 * n; continue; }
        if (dis[v] == 0) {
            int res = dfs_hackenbush_graph(v, u, n);
            low[u] = min(low[u], low[v]);
            if (low[v] > dis[u]) ans ^= (1 + res) ^ 1; // bridge
            else ans ^= res; // non bridge
        } else low[u] = min(low[u], dis[v]);
    }
    if (pre > n) pre -= 2 * n;
    for (auto v : adj[u]) if (v != pre && dis[u] <= dis[v]) ans ^= 1;
    return ans;
}
```

## Hackenbush on Simple Trees & Weighted Trees
For a simple unweighted tree, the colon principle reduces to $G(u) = \bigoplus_{v} (G(v) + 1)$.
For a **weighted tree**, where weights represent a series of edges:
- If weight $W = 1$, XOR with $G(v) + 1$.
- If weight $W$ is **even**, XOR with $G(v)$.
- If weight $W > 1$ is **odd**, XOR with $G(v) \oplus 1$.

### Weighted Hackenbush Trees
*Evaluates Hackenbush on trees where edges have weights (acting as sequential chains).*
```cpp
vector<pair<int, int>> adj_weighted[MAX_NODES];

// O ( V )
long long dfs_hackenbush_tree(int u, int p) {
    long long x = 0;
    for (auto [v, w] : adj_weighted[u]) {
        if (v != p) {
            long long child = dfs_hackenbush_tree(v, u);
            if (w == 1) x ^= (child + 1);
            else if ((w & 1LL) == 0) x ^= child; // W is even
            else x ^= (child ^ 1LL);             // W > 1 and odd
        }
    }
    return x;
}
```

## Partisan Hackenbush (Red-Blue)
Left player can only cut **Blue** edges, Right player can only cut **Red** edges. Evaluates to **Surreal Numbers**!
- **Positive Numbers:** Favor Left (Blue).
- **Negative Numbers:** Favor Right (Red).
- **Fractional Stalks:** Reading from ground up, the first edge is $\pm 1$. After the first color change, each subsequent edge contributes exactly half the value of the previous edge (e.g., $1, \frac{1}{2}, \frac{1}{4}$), moving toward the new color's sign.

### Partisan Stalk Evaluation
*Evaluates a single independent stalk of Red/Blue edges using Surreal Numbers.*
```cpp
// O ( E )
// Evaluates a single independent stalk of edges from ground to top.
// string edges contains 'B' (Blue) and 'R' (Red).
double partisan_hackenbush_stalk(const string& edges) {
    if (edges.empty()) return 0.0;
    double val = (edges[0] == 'B') ? 1.0 : -1.0;
    double current_step = val;
    for (int i = 1; i < edges.size(); ++i) {
        if (edges[i] == edges[i - 1]) {
            val += current_step; // Keep adding 1 or -1
        } else {
            current_step /= 2.0; // Color changed, start halving
            val += (edges[i] == 'B') ? abs(current_step) : -abs(current_step);
        }
    }
    return val; // Sum all stalk values. > 0 Left Wins, < 0 Right Wins, == 0 Second Player Wins.
}
```

## Tree Chomp (Subtree Removal)
**Game Rules:** A tree is given. Choosing a node removes it and its entire subtree (all descendants).
- **Formula:** It is solved recursively. The state transitions split the tree into a forest of independent trees. The Grundy value is the **MEX** of the XOR sums of the resulting independent components. In fact, this directly mirrors the Colon Principle!

### Tree Chomp (Subtree Removal)
*Evaluates tree chomp using the Colon Principle, identically to Green Hackenbush.*
```cpp
// O ( V )
// Tree Chomp evaluates identically to Hackenbush on a simple tree!
int tree_chomp_dfs(int u, int p, const vector<vector<int>>& adj) {
    int xor_sum = 0;
    for (int v : adj[u]) {
        if (v != p) xor_sum ^= (tree_chomp_dfs(v, u, adj) + 1);
    }
    return xor_sum;
}
```

### Green Hackenbush on Trees (Colon Principle)
*Reduces Green Hackenbush on a rooted tree into standard Nim by recursively XOR-summing the SG values of branches + 1.*
```cpp
int green_hackenbush_tree(int u, int p, const vector<vector<int>>& adj) {
    int sg = 0;
    for (int v : adj[u]) {
        if (v == p) continue;
        sg ^= (green_hackenbush_tree(v, u, adj) + 1);
    }
    return sg;
}
```

### Partisan Hackenbush (Red-Blue Strings / Surreal Numbers)
*Computes the exact fractional Surreal Number value of a sequential stack of Red/Blue edges (a Hackenbush stalk).*
```cpp
double partisan_hackenbush_stalk_value(const string& stalk) {
    if (stalk.empty()) return 0.0;
    double val = 0.0;
    int sign = (stalk[0] == 'B') ? 1 : -1;
    int i = 0;
    while (i < (int)stalk.size() && stalk[i] == stalk[0]) {
        val += sign;
        i++;
    }
    double frac = sign;
    while (i < (int)stalk.size()) {
        frac /= 2.0;
        if (stalk[i] == stalk[0]) val += frac;
        else val -= frac;
        i++;
    }
    return val;
}
```
