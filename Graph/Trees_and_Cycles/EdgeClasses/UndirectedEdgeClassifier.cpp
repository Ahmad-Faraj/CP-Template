#include "../../../core.h"
/*
 * Topic: Undirected Edge Classifier
 * Description: Classifies edges (tree, back) during DFS on an undirected graph.
 * 
 * Important Facts:
 * - 0-based indexing for nodes by default.
 * - Time Complexity: O(V + E)
 * - Space Complexity: O(V + E)
 */

struct UndirectedEdgeClassifier {
    int n, timer;
    vector<vector<int>> adj;
    vector<int> tin, depth;
    vector<bool> vis;

    // Time Complexity: O(V)
    // Space Complexity: O(V)
    // Initialize undirected edge classifier with number of nodes.
    UndirectedEdgeClassifier(int _n) : n(_n), timer(0), adj(n), tin(n, 0), depth(n, 0), vis(n, false) {}

    // Time Complexity: O(1)
    // Space Complexity: O(1)
    // Add an undirected edge between u and v.
    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Time Complexity: O(V + E)
    // Space Complexity: O(V)
    // Traverse graph to classify tree, forward, and backward edges.
    void dfs(int u, int p = -1) {
        vis[u] = true;
        tin[u] = ++timer;
        depth[u] = (p == -1 ? 1 : depth[p] + 1);
        for (int v : adj[u]) {
            if (vis[v] && tin[u] < tin[v]) {
                // forward edge
                continue;
            }
            if (!vis[v]) {
                // tree edge
                dfs(v, u);
            } else if (v != p) {
                // backward edge
            }
        }
    }

    // Time Complexity: O(V + E)
    // Space Complexity: O(V)
    // Run the classifier on all connected components.
    void run() {
        for (int i = 0; i < n; ++i) {
            if (!vis[i]) dfs(i);
        }
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m;
    if (cin >> n >> m) {
        UndirectedEdgeClassifier uec(n);
        for (int i = 0; i < m; i++) {
            int u, v;
            cin >> u >> v;
            uec.add_edge(u - 1, v - 1);
        }
        uec.run();
        cout << "Processed\n";
    }
    return 0;
}
