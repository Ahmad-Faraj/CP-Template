// Centroid Decomposition: builds the centroid tree, whose depth is O(log n), in O(n log n).
// Use when: counting or optimising over all paths - "paths of length k", "nearest marked node", path queries.
// Handles: any tree. par[c] is c's parent in the centroid tree, and -1 for its root.
// Time: O(n log n)
// Indexing: 1-based nodes
// Note: call decompose(1, -1) once. vis[] marks removed centroids, so a second run needs a fresh object.

#include <bits/stdc++.h>
using namespace std;

struct centroid_decomposition {
    int n;
    vector<int> sz, par;
    vector<bool> vis;
    vector<vector<int>> adj;

    centroid_decomposition(int n) : n(n), sz(n + 1), par(n + 1, -1), vis(n + 1, false), adj(n + 1) {}

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

    void decompose(int u, int p) { // build the centroid tree; call once as decompose(1, -1)
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

// Standard problem: print each node's parent in the centroid tree, -1 marking the centroid root
void solve() {
    int n;
    cin >> n;
    centroid_decomposition cd(n);
    for (int i = 1, u, v; i < n; i++) {
        cin >> u >> v;
        cd.add_edge(u, v);
    }
    cd.decompose(1, -1);
    for (int i = 1; i <= n; i++) cout << cd.par[i] << " \n"[i == n];
}
