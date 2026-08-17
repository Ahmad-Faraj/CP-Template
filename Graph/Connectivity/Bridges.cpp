// Bridges: the edges whose removal disconnects the graph, plus the two-edge-connected components they separate.
// Use when: "which roads are critical", "how many edges must fail to split the network", building a bridge tree.
// Handles: multi-edges (two parallel edges are never a bridge), self loops, disconnected graphs.
// Time: O(n + m)
// Indexing: 1-based nodes; component ids run 1..component_count
// Note: run() is recursive. A bridge is an EDGE - for the vertex version see Articulation_Points.cpp.

#include <bits/stdc++.h>
using namespace std;

struct Bridges {
    int n, timer = 0, edges = 0, comp_count = 0;
    vector<vector<pair<int, int>>> adj; // (neighbour, edge id)
    vector<int> tin, low, comp;
    vector<pair<int, int>> bridge_list;
    vector<char> is_bridge_edge;

    Bridges(int n) : n(n), adj(n + 1), tin(n + 1, 0), low(n + 1, 0), comp(n + 1, 0) {}

    void add_edge(int u, int v) {
        adj[u].push_back({v, edges});
        adj[v].push_back({u, edges});
        edges++;
    }

    void dfs(int u, int in_edge) {
        tin[u] = low[u] = ++timer;
        for (auto [v, id] : adj[u]) {
            if (id == in_edge) continue; // skip the one edge we arrived on, not every edge to the parent
            if (tin[v])
                low[u] = min(low[u], tin[v]);
            else {
                dfs(v, id);
                low[u] = min(low[u], low[v]);
                if (low[v] > tin[u]) {
                    bridge_list.push_back({u, v});
                    is_bridge_edge[id] = 1;
                }
            }
        }
    }

    void label_components() { // two-edge-connected components: the pieces left when bridges are cut
        for (int s = 1; s <= n; s++) {
            if (comp[s]) continue;
            comp[s] = ++comp_count;
            vector<int> stack_{s};
            while (!stack_.empty()) {
                int u = stack_.back();
                stack_.pop_back();
                for (auto [v, id] : adj[u]) {
                    if (is_bridge_edge[id] || comp[v]) continue;
                    comp[v] = comp_count;
                    stack_.push_back(v);
                }
            }
        }
    }

    void run() { // call once, after every add_edge
        is_bridge_edge.assign(edges, 0);
        for (int i = 1; i <= n; i++)
            if (!tin[i]) dfs(i, -1);
        label_components();
    }

    int bridge_count() { return (int)bridge_list.size(); }
    int component_count() { return comp_count; }
    int component_of(int u) { return comp[u]; }
    bool is_bridge(int u, int v) { return comp[u] != comp[v]; } // for an edge u-v that exists

    vector<vector<int>> bridge_tree() { // the tree over two-edge-connected components
        vector<vector<int>> tree(comp_count + 1);
        for (auto [u, v] : bridge_list) tree[comp[u]].push_back(comp[v]), tree[comp[v]].push_back(comp[u]);
        return tree;
    }
};

// Standard problem: report how many bridges the graph has, and list them
void solve() {
    int n, m;
    cin >> n >> m;
    Bridges b(n);
    for (int i = 0, u, v; i < m; i++) {
        cin >> u >> v;
        b.add_edge(u, v);
    }
    b.run();
    cout << b.bridge_count() << '\n';
    for (auto [u, v] : b.bridge_list) cout << u << ' ' << v << '\n';
}
