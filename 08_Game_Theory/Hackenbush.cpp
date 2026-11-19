// Hackenbush: Grundy value of green Hackenbush on a graph (cut an edge; what loses ground falls).
// Use when: "remove an edge, parts cut off from the root disappear", tree/graph cutting games.
// Usage: Hackenbush h(n); h.set_ground(1); h.add_edge(u, v); ... h.grundy() (0 = mover loses).
// Handles: trees, cycles, multi-edges, self loops anywhere, several ground nodes, detached parts.
// Time: O(n + m)
// Indexing: 1-based nodes. set_ground every ground node BEFORE add_edge (default ground: node 1).
// Note: recursive DFS, depth up to n: big stack for long paths.

#include <bits/stdc++.h>
using namespace std;

struct Hackenbush {
    int n, timer = 0, root = -1;
    vector<vector<int>> adj;
    vector<int> tin, low;
    vector<char> is_ground;

    Hackenbush(int n) : n(n), adj(n + 1), tin(n + 1), low(n + 1), is_ground(n + 1, 0) {}

    void set_ground(int v) {
        is_ground[v] = 1;
        if (root < 0) root = v;
    }

    void add_edge(int u, int v) {
        if (is_ground[u]) u = root;
        if (is_ground[v]) v = root;
        adj[u].push_back(v);
        if (u != v) adj[v].push_back(u);
    }

    int dfs(int u, int pre) {
        tin[u] = low[u] = ++timer;
        int ans = 0;
        for (int v : adj[u]) {
            if (v == pre) {
                pre += 2 * n;
                continue;
            }
            if (tin[v] == 0) {
                int res = dfs(v, u);
                low[u] = min(low[u], low[v]);
                if (low[v] > tin[u]) ans ^= (1 + res) ^ 1;
                else ans ^= res;
            } else
                low[u] = min(low[u], tin[v]);
        }
        if (pre > n) pre -= 2 * n;
        for (int v : adj[u])
            if (v != pre && tin[u] <= tin[v]) ans ^= 1;
        return ans;
    }

    int grundy() {
        if (root < 0) set_ground(1);
        fill(tin.begin(), tin.end(), 0);
        fill(low.begin(), low.end(), 0);
        timer = 0;
        return dfs(root, 0);
    }

    bool first_player_wins() { return grundy() != 0; }
};
