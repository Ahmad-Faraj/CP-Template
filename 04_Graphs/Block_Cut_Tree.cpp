// Block Cut Tree: vertex-biconnected components (blocks), cut vertices, and the block-cut tree.
// Use when: "must every path from u to v pass through x", "cut vertices on the u-v path",
//           "split into 2-vertex-connected parts", queries on paths through articulation points.
// Usage: Block_Cut_Tree B(n); B.add_edge(u, v); B.build();
//        B.comps[b] = vertices of block b, B.is_cut[v], B.tree = adjacency of the block-cut tree.
// Handles: disconnected graphs, multi-edges, self loops (ignored), isolated vertices (own block).
// Time: O(n + m)
// Indexing: 1-based vertices. Blocks 0-based; in tree, vertex v is node v and block b is node
//           n + 1 + b (tree has n + 1 + comps.size() nodes, node 0 unused). Each tree edge joins
//           a vertex and a block containing it, so it is bipartite (a forest if disconnected).
// Note: a bridge is a block of 2 vertices. Cut vertices strictly inside the u-v tree path are the
//       vertices every u-v path must use. dfs recurses up to n deep.

#include <bits/stdc++.h>
using namespace std;

struct Block_Cut_Tree {
    int n, timer = 0;
    vector<vector<int>> g, comps, tree;
    vector<int> tin, low, st;
    vector<char> is_cut;

    Block_Cut_Tree(int n) : n(n), g(n + 1) {}

    void add_edge(int u, int v) {
        if (u == v) return;
        g[u].push_back(v), g[v].push_back(u);
    }

    void dfs(int u, int p) {
        tin[u] = low[u] = ++timer;
        st.push_back(u);
        for (int v : g[u]) {
            if (!tin[v]) {
                dfs(v, u);
                low[u] = min(low[u], low[v]);
                if (low[v] >= tin[u]) {
                    comps.push_back({u});
                    int x;
                    do x = st.back(), st.pop_back(), comps.back().push_back(x);
                    while (x != v);
                }
            } else if (v != p) low[u] = min(low[u], tin[v]);
        }
    }

    void build() {
        tin.assign(n + 1, 0), low.assign(n + 1, 0), comps.clear(), timer = 0;
        for (int v = 1; v <= n; v++)
            if (!tin[v]) {
                dfs(v, 0);
                st.clear();
                if (g[v].empty()) comps.push_back({v});
            }
        is_cut.assign(n + 1, 0);
        vector<int> blocks_of(n + 1, 0);
        tree.assign(n + 1 + comps.size(), {});
        for (int b = 0; b < (int)comps.size(); b++)
            for (int v : comps[b]) {
                blocks_of[v]++;
                tree[v].push_back(n + 1 + b), tree[n + 1 + b].push_back(v);
            }
        for (int v = 1; v <= n; v++) is_cut[v] = blocks_of[v] > 1;
    }
};
