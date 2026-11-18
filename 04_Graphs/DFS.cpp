// DFS: one traversal giving the DFS forest: parent, depth, subtree size, in/out times, 2-colour.
// Use when: rooting a tree (par/depth/subtree), "is u an ancestor of v", "is the graph bipartite".
// Usage: DFS D(n); D.add_edge(u, v, true); bool bip = D.dfs(); then D.par / depth / in / out...
// Handles: forests / disconnected graphs (each unvisited node starts a new tree), self loops.
// Time: O(n + m)
// Indexing: 1-based nodes; roots get par 0, depth 0, colour 0; times run 1..2n
// Note: dfs() returns "bipartite", meaningful only if every edge was added both ways.
// Note: u is an ancestor of v iff in[u] <= in[v] && out[v] <= out[u].
// Note: dfs_visit is recursive (depth = tree height), so a 2e5 path needs a big stack.

#include <bits/stdc++.h>
using namespace std;

struct DFS {
    int n;
    int timer;
    vector<vector<int>> adj;
    vector<int> vis;
    vector<int> par;
    vector<int> in;
    vector<int> out;
    vector<int> depth;
    vector<int> subtree;
    vector<int> color;

    DFS(int n) : n(n) {
        adj.resize(n + 1);
        vis.resize(n + 1);
        par.resize(n + 1);
        in.resize(n + 1);
        out.resize(n + 1);
        depth.resize(n + 1);
        subtree.resize(n + 1);
        color.resize(n + 1);
        timer = 0;
    }

    void add_edge(int u, int v, bool bidirectional = false) {
        adj[u].push_back(v);
        if (bidirectional) adj[v].push_back(u);
    }

    bool dfs() {
        for (int i = 1; i <= n; ++i) {
            vis[i] = 0;
        }
        timer = 0;
        bool ok = true;
        for (int i = 1; i <= n; ++i) {
            if (!vis[i]) {
                ok &= dfs_visit(i);
            }
        }
        return ok;
    }

    bool dfs_visit(int u) {
        timer += 1;
        in[u] = timer;
        vis[u] = 1;
        subtree[u] = 1;
        bool res = true;
        for (auto v : adj[u]) {
            if (!vis[v]) {
                par[v] = u;
                depth[v] = depth[u] + 1;
                color[v] = 1 - color[u];
                res &= dfs_visit(v);
                subtree[u] += subtree[v];
            } else if (color[v] == color[u]) {
                res = false;
            }
        }
        timer += 1;
        out[u] = timer;
        return res;
    }
};
