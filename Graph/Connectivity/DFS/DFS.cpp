#include "../../../core.h"
/*
 * Algorithm: Depth First Search (DFS)
 * Purpose: Basic graph traversal recording entry/exit times and component sizes.
 * Complexity: Time O(V + E) | Space O(V + E)
 * Verified: ..........
 */

template <typename T = int>
struct DFS {
    int n;
    int timer;
    vector<vector<T>> adj;
    vector<T> vis, par, in, out, depth, subtree, color;

    // Time Complexity: O(V)
    // Space Complexity: O(V)
    // Initialize the DFS structure for a graph with n nodes.
    DFS(int _n) : n(_n), timer(0), adj(_n + 1), vis(_n + 1, 0), par(_n + 1, 0), in(_n + 1, 0), 
                  out(_n + 1, 0), depth(_n + 1, 0), subtree(_n + 1, 0), color(_n + 1, 0) {}

    // Time Complexity: O(1) amortized
    // Space Complexity: O(1)
    // Add a directed or undirected edge.
    void add_edge(T u, T v, bool undirected = true) {
        adj[u].push_back(v);
        if (undirected) adj[v].push_back(u);
    }

    // Time Complexity: O(V + E)
    // Space Complexity: O(V)
    // Initiates a DFS from a specific node. Returns true if the component is bipartite.
    bool dfs_visit(T u) {
        timer++;
        in[u] = timer;
        vis[u] = 1;
        subtree[u] = 1;
        bool res = true;
        for (auto v : adj[u]) {
            if (!vis[v]) {
                par[v] = u;
                depth[v] = depth[u] + 1;
                color[v] = 1 - color[u];
                res &= dfs_visit(v);
                subtree[u] += subtree[v];
            } else if (color[v] == color[u]) {
                res = false; // Not bipartite
            }
        }
        timer++;
        out[u] = timer;
        return res;
    }

    // Time Complexity: O(V + E)
    // Space Complexity: O(V)
    // Initiates DFS over all components. Returns true if the entire graph is bipartite.
    bool solve() {
        fill(vis.begin(), vis.end(), 0);
        timer = 0;
        bool is_bipartite = true;
        for (int i = 1; i <= n; ++i) {
            if (!vis[i]) {
                is_bipartite &= dfs_visit(i);
            }
        }
        return is_bipartite;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m;
    if (cin >> n >> m) {
        DFS<int> dfs_solver(n);
        for (int i = 0; i < m; i++) {
            int u, v;
            cin >> u >> v;
            dfs_solver.add_edge(u, v, true);
        }
        bool bipartite = dfs_solver.solve();
        if (bipartite) cout << "Bipartite\n";
        else cout << "Not Bipartite\n";
    }
    return 0;
}
