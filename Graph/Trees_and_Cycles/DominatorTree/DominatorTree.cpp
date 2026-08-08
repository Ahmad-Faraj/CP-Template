#include "../../../core.h"
/*
 * Algorithm: Dominator Tree (Lengauer-Tarjan)
 * Purpose: Finds dominators in a directed graph efficiently using DFS and DSU.
 * Complexity: Time O((V+E) log V) | Space O(V + E)
 * Verified: ..........
 */
struct DominatorTree {
    int n, T;
    vector<vector<int>> g, rg, bucket, domTree;
    vector<int> sdom, par, idom, dsu, label, pdom, id, rev_;

    // Time Complexity: O(V)
    // Space Complexity: O(V)
    // Initialize the dominator tree structure with the number of nodes.
    DominatorTree(int _n) : n(_n),
          T(0),
          g(n + 1),
          rg(n + 1),
          bucket(n + 1),
          domTree(n + 1),
          sdom(n + 1),
          par(n + 1),
          idom(n + 1),
          dsu(n + 1),
          label(n + 1),
          pdom(n + 1),
          id(n + 1, 0),
          rev_(n + 1, 0) {}

    // Time Complexity: O(1)
    // Space Complexity: O(1)
    // Add a directed edge from u to v.
    void add_edge(int u, int v) { g[u].push_back(v); }

    // Time Complexity: O(log V)
    // Space Complexity: O(log V)
    // Internal DSU find method with path compression.
    int find_(int u, int x = 0) {
        if (u == dsu[u]) return x ? -1 : u;
        int v = find_(dsu[u], x + 1);
        if (v < 0) return u;
        if (sdom[label[dsu[u]]] < sdom[label[u]]) label[u] = label[dsu[u]];
        dsu[u] = v;
        return x ? v : label[u];
    }

    // Time Complexity: O(V + E)
    // Space Complexity: O(V)
    // DFS traversal to compute preorder numbers for Lengauer-Tarjan.
    void dfs(int u) {
        id[u] = ++T;
        rev_[T] = u;
        label[T] = sdom[T] = dsu[T] = T;
        for (int v : g[u]) {
            if (!id[v]) {
                dfs(v);
                par[id[v]] = id[u];
            }
            if (id[v]) rg[id[v]].push_back(id[u]);
        }
    }

    // Time Complexity: O((V + E) log V)
    // Space Complexity: O(V + E)
    // Build the dominator tree starting from the given root.
    void build(int root) {
        dfs(root);
        int valid_nodes = T;
        for (int i = valid_nodes; i >= 1; --i) {
            for (int v : rg[i]) {
                int u = find_(v);
                sdom[i] = min(sdom[i], sdom[u]);
            }
            if (i > 1) bucket[sdom[i]].push_back(i);
            for (int w : bucket[i]) {
                int v = find_(w);
                if (sdom[v] == sdom[w])
                    idom[w] = sdom[w];
                else
                    idom[w] = v;
            }
            if (i > 1) dsu[i] = par[i];
        }
        pdom[root] = root;
        for (int i = 2; i <= valid_nodes; ++i) {
            if (idom[i] != sdom[i]) idom[i] = idom[idom[i]];
            int u = rev_[idom[i]];
            int v = rev_[i];
            pdom[v] = u;
            domTree[u].push_back(v);
            domTree[v].push_back(u);
        }
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m;
    if (cin >> n >> m) {
        DominatorTree dt(n);
        for (int i = 0; i < m; i++) {
            int u, v;
            cin >> u >> v;
            dt.add_edge(u, v);
        }
        dt.build(1);
        for (int i = 1; i <= n; i++) {
            cout << dt.pdom[i] << (i == n ? "" : " ");
        }
        cout << "\n";
    }
    return 0;
}
