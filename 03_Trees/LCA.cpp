// LCA: lowest common ancestor, k-th ancestor, and distance between any two nodes, by binary
//      lifting.
// Use when: "LCA of u and v", "distance between u and v", "k-th ancestor of u", any path query on a
//           rooted tree.
// Usage: LCA g(n); g.add_edge(u, v); g.build(root); g.get_lca(u, v); g.dist(u, v);
//        g.kth_ancestor(u, k);
// Not: edges changing after build() (static tree).
// Time: build O(n log n) | get_lca O(log n) | kth_ancestor O(log n) | dist O(log n)
// Indexing: 1-based nodes
// Note: call build(root) once, after every add_edge. Iterative, so a path-shaped tree is fine.

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

    void dfs(int root, int p = 0) {
        vector<pair<int, int>> stack_{{root, p}};
        while (!stack_.empty()) {
            auto [u, parent] = stack_.back();
            stack_.pop_back();
            for (int v : adj[u]) {
                if (v == parent) continue;
                dep[v] = dep[u] + 1, anc[v][0] = u;
                for (int bit = 1; bit < LOG; bit++) anc[v][bit] = anc[anc[v][bit - 1]][bit - 1];
                stack_.push_back({v, u});
            }
        }
    }

    void build(int root = 1) { dfs(root); }

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

    // edges on the path u..v
    int dist(int u, int v) { return dep[u] + dep[v] - 2 * dep[get_lca(u, v)]; }
};
