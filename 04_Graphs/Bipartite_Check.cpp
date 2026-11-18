// Bipartite Check: 2-colours an undirected graph by BFS, or reports that no 2-colouring exists.
// Use when: "split into two teams so no two friends share a team", "is the graph bipartite".
// Usage: Bipartite_Check B(n); B.add_edge(u, v); vector<int> c = B.run(); // empty = IMPOSSIBLE
// Handles: disconnected graphs, isolated nodes, multi-edges; a self loop makes it impossible.
// Time: O(n + m)
// Indexing: 1-based nodes; run() gives color[1..n] in {1, 2} (color[0] unused), {} on odd cycle
// Note: iterative BFS, no recursion. Each component's smallest node gets colour 1.

#include <bits/stdc++.h>
using namespace std;

struct Bipartite_Check {
    int n;
    vector<vector<int>> adj;
    vector<int> color;

    Bipartite_Check(int n) : n(n), adj(n + 1), color(n + 1) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    bool bfs(int s) { // colour s's component; false on an odd cycle
        queue<int> q;
        color[s] = 1;
        q.push(s);
        while (q.size()) {
            int u = q.front();
            q.pop();
            for (int v : adj[u]) {
                if (!color[v]) {
                    color[v] = color[u] % 2 + 1;
                    q.push(v);
                } else if (color[v] == color[u])
                    return false;
            }
        }
        return true;
    }

    vector<int> run() {
        color.assign(n + 1, 0);
        for (int i = 1; i <= n; i++)
            if (!color[i] && !bfs(i)) return {};
        return color;
    }
};
