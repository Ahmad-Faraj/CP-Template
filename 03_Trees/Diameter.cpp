// Tree Diameter: the longest path in an unweighted tree - length, both endpoints, the path itself.
// Use when: "longest path in the tree", "farthest pair of nodes", tree centre / radius.
// Usage: Tree_Diameter T(n); T.add_edge(u, v); auto [len, a, b] = T.diameter(); T.path();
// Handles: n = 1 (length 0, a = b = 1). After diameter(), dist[] and parent[] are rooted at a.
// Not: weighted edges, forests (only the component of node 1 is searched).
// Time: O(n)
// Indexing: 1-based nodes; length counts edges
// Note: dfs is recursive (depth = tree height), so a 2e5 path needs a big stack.

#include <bits/stdc++.h>
using namespace std;

struct Tree_Diameter {
    int n;
    vector<vector<int>> adj;
    vector<int> dist, parent;

    Tree_Diameter(int n = 0) : n(n), adj(n + 1), dist(n + 1), parent(n + 1) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void dfs(int u, int p = -1, int d = 0) {
        dist[u] = d;
        parent[u] = p;
        for (int v : adj[u])
            if (v != p) dfs(v, u, d + 1);
    }

    int farthest(int src) { // a node farthest from src (fills dist / parent from src)
        dfs(src);
        int best = src;
        for (int i = 1; i <= n; i++)
            if (dist[i] > dist[best]) best = i;
        return best;
    }

    array<int, 3> diameter() { // {length in edges, endpoint a, endpoint b}
        int a = farthest(1), b = farthest(a);
        return {dist[b], a, b};
    }

    vector<int> path() { // nodes of one diameter, from b back to a
        int b = diameter()[2];
        vector<int> p;
        for (int u = b; u != -1; u = parent[u]) p.push_back(u);
        return p;
    }
};
