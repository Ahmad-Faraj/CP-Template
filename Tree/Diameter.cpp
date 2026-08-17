// Tree Diameter: the longest path in a tree - its length, both endpoints, and the path itself, in two DFS passes.
// Use when: "longest path in the tree", tree centre or radius, or any problem anchored on the diameter.
// Handles: unweighted edges, returns both endpoints and the node sequence between them. Tree must be connected.
// Time: O(n)
// Indexing: 1-based nodes
// Note: dfs is recursive, so a path-shaped tree can overflow the stack. Distances count edges, not nodes.

#include <bits/stdc++.h>
using namespace std;

struct Tree_Diameter {
    int n;
    vector<int> dist, parent;
    vector<vector<int>> adj;

    Tree_Diameter(int _n = 0) : n(_n), dist(_n + 1), parent(_n + 1), adj(_n + 1) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void dfs(int u, int p = -1, int d = 0) {
        dist[u] = d;
        parent[u] = p;
        for (auto v : adj[u])
            if (v != p) dfs(v, u, d + 1);
    }

    int farthest_from(int src) { // the node furthest from src
        dfs(src);
        int best = src;
        for (int i = 1; i <= n; i++)
            if (dist[i] > dist[best]) best = i;
        return best;
    }

    array<int, 3> diameter() { // {length in edges, one endpoint, the other endpoint}
        int a = farthest_from(1);
        int b = farthest_from(a);
        return {dist[b], a, b};
    }

    vector<int> path() { // the nodes along a diameter, in order
        auto [len, a, b] = diameter();
        (void)len;
        vector<int> p;
        for (int u = b; u != -1; u = parent[u]) p.push_back(u); // parent[] is from the dfs rooted at a
        return p;
    }
};

// Standard problem: report the diameter's length, its two endpoints, and the path between them
void solve() {
    int n;
    cin >> n;
    Tree_Diameter t(n);
    for (int i = 1, u, v; i < n; i++) {
        cin >> u >> v;
        t.add_edge(u, v);
    }
    auto [len, a, b] = t.diameter();
    cout << len << ' ' << a << ' ' << b << '\n';
    for (int u : t.path()) cout << u << ' ';
    cout << '\n';
}
