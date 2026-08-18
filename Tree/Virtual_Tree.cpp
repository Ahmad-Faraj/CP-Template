// Virtual Tree: compresses a set of k marked nodes into a tree of O(k) nodes preserving their ancestry.
// Use when: many queries each mark k nodes with sum of k bounded, and a per-query O(n) walk is too slow.
// Handles: any marked set including one node or the whole tree, weighted edges, and repeated marks.
// Time: init O(n log n) once | build O(k log k) per query, so O(sum k log k) overall
// Indexing: 1-based nodes; build returns the compressed nodes sorted by entry time, root first
// Note: build clears the previous query's adjacency itself, so vt[] is only ever valid for the latest build.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Virtual_Tree {
    int n, LOG;
    vector<vector<pair<int, ll>>> adj;
    vector<vector<int>> up, vt; // up = binary lifting; vt = the compressed tree of the latest build
    vector<int> tin, tout, dep;
    vector<ll> root_dist; // weighted distance from the root, for dist()
    vector<int> touched;
    int timer = 0;

    Virtual_Tree(int n) : n(n), adj(n + 1), vt(n + 1), tin(n + 1), tout(n + 1), dep(n + 1), root_dist(n + 1) {
        LOG = 1;
        while ((1 << LOG) <= n) LOG++;
        up.assign(LOG, vector<int>(n + 1, 0));
    }

    void add_edge(int u, int v, ll w = 1) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    void init(int root = 1) { // entry/exit times and lifting tables; call once before any build
        vector<pair<int, int>> stack_{{root, 0}}; // iterative, so a path of 2e5 nodes is safe
        up[0][root] = root;
        while (!stack_.empty()) {
            auto [u, state] = stack_.back();
            stack_.pop_back();
            if (state) {
                tout[u] = timer;
                continue;
            }
            tin[u] = ++timer;
            stack_.push_back({u, 1});
            for (int k = 1; k < LOG; k++) up[k][u] = up[k - 1][up[k - 1][u]];
            for (auto [v, w] : adj[u]) {
                if (v == up[0][u]) continue; // up[0][root] is the root itself, so the root keeps every neighbour
                up[0][v] = u, dep[v] = dep[u] + 1, root_dist[v] = root_dist[u] + w;
                stack_.push_back({v, 0});
            }
        }
    }

    bool is_ancestor(int u, int v) { return tin[u] <= tin[v] && tout[v] <= tout[u]; }

    int lca(int u, int v) {
        if (is_ancestor(u, v)) return u;
        if (is_ancestor(v, u)) return v;
        for (int k = LOG - 1; k >= 0; k--)
            if (!is_ancestor(up[k][u], v)) u = up[k][u];
        return up[0][u];
    }

    ll dist(int u, int v) { return root_dist[u] + root_dist[v] - 2 * root_dist[lca(u, v)]; }

    // the compressed tree over `nodes`, as vt[] plus the returned list; the list's front is its root
    vector<int> build(vector<int> nodes) {
        for (int u : touched) vt[u].clear(); // only the nodes the last build actually used
        touched.clear();
        if (nodes.empty()) return {};
        sort(nodes.begin(), nodes.end(), [&](int a, int b) { return tin[a] < tin[b]; });
        int k = (int)nodes.size();
        for (int i = 0; i + 1 < k; i++) nodes.push_back(lca(nodes[i], nodes[i + 1]));
        sort(nodes.begin(), nodes.end(), [&](int a, int b) { return tin[a] < tin[b]; });
        nodes.erase(unique(nodes.begin(), nodes.end()), nodes.end());
        vector<int> stack_{nodes[0]};
        for (int i = 1; i < (int)nodes.size(); i++) {
            while (!is_ancestor(stack_.back(), nodes[i])) stack_.pop_back();
            vt[stack_.back()].push_back(nodes[i]);
            stack_.push_back(nodes[i]);
        }
        touched = nodes;
        return nodes;
    }
};

// Standard problem: q queries, each marking k nodes; report the total edge weight of the smallest
// connected subtree containing all of them
void solve() {
    int n, q;
    cin >> n >> q;
    Virtual_Tree g(n);
    for (int i = 1, u, v; i < n; i++) {
        ll w;
        cin >> u >> v >> w;
        g.add_edge(u, v, w);
    }
    g.init(1);
    while (q--) {
        int k;
        cin >> k;
        vector<int> marked(k);
        for (int &x : marked) cin >> x;
        vector<int> nodes = g.build(marked);
        ll total = 0;
        for (int u : nodes)
            for (int v : g.vt[u]) total += g.dist(u, v);
        cout << total << '\n';
    }
}
