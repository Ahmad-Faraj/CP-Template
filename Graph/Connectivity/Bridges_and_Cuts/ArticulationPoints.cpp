#include "../../../core.h"

/*
 * Topic: Articulation Points (Cut Vertices)
 * Description: Finds vertices that disconnect the graph if removed.
 *
 * Important Facts:
 * - 1-based indexing for nodes by default.
 * - Time Complexity: O(V + E)
 * - Space Complexity: O(V + E)
 */
template <typename T = int> struct ArticulationPoints {
    T dfs_timer;
    vector<T> dfs_num, dfs_low, vis;
    vector<vector<T>> adj;
    set<T> articulation_points;

    // Time Complexity: O(V)
    // Space Complexity: O(V)
    // Initializes required structures for V vertices.
    ArticulationPoints(T n) {
        dfs_timer = 0;
        adj = vector<vector<T>>(n + 1);
        dfs_num = dfs_low = vis = vector<T>(n + 1);
    }

    // Time Complexity: O(1) amortized
    // Space Complexity: O(1)
    // Adds an undirected edge between vertices u and v.
    void add_edge(T u, T v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Time Complexity: O(V + E) overall across all calls
    // Space Complexity: O(V) for recursion stack
    // DFS traversal to compute discovery times and find articulation points.
    void tarjan(T node, T parent) {
        dfs_num[node] = dfs_low[node] = ++dfs_timer;
        vis[node] = 1;
        T children = 0;
        for (auto child : adj[node]) {
            if (child == parent) continue;
            if (!dfs_num[child]) {
                tarjan(child, node);
                dfs_low[node] = min(dfs_low[node], dfs_low[child]);
                if (dfs_low[child] >= dfs_num[node] && parent != -1) {
                    articulation_points.insert(node);
                }
                ++children;
            } else if (vis[child]) {
                dfs_low[node] = min(dfs_low[node], dfs_num[child]);
            }
        }
        if (parent == -1 && children > 1) {
            articulation_points.insert(node);
        }
    }

    // Time Complexity: O(V + E)
    // Space Complexity: O(V)
    // Executes Tarjan's algorithm to identify all articulation points.
    void solve() {
        for (T i = 1; i < (T)(int)(adj).size(); ++i)
            if (!dfs_num[i]) tarjan(i, -1);
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m;
    if (cin >> n >> m) {
        ArticulationPoints<int> ap(n);
        for (int i = 0; i < m; i++) {
            int u, v;
            cin >> u >> v;
            ap.add_edge(u, v);
        }
        ap.solve();
        cout << ap.articulation_points.size() << "\n";
        for (auto v : ap.articulation_points) {
            cout << v << " ";
        }
        cout << "\n";
    }
    return 0;
}
