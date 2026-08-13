#include "../../../core.h"
/*
 * Topic: Directed Edge Classifier
 * Description: Classifies edges (tree, back, forward, cross) during DFS on a directed graph.
 * 
 * Important Facts:
 * - 0-based indexing for nodes by default.
 * - Time Complexity: O(V + E)
 * - Space Complexity: O(V + E)
 */

struct DirectedEdgeClassifier {
    int n, timer;
    vector<vector<int>> adj;
    vector<int> tin, tout;
    vector<bool> vis;

    // Time Complexity: O(V)
    // Space Complexity: O(V)
    // Initialize edge classifier structure with number of nodes.
    DirectedEdgeClassifier(int _n) : n(_n), timer(0), adj(n), tin(n, 0), tout(n, 0), vis(n, false) {}

    // Time Complexity: O(1)
    // Space Complexity: O(1)
    // Add a directed edge from u to v.
    void add_edge(int u, int v) { adj[u].push_back(v); }

    // Time Complexity: O(V + E)
    // Space Complexity: O(V)
    // Traverse graph to classify tree, back, forward, and cross edges.
    void dfs(int u, int /*p*/ = -1) {
        vis[u] = true;
        tin[u] = ++timer;
        for (int v : adj[u]) {
            if (!vis[v]) {
                // tree edge
                dfs(v, u);
            } else {
                if (tin[v] < tin[u] && tout[v] == 0) {
                    // back edge
                } else if (tin[v] > tin[u] && tout[v] != 0) {
                    // forward edge
                } else if (tin[v] < tin[u] && tout[v] != 0) {
                    // cross edge
                }
            }
        }
        tout[u] = ++timer;
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
        DirectedEdgeClassifier dec(n);
        for (int i = 0; i < m; i++) {
            int u, v;
            cin >> u >> v;
            dec.add_edge(u - 1, v - 1);
        }
        dec.run();
        cout << "Processed\n";
    }
    return 0;
}
