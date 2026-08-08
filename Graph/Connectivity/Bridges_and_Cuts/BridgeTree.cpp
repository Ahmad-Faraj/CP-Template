#include "../../../core.h"
/*
 * Algorithm: Bridge Tree (2-Edge-Connected Components)
 * Purpose: Condenses a graph by grouping 2-edge-connected components into nodes.
 * Complexity: Time O(V + E) | Space O(V + E)
 * Verified: ..........
 */
struct BridgeTree {
    int n, timer;
    vector<vector<int>> adj;
    vector<int> tin, low, comp;
    vector<bool> is_bridge;
    vector<pair<int, int>> edges;

    // Time Complexity: O(V)
    // Space Complexity: O(V)
    // BridgeTree bt(n);
    BridgeTree(int n_) : n(n_), timer(0), adj(n_), tin(n_, -1), low(n_, -1), comp(n_, -1) {}

    // Time Complexity: O(1)
    // Space Complexity: O(1)
    // bt.addEdge(u, v);
    void addEdge(int u, int v) {
        adj[u].push_back(static_cast<int>(edges.size()));
        adj[v].push_back(static_cast<int>(edges.size()));
        edges.push_back({u, v});
        is_bridge.push_back(false);
    }

    // Time Complexity: O(V + E)
    // Space Complexity: O(V + E)
    // Used internally.
    void dfs(int v, int p) {
        tin[v] = low[v] = timer++;
        for (int id : adj[v]) {
            if (id == p) continue;
            int to = edges[id].first ^ edges[id].second ^ v;
            if (tin[to] != -1) {
                low[v] = min(low[v], tin[to]);
            } else {
                dfs(to, id);
                low[v] = min(low[v], low[to]);
                if (low[to] > tin[v]) {
                    is_bridge[id] = true;
                }
            }
        }
    }

    // Time Complexity: O(V + E)
    // Space Complexity: O(V + E)
    // Used internally.
    void dfs_comp(int v, int c) {
        comp[v] = c;
        for (int id : adj[v]) {
            int to = edges[id].first ^ edges[id].second ^ v;
            if (comp[to] == -1 && !is_bridge[id]) {
                dfs_comp(to, c);
            }
        }
    }

    // Time Complexity: O(V + E)
    // Space Complexity: O(V)
    // int num_2ecc = bt.build();
    int build() {
        for (int i = 0; i < n; i++) {
            if (tin[i] == -1) {
                dfs(i, -1);
            }
        }
        int c = 0;
        for (int i = 0; i < n; i++) {
            if (comp[i] == -1) {
                dfs_comp(i, c++);
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
        BridgeTree bt(n);
        for (int i = 0; i < m; i++) {
            int u, v;
            cin >> u >> v;
            bt.addEdge(u - 1, v - 1);
        }
        int num_2ecc = bt.build();
        cout << num_2ecc << "\n";
        for (int i = 0; i < n; i++) {
            cout << bt.comp[i] + 1 << (i == n - 1 ? "" : " ");
        }
        cout << "\n";
    }
    return 0;
}
