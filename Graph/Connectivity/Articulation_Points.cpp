// Articulation Points: the vertices whose removal disconnects the graph, and how many pieces each removal leaves.
// Use when: "which servers are critical", "removing which city splits the map", components left after a deletion.
// Handles: multi-edges, self loops, disconnected graphs, and the root case needing two DFS children to qualify.
// Time: O(n + m)
// Indexing: 1-based nodes
// Note: run() is recursive. An articulation point is a VERTEX - for the edge version see Bridges.cpp.

#include <bits/stdc++.h>
using namespace std;

struct Articulation_Points {
    int n, timer = 0;
    vector<vector<int>> adj;
    vector<int> tin, low, extra_pieces; // extra_pieces[u] = components gained by deleting u
    vector<char> is_cut;

    Articulation_Points(int n)
        : n(n), adj(n + 1), tin(n + 1, 0), low(n + 1, 0), extra_pieces(n + 1, 0), is_cut(n + 1, 0) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void dfs(int u, int parent) {
        tin[u] = low[u] = ++timer;
        int children = 0;
        for (int v : adj[u]) {
            if (v == parent) continue;
            if (tin[v])
                low[u] = min(low[u], tin[v]);
            else {
                dfs(v, u);
                low[u] = min(low[u], low[v]);
                if (low[v] >= tin[u] && parent != -1) {
                    is_cut[u] = 1;
                    extra_pieces[u]++; // this child's subtree falls off when u goes
                }
                children++;
            }
        }
        if (parent == -1) { // the root qualifies only with two or more DFS children
            if (children > 1) is_cut[u] = 1;
            extra_pieces[u] = max(0, children - 1);
        }
    }

    void run() { // call once, after every add_edge
        for (int i = 1; i <= n; i++)
            if (!tin[i]) dfs(i, -1);
    }

    bool is_articulation(int u) { return is_cut[u]; }

    vector<int> points() { // sorted, ascending
        vector<int> out;
        for (int i = 1; i <= n; i++)
            if (is_cut[i]) out.push_back(i);
        return out;
    }

    int components_after_removing(int u) { // in u's own component only, counting the pieces u leaves behind
        return extra_pieces[u] + 1;
    }
};

// Standard problem: report how many articulation points the graph has, and list them
void solve() {
    int n, m;
    cin >> n >> m;
    Articulation_Points ap(n);
    for (int i = 0, u, v; i < m; i++) {
        cin >> u >> v;
        ap.add_edge(u, v);
    }
    ap.run();
    vector<int> p = ap.points();
    cout << p.size() << '\n';
    for (int x : p) cout << x << '\n';
}
