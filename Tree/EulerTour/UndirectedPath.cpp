#include "../../core.h"
/*
 * Topic: Eulerian Paths and Cycles
 * 
 * Rules & Heuristics:
 * - A path that visits every edge exactly once.
 * - Exists in Undirected if: 0 or 2 nodes have odd degree, and graph is connected.
 * - Exists in Directed if: At most 1 node has out-in=1, 1 node has in-out=1, others in=out.
 * - Uses Hierholzer's Algorithm (DFS deleting edges).
 * - Complexity: O(V + E) time.
 */
// Time Complexity: O(V + E)
// Space Complexity: O(V + E)

// Time Complexity: O((V + E) log E)
// Space Complexity: O(V + E)
struct UndirectedPath {
    int n, m;
    vector<set<int>> adj;
    vector<int> degree, tour;

    // Time Complexity: O(V)
    // Space Complexity: O(V)
    // Usage: Initialize Eulerian path structure for an undirected graph.
    UndirectedPath(int _n, int _m) : n(_n), m(_m), adj(n), degree(n, 0) {}

    // Time Complexity: O(log E)
    // Space Complexity: O(1)
    // Usage: Add an undirected edge between u and v.
    void add_edge(int u, int v) {
        adj[u].insert(v);
        adj[v].insert(u);
        degree[u]++;
        degree[v]++;
    }

    // Time Complexity: O((V + E) log E)
    // Space Complexity: O(V + E)
    // Usage: Apply Hierholzer's algorithm to recursively form the path.
    void dfs(int node) {
        while (!adj[node].empty()) {
            auto it = prev(adj[node].end());
            int x = *it;
            adj[node].erase(it);
            if (adj[x].erase(node)) { dfs(x); }
        }
        tour.emplace_back(node);
    }

    // Time Complexity: O(V)
    // Space Complexity: O(1)
    // Usage: Ensure exactly zero or two nodes have odd degree.
    bool check() {
        int odds = 0;
        for (int i = 0; i < n; ++i) {
            if (degree[i] & 1) odds++;
        }
        return odds == 0 || odds == 2;
    }

    // Time Complexity: O(V + E)
    // Space Complexity: O(V + E)
    // Usage: Verify that the graph (edges) is connected.
    bool isConnected() {
        int start = -1;
        for (int i = 0; i < n; ++i) {
            if (degree[i] > 0) {
                start = i;
                break;
            }
        }
        if (start == -1) return true;
        vector<char> vis(n, 0);
        stack<int> st;
        st.push(start);
        vis[start] = 1;
        while (!st.empty()) {
            int v = st.top();
            st.pop();
            for (int u : adj[v]) {
                if (!vis[u]) {
                    vis[u] = 1;
                    st.push(u);
                }
            }
        }
        for (int i = 0; i < n; ++i) {
            if (degree[i] > 0 && !vis[i]) return false;
        }
        return true;
    }

    // Time Complexity: O((V + E) log E)
    // Space Complexity: O(V + E)
    // Usage: Check validity, compute, and return the undirected Eulerian path.
    bool get() {
        if (!check() || !isConnected()) return false;
        int start = -1;
        for (int i = 0; i < n; ++i) {
            if (degree[i] & 1) {
                start = i;
                break;
            }
        }
        if (start == -1) {
            for (int i = 0; i < n; ++i) {
                if (degree[i] > 0) {
                    start = i;
                    break;
                }
            }
        }
        if (start == -1) return (m == 0);
        tour.clear();
        dfs(start);
        reverse(tour.begin(), tour.end());
        return (int)(tour).size() == m + 1;
    }
};
