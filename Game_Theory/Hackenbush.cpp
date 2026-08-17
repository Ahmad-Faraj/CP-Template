// Hackenbush: the Grundy value of green Hackenbush - cut an edge, and anything no longer joined to ground falls off.
// Use when: "cut edges off a graph rooted at the ground", tree-cutting games, Sprague-Grundy over a rooted graph.
// Handles: trees, cycles, multi-edges, self loops at ANY node, several ground nodes, and pieces already detached.
// Time: O(n + m)
// Indexing: 1-based nodes; mark every ground node with set_ground before calling grundy()
// Note: two principles - a tree XORs its child subtrees, and any cycle contracts to a point without changing the value.

#include <bits/stdc++.h>
using namespace std;

struct Hackenbush {
    int n, timer = 0, root = 1;
    vector<vector<int>> adj;
    vector<int> tin, low;
    vector<char> is_ground;

    Hackenbush(int n) : n(n), adj(n + 1), tin(n + 1, 0), low(n + 1, 0), is_ground(n + 1, 0) {}

    void set_ground(int v) { // every ground node is really the same node
        is_ground[v] = 1;
        if (root == 1 || v < root) root = v;
    }

    // A self loop is kept in the adjacency rather than counted separately, so that a loop
    // hanging off a deep node is folded into THAT node's value and then carried up through
    // the bridge above it. Counting loops globally is wrong for any node but the ground.
    void add_edge(int u, int v) {
        if (is_ground[u]) u = root;
        if (is_ground[v]) v = root;
        adj[u].push_back(v);
        if (u != v) adj[v].push_back(u);
    }

    int dfs(int u, int parent) {
        tin[u] = low[u] = ++timer;
        int value = 0;
        for (int v : adj[u]) {
            if (v == parent) { // consume ONE edge back to the parent; a parallel one is a real cycle
                parent += 2 * n;
                continue;
            }
            if (tin[v] == 0) {
                int above = dfs(v, u);
                low[u] = min(low[u], low[v]);
                if (low[v] > tin[u])
                    value ^= (1 + above) ^ 1; // a bridge: the edge plus whatever hangs above it
                else
                    value ^= above; // inside a cycle, so the edge contracts away
            } else
                low[u] = min(low[u], tin[v]);
        }
        if (parent > n) parent -= 2 * n;
        for (int v : adj[u])
            if (v != parent && tin[u] <= tin[v]) value ^= 1; // each cycle closed here fuses to one loop
        return value;
    }

    int grundy() { // 0 means the player to move loses
        fill(tin.begin(), tin.end(), 0);
        fill(low.begin(), low.end(), 0);
        timer = 0;
        return dfs(root, 0);
    }

    bool first_player_wins() { return grundy() != 0; }
};

// Standard problem: SPOJ PLAYTREE - a graph grounded at node 1, report who wins
void solve() {
    int n, m;
    cin >> n >> m;
    Hackenbush h(n);
    h.set_ground(1);
    for (int i = 0, u, v; i < m; i++) {
        cin >> u >> v;
        h.add_edge(u, v);
    }
    cout << (h.first_player_wins() ? "Alice" : "Bob") << '\n';
}
