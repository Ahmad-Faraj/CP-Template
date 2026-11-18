// Small To Large: merge each child's set into its parent's, always the smaller into the larger.
// Use when: "number of distinct colours in every subtree", per-subtree sets / maps of values.
// Usage: Small_To_Large S(n); S.add_edge(u, v); S.colors[u].insert(c); auto d = S.run(root);
// Handles: several values per node (insert more than one), any value range, n = 1.
// Not: the O(n log n) sack technique (that is DSU_On_Tree.cpp); swap set<int> for map to count.
// Time: O(n log^2 n)
// Indexing: 1-based nodes; run() returns distinct_num with distinct_num[u] for u in 1..n
// Note: run() empties colors[] (it is consumed). dfs is recursive (depth = tree height).

#include <bits/stdc++.h>
using namespace std;

struct Small_To_Large {
    int n;
    vector<vector<int>> adj;
    vector<set<int>> colors; // the values sitting on each node; fill before run()
    vector<int> distinct_num;

    Small_To_Large(int n) : n(n), adj(n + 1), colors(n + 1), distinct_num(n + 1) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void dfs(int node, int par) {
        for (int child : adj[node]) {
            if (child == par) continue;
            dfs(child, node);
            // make node the larger set always
            if (colors[node].size() < colors[child].size()) swap(colors[node], colors[child]);
            for (int item : colors[child]) colors[node].insert(item);
            colors[child].clear();
        }
        distinct_num[node] = (int)colors[node].size();
    }

    vector<int> run(int root = 1) {
        dfs(root, 0);
        return distinct_num;
    }
};
