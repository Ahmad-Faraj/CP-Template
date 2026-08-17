// MST Kruskal: the cheapest set of edges connecting every node, by sorting edges and joining with a DSU.
// Use when: "connect all cities as cheaply as possible", "the widest bottleneck edge", maximum spanning trees.
// Handles: disconnected graphs (a spanning forest), multi-edges, self loops, negative weights, maximum variants.
// Time: O(m log m)
// Indexing: 1-based nodes; edge ids are 0-based in the order added
// Note: call build() once. Check is_spanning before trusting the cost - a disconnected graph gives a forest.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct MST_Kruskal {
    struct Edge {
        int id, u, v;
        ll w;
    };

    int n, joined = 0;
    vector<Edge> edges;
    vector<int> parent, rnk, chosen; // chosen holds edge ids, in increasing weight order
    ll total = 0;

    MST_Kruskal(int n) : n(n), parent(n + 1), rnk(n + 1, 0) { iota(parent.begin(), parent.end(), 0); }

    void add_edge(int u, int v, ll w) { edges.push_back({(int)edges.size(), u, v, w}); }

    int find(int u) {
        while (parent[u] != u) u = parent[u] = parent[parent[u]];
        return u;
    }

    bool join(int u, int v) {
        u = find(u), v = find(v);
        if (u == v) return false;
        if (rnk[u] < rnk[v]) swap(u, v);
        parent[v] = u;
        if (rnk[u] == rnk[v]) rnk[u]++;
        joined++;
        return true;
    }

    ll build(bool maximum = false) { // total weight of the spanning tree, or of the spanning forest
        vector<Edge> sorted = edges; // sort a copy, so edge ids stay meaningful
        sort(sorted.begin(), sorted.end(),
             [&](const Edge &a, const Edge &b) { return maximum ? a.w > b.w : a.w < b.w; });
        total = 0;
        chosen.clear();
        for (const Edge &e : sorted)
            if (join(e.u, e.v)) {
                total += e.w;
                chosen.push_back(e.id);
            }
        return total;
    }

    bool is_spanning() { return joined == n - 1; }        // false when the graph was disconnected
    int component_count() { return n - joined; }
    ll cost() { return total; }
    vector<int> edge_ids() { return chosen; }             // the ids of the edges taken

    vector<vector<pair<int, ll>>> tree() {                // the chosen edges as an adjacency list
        vector<vector<pair<int, ll>>> adj(n + 1);
        for (int id : chosen) {
            const Edge &e = edges[id];
            adj[e.u].push_back({e.v, e.w});
            adj[e.v].push_back({e.u, e.w});
        }
        return adj;
    }
};

// Standard problem: minimum cost to connect every node, or IMPOSSIBLE when the graph is disconnected
void solve() {
    int n, m;
    cin >> n >> m;
    MST_Kruskal mst(n);
    for (int i = 0, u, v; i < m; i++) {
        ll w;
        cin >> u >> v >> w;
        mst.add_edge(u, v, w);
    }
    ll c = mst.build();
    if (!mst.is_spanning())
        cout << "IMPOSSIBLE\n";
    else
        cout << c << '\n';
}
