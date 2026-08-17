// Depth First Search: one traversal yielding the DFS forest, entry/exit times, subtree sizes, components, colours.
// Use when: "how many components", "is there a cycle", "is the graph bipartite", "in what order can these tasks run".
// Handles: directed or undirected, disconnected graphs, multi-edges, self loops. Cycle rules respect direction.
// Time: O(n + m)
// Indexing: 1-based nodes; component ids run 1..component_count
// Note: run() is recursive, so ~1e5 deep needs a bigger stack. Pass directed at construction, not later.

#include <bits/stdc++.h>
using namespace std;

struct Depth_First_Search {
    int n, timer = 0, comp_count = 0, edges = 0;
    bool directed;
    vector<vector<pair<int, int>>> adj; // (neighbour, edge id)
    vector<int> parent, depth, subtree, tin, tout, comp, colour;
    vector<char> state; // 0 unseen, 1 on the current DFS path, 2 finished
    vector<int> finish_order;
    bool cyclic = false, bipartite = true;

    Depth_First_Search(int n, bool directed = false)
        : n(n), directed(directed), adj(n + 1), parent(n + 1, -1), depth(n + 1, 0), subtree(n + 1, 0),
          tin(n + 1, 0), tout(n + 1, 0), comp(n + 1, 0), colour(n + 1, -1), state(n + 1, 0) {}

    void add_edge(int u, int v) {
        adj[u].push_back({v, edges});
        if (!directed) adj[v].push_back({u, edges});
        edges++;
    }

    void dfs(int u, int in_edge) {
        state[u] = 1;
        tin[u] = ++timer;
        subtree[u] = 1;
        comp[u] = comp_count;
        for (auto [v, id] : adj[u]) {
            if (v == u) { // a self loop is a cycle either way
                cyclic = true;
                bipartite = false;
                continue;
            }
            if (!directed && id == in_edge) continue; // the edge we came in on, by id so parallels still count
            if (!state[v]) {
                parent[v] = u;
                depth[v] = depth[u] + 1;
                colour[v] = colour[u] ^ 1;
                dfs(v, id);
                subtree[u] += subtree[v];
            } else {
                if (directed) {
                    if (state[v] == 1) cyclic = true; // a back edge to the current path
                } else
                    cyclic = true; // any non-tree edge closes a cycle in an undirected graph
                if (colour[v] == colour[u]) bipartite = false;
            }
        }
        state[u] = 2;
        tout[u] = ++timer;
        finish_order.push_back(u);
    }

    void run() { // call once, after every add_edge
        for (int i = 1; i <= n; i++)
            if (!state[i]) {
                ++comp_count;
                colour[i] = 0;
                dfs(i, -1);
            }
    }

    int component_count() { return comp_count; }
    int component_of(int u) { return comp[u]; }
    bool same_component(int u, int v) { return comp[u] == comp[v]; }
    bool has_cycle() { return cyclic; }
    bool is_bipartite() { return !directed && bipartite; } // undirected graphs only
    bool is_ancestor(int u, int v) { return tin[u] <= tin[v] && tout[v] <= tout[u]; }

    vector<int> path_to(int v) { // from v's DFS root down to v
        vector<int> p;
        for (int x = v; x != -1; x = parent[x]) p.push_back(x);
        reverse(p.begin(), p.end());
        return p;
    }

    vector<int> topological_order() { // directed acyclic graphs only; empty if directed and cyclic
        if (!directed || cyclic) return {};
        vector<int> order = finish_order;
        reverse(order.begin(), order.end()); // reverse finishing order is a topological order
        return order;
    }
};

// Standard problem: report the component count, whether a cycle exists, and whether the graph is bipartite
void solve() {
    int n, m;
    cin >> n >> m;
    Depth_First_Search g(n);
    for (int i = 0, u, v; i < m; i++) {
        cin >> u >> v;
        g.add_edge(u, v);
    }
    g.run();
    cout << g.component_count() << '\n';
    cout << (g.has_cycle() ? "CYCLE" : "ACYCLIC") << '\n';
    cout << (g.is_bipartite() ? "BIPARTITE" : "NOT BIPARTITE") << '\n';
}
