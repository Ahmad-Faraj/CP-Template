#include "../../../core.h"
/*
 * Algorithm: Kosaraju's Strongly Connected Components (SCC)
 * Purpose: Finds SCCs in a directed graph using two DFS passes.
 * Complexity: Time O(V + E) | Space O(V + E)
 * Verified: [CSES 1683 - Planets and Kingdoms](https://cses.fi/problemset/task/1683)
 */
struct Kosaraju {
    int n;
    vector<vector<int>> adj, rev_adj;
    vector<int> order, component;
    vector<bool> used;

    // Time Complexity: O(V)
    // Space Complexity: O(V)
    // Kosaraju kos(n);
    Kosaraju(int n_) : n(n_), adj(n_), rev_adj(n_), component(n_, -1), used(n_, false) {}

    // Time Complexity: O(1)
    // Space Complexity: O(1)
    // kos.addEdge(u, v);
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        rev_adj[v].push_back(u);
    }

    // Time Complexity: O(V + E)
    // Space Complexity: O(V + E)
    // Used internally.
    void dfs1(int v) {
        used[v] = true;
        for (int u : adj[v]) {
            if (!used[u]) dfs1(u);
        }
        order.push_back(v);
    }

    // Time Complexity: O(V + E)
    // Space Complexity: O(V + E)
    // Used internally.
    void dfs2(int v, int c) {
        component[v] = c;
        for (int u : rev_adj[v]) {
            if (component[u] == -1) dfs2(u, c);
        }
    }

    // Time Complexity: O(V + E)
    // Space Complexity: O(V)
    // int num_scc = kos.build();
    int build() {
        for (int i = 0; i < n; i++) {
            if (!used[i]) dfs1(i);
        }
        int c = 0;
        for (int i = 0; i < n; i++) {
            int v = order[n - 1 - i];
            if (component[v] == -1) {
                dfs2(v, c++);
            }
        }
        return c;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m;
    if (cin >> n >> m) {
        Kosaraju kos(n);
        for (int i = 0; i < m; i++) {
            int u, v;
            cin >> u >> v;
            kos.addEdge(u - 1, v - 1);
        }
        int num_scc = kos.build();
        cout << num_scc << "\n";
        for (int i = 0; i < n; i++) {
            cout << kos.component[i] + 1 << (i == n - 1 ? "" : " ");
        }
        cout << "\n";
    }
    return 0;
}
