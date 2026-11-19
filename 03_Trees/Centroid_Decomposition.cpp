// Centroid Decomposition: builds the centroid tree (depth O(log n)) of a tree.
// Use when: counting or optimising over all paths - "paths of length k", "nearest marked node".
// Usage: centroid_decomposition cd(n); cd.add_edge(u, v); cd.decompose(); then cd.par[c].
// Handles: any tree, n = 1. par[c] is c's parent in the centroid tree, -1 for its root.
// Time: O(n log n)
// Indexing: 1-based nodes
// Note: call decompose() once; a rerun needs a fresh object.
// Note: recursive (depth = tree height), so a 2e5 path needs a big stack.

#include <bits/stdc++.h>
using namespace std;

struct centroid_decomposition {
    int n;
    vector<int> sz, par;
    vector<bool> vis;
    vector<vector<int>> adj;

    centroid_decomposition(int n) : n(n), sz(n + 1), par(n + 1, -1), vis(n + 1), adj(n + 1) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    int get_subtree_size(int u, int p) {
        sz[u] = 1;
        for (auto v : adj[u]) {
            if (v == p || vis[v]) continue;
            sz[u] += get_subtree_size(v, u);
        }
        return sz[u];
    }

    int get_centroid(int u, int p, int tree_size) {
        for (auto v : adj[u]) {
            if (v == p || vis[v]) continue;
            if (sz[v] * 2 > tree_size) return get_centroid(v, u, tree_size);
        }
        return u;
    }

    void decompose(int u = 1, int p = -1) {
        int tree_size = get_subtree_size(u, p);
        int centroid = get_centroid(u, p, tree_size);
        vis[centroid] = true;
        par[centroid] = p;
        for (auto v : adj[centroid]) {
            if (vis[v]) continue;
            decompose(v, centroid);
        }
    }
};
