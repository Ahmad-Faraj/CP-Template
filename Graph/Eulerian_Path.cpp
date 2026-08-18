// Eulerian Path: a walk using every edge exactly once, and the circuit case where it returns to the start.
// Use when: "traverse every road once", reconstructing a sequence from overlapping pieces, de Bruijn sequences.
// Handles: directed and undirected graphs, multi-edges, self loops, isolated nodes, and reporting that none exists.
// Time: O(n + m)
// Indexing: 1-based nodes; edge ids are 0-based in the order added
// Note: a path needs the degree rule AND all edges in one component. Both are checked; empty means none exists.

#include <bits/stdc++.h>
using namespace std;

struct Eulerian_Path {
    int n, edges = 0;
    bool directed;
    vector<vector<pair<int, int>>> adj; // (neighbour, edge id)
    vector<int> out_deg, in_deg;
    vector<char> used;

    Eulerian_Path(int n, bool directed = false)
        : n(n), directed(directed), adj(n + 1), out_deg(n + 1, 0), in_deg(n + 1, 0) {}

    void add_edge(int u, int v) {
        adj[u].push_back({v, edges});
        out_deg[u]++, in_deg[v]++;
        if (!directed) {
            adj[v].push_back({u, edges});
            out_deg[v]++, in_deg[u]++;
        }
        edges++;
    }

    // where a path must begin, or -1 when no Eulerian path exists at all
    int start_node() {
        int start = -1, odd = 0, plus = 0, minus = 0;
        for (int v = 1; v <= n; v++) {
            if (!adj[v].empty() && start == -1) start = v; // any node with an edge, for the circuit case
            if (directed) {
                int diff = out_deg[v] - in_deg[v];
                if (diff == 1) plus++, start = v;
                else if (diff == -1) minus++;
                else if (diff != 0) return -1;
            } else if (out_deg[v] % 2) {
                odd++;
                if (odd <= 2) start = v;
            }
        }
        if (directed && !(plus == minus && plus <= 1)) return -1;
        if (!directed && odd != 0 && odd != 2) return -1;
        return start;
    }

    // the node sequence of the walk, or empty when there is no Eulerian path
    vector<int> path() {
        if (edges == 0) return {};
        int start = start_node();
        if (start == -1) return {};
        used.assign(edges, 0);
        vector<int> iter(n + 1, 0), stack_{start}, order;
        while (!stack_.empty()) { // Hierholzer, iterative so a long walk cannot blow the stack
            int u = stack_.back();
            bool advanced = false;
            while (iter[u] < (int)adj[u].size()) {
                auto [v, id] = adj[u][iter[u]++];
                if (used[id]) continue;
                used[id] = 1;
                stack_.push_back(v);
                advanced = true;
                break;
            }
            if (!advanced) {
                order.push_back(u);
                stack_.pop_back();
            }
        }
        if ((int)order.size() != edges + 1) return {}; // some edge sits in another component
        reverse(order.begin(), order.end());
        return order;
    }

    bool has_path() { return !path().empty(); }

    bool has_circuit() { // a closed walk using every edge
        vector<int> p = path();
        return !p.empty() && p.front() == p.back();
    }
};

// Standard problem: report an Eulerian path of an undirected graph, or IMPOSSIBLE
void solve() {
    int n, m;
    cin >> n >> m;
    Eulerian_Path g(n);
    for (int i = 0, u, v; i < m; i++) {
        cin >> u >> v;
        g.add_edge(u, v);
    }
    vector<int> p = g.path();
    if (p.empty()) {
        cout << "IMPOSSIBLE\n";
        return;
    }
    for (size_t i = 0; i < p.size(); i++) cout << p[i] << " \n"[i + 1 == p.size()];
}
