#include "../../../core.h"
/*
 * Topic: Topological Sorting
 * Description: Linearly orders a Directed Acyclic Graph (DAG) and detects cycles.
 *              Uses DFS to push nodes to answer array on exit time.
 * 
 * Important Facts:
 * - 1-based indexing for nodes by default.
 * - Time Complexity: O(V + E)
 * - Space Complexity: O(V + E)
 */
struct TopologicalSort {
    int n;
    vector<vector<int>> adj;
    vector<int> vis, ans;
    bool bad;

    // Time Complexity: O(V)
    // Space Complexity: O(V)
    // Prepares the graph structures for topological sorting.
    TopologicalSort(int _n) : n(_n), adj(n + 1), vis(n + 1) {}

    // Time Complexity: O(1) amortized
    // Space Complexity: O(1)
    // Inserts a directed edge from vertex u to v.
    void add_edge(int u, int v) { adj[u].push_back(v); }

    // Time Complexity: O(V + E) overall
    // Space Complexity: O(V)
    // Recursive DFS to visit nodes and construct the topological order.
    void dfs(int v) {
        vis[v] = 1;
        for (int u : adj[v]) {
            if (vis[u] == 0)
                dfs(u);
            else if (vis[u] == 1)
                bad = true;
        }
        vis[v] = 2;
        ans.push_back(v);
    }

    // Time Complexity: O(V + E)
    // Space Complexity: O(V)
    // Computes the topological sort and returns true if the graph is a DAG.
    bool sort() {
        fill(vis.begin(), vis.end(), 0);
        ans.clear();
        bad = false;
        for (int i = 1; i <= n; ++i)
            if (!vis[i]) dfs(i);
        reverse(ans.begin(), ans.end());
        return !bad;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m;
    if (cin >> n >> m) {
        TopologicalSort ts(n);
        for (int i = 0; i < m; i++) {
            int u, v;
            cin >> u >> v;
            ts.add_edge(u, v);
        }
        if (ts.sort()) {
            for (int i = 0; i < n; i++) {
                cout << ts.ans[i] << (i == n - 1 ? "" : " ");
            }
            cout << "\n";
        } else {
            cout << "IMPOSSIBLE\n";
        }
    }
    return 0;
}
