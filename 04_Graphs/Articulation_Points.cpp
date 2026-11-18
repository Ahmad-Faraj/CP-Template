// Articulation Points: the vertices whose removal increases the number of connected components.
// Use when: "which cities are critical", "removing one node disconnects the network".
// Usage: Articulation_points<> ap(n); ap.add_edge(u, v); ap.solve();
//        set<int> s = ap.get_articulation_points();
// Handles: disconnected graphs and isolated vertices. Undirected edges.
// Time: O(n + m)
// Indexing: 1-based nodes
// Note: tarjan() is recursive (depth up to n): it can overflow an 8 MB stack near 2e5 nodes.
//       Call solve() once.

#include <bits/stdc++.h>
using namespace std;

template <typename T = int> struct Articulation_points {

    T dfs_timer;
    vector<T> dfs_num, dfs_low, vis;
    vector<vector<T>> adj;
    set<T> articulation_points;

    Articulation_points() = default;

    Articulation_points(T n) {
        dfs_timer = 0;
        adj = vector<vector<T>>(n + 1);
        dfs_num = dfs_low = vis = vector<T>(n + 1);
    }

    void add_edge(T u, T v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void tarjan(T node, T parent) {
        dfs_num[node] = dfs_low[node] = ++dfs_timer;
        vis[node] = true;
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

    void solve() {
        for (T i = 1; i < (int)adj.size(); ++i)
            if (!dfs_num[i]) tarjan(i, -1);
    }

    void print() {
        cout << (int)articulation_points.size() << "\n";
        for (auto &x : articulation_points) {
            cout << x << "\n";
        }
    }

    set<T> get_articulation_points() { return articulation_points; }
};
