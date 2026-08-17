// LCA: lowest common ancestor, k-th ancestor, and distance between any two nodes, by binary lifting.
// Use when: "LCA of u and v", "distance between u and v", "k-th ancestor of u", any path query on a rooted tree.
// Handles: any rooted tree, k-th ancestor, LCA, edge-count distance. Static - edges cannot change after build().
// Time: build O(n log n) | get_lca O(log n) | kth_ancestor O(log n) | dist O(log n)
// Indexing: 1-based nodes
// Note: call build(root) once, after every add_edge. dfs is recursive, so a path-shaped tree can overflow the stack.

#include <bits/stdc++.h>
using namespace std;

struct LCA {
    int N, LOG;
    vector<vector<int>> anc, adj;
    vector<int> dep;

    LCA(int n = 0) {
        N = n + 10, LOG = 0;
        while ((1 << LOG) <= N) LOG++;
        dep = vector<int>(N);
        adj = vector<vector<int>>(N);
        anc = vector<vector<int>>(N, vector<int>(LOG));
    }

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void dfs(int u, int p = 0) {
        for (auto &v : adj[u]) {
            if (v == p) continue;
            dep[v] = dep[u] + 1, anc[v][0] = u;
            for (int bit = 1; bit < LOG; bit++) anc[v][bit] = anc[anc[v][bit - 1]][bit - 1];
            dfs(v, u);
        }
    }

    void build(int root = 1) { dfs(root); } // run once, after every add_edge

    int kth_ancestor(int u, int k) { // the ancestor k levels above u, or -1 if u is not that deep
        if (dep[u] < k) return -1;
        for (int bit = LOG - 1; bit >= 0; bit--)
            if (k & (1 << bit)) u = anc[u][bit];
        return u;
    }

    int get_lca(int u, int v) { // lowest common ancestor of u and v
        if (dep[u] < dep[v]) swap(u, v);
        u = kth_ancestor(u, dep[u] - dep[v]);
        if (u == v) return u;
        for (int bit = LOG - 1; bit >= 0; bit--)
            if (anc[u][bit] != anc[v][bit]) u = anc[u][bit], v = anc[v][bit];
        return anc[u][0];
    }

    int dist(int u, int v) { return dep[u] + dep[v] - 2 * dep[get_lca(u, v)]; } // edges on the path u..v
};

// Standard problem: n nodes, then q queries each asking the LCA and the distance of a pair
void solve() {
    int n, q;
    cin >> n >> q;
    LCA t(n);
    for (int i = 1, u, v; i < n; i++) {
        cin >> u >> v;
        t.add_edge(u, v);
    }
    t.build(1);
    while (q--) {
        int u, v;
        cin >> u >> v;
        cout << t.get_lca(u, v) << ' ' << t.dist(u, v) << '\n';
    }
}
