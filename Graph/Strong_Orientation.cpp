// Strong Orientation: directs every undirected edge so the resulting digraph has as few components as possible.
// Use when: "make every street one-way and keep the town navigable", or "how few one-way pieces are forced".
// Handles: disconnected graphs, bridges (which force a split), multi-edges, self loops.
// Time: O(n + m)
// Indexing: 1-based nodes; edge ids are 0-based in the order added
// Note: a single component is possible only if the graph is connected and bridgeless - check is_strongly_orientable.

#include <bits/stdc++.h>
using namespace std;

struct Strong_Orientation {
    int n, edges = 0, timer = 0, bridges = 0, components = 0;
    vector<vector<pair<int, int>>> adj; // (neighbour, edge id)
    vector<pair<int, int>> edge_list;
    vector<int> tin, low;
    vector<char> used, flipped; // flipped[id] = the edge runs v->u, against the order it was added

    Strong_Orientation(int n) : n(n), adj(n + 1), tin(n + 1, -1), low(n + 1, -1) {}

    void add_edge(int u, int v) {
        adj[u].push_back({v, edges});
        adj[v].push_back({u, edges});
        edge_list.push_back({u, v});
        edges++;
    }

    void dfs(int u) { // orients each edge the first time it is walked, which is what makes this work
        low[u] = tin[u] = timer++;
        for (auto [v, id] : adj[u]) {
            if (used[id]) continue;
            used[id] = 1;
            flipped[id] = (u != edge_list[id].first);
            if (tin[v] == -1) {
                dfs(v);
                low[u] = min(low[u], low[v]);
                if (low[v] > tin[u]) bridges++;
            } else
                low[u] = min(low[u], tin[v]);
        }
    }

    void run() { // call once, after every add_edge
        used.assign(edges, 0);
        flipped.assign(edges, 0);
        timer = bridges = components = 0;
        fill(tin.begin(), tin.end(), -1);
        fill(low.begin(), low.end(), -1);
        for (int v = 1; v <= n; v++)
            if (tin[v] == -1) {
                components++;
                dfs(v);
            }
    }

    int component_count() { return components; }
    int bridge_count() { return bridges; }
    int min_scc_count() { return components + bridges; }         // the fewest components any orientation achieves
    bool is_strongly_orientable() { return components == 1 && bridges == 0; }

    pair<int, int> directed_edge(int id) { // the edge as oriented: (from, to)
        auto [u, v] = edge_list[id];
        return flipped[id] ? make_pair(v, u) : make_pair(u, v);
    }

    string orientation() { // '>' keeps the input order, '<' reverses it
        string s(edges, '>');
        for (int i = 0; i < edges; i++)
            if (flipped[i]) s[i] = '<';
        return s;
    }
};

// Standard problem: orient every edge to minimise the number of strongly connected components, and print the choice
void solve() {
    int n, m;
    cin >> n >> m;
    Strong_Orientation so(n);
    for (int i = 0, u, v; i < m; i++) {
        cin >> u >> v;
        so.add_edge(u, v);
    }
    so.run();
    cout << so.min_scc_count() << '\n';
    cout << so.orientation() << '\n';
}
