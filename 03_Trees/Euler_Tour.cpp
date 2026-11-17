// Euler Tour: number the nodes in dfs order so every subtree becomes one contiguous range.
// Use when: "update a node, query the sum / max over the subtree of u", "is u an ancestor of v".
// Usage: Euler_Tour E(n); E.add_edge(u, v); E.build(root); range [E.start[u], E.finish[u]].
// Handles: forests (unvisited nodes start new trees after root), n = 1.
// Not: path queries (use HLD.cpp). Pair it with any range structure (Fenwick, segment tree).
// Time: build O(n), is_ancestor O(1)
// Indexing: 1-based nodes and 1-based positions; order[pos] = the node at position pos
// Note: dfs is recursive (depth = tree height), so a 2e5 path needs a big stack.

#include <bits/stdc++.h>
using namespace std;

struct Euler_Tour {
    int n, timer = 0;
    vector<vector<int>> adj;
    vector<int> start, finish, order; // subtree of u = positions start[u] .. finish[u]

    Euler_Tour(int n) : n(n), adj(n + 1), start(n + 1), finish(n + 1), order(n + 1) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void euler_tour(int src, int par) {
        start[src] = ++timer;
        order[timer] = src;
        for (int child : adj[src])
            if (child != par) euler_tour(child, src);
        finish[src] = timer;
    }

    void build(int root = 1) {
        timer = 0;
        fill(start.begin(), start.end(), 0);
        euler_tour(root, 0);
        for (int v = 1; v <= n; v++)
            if (!start[v]) euler_tour(v, 0);
    }

    bool is_ancestor(int u, int v) { return start[u] <= start[v] && finish[v] <= finish[u]; }

    template <class T> vector<T> flatten(const vector<T> &val) { // val[u] -> out[start[u]]
        vector<T> out(n + 1);
        for (int u = 1; u <= n; u++) out[start[u]] = val[u];
        return out;
    }
};
