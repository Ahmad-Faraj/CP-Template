// Kruskal Reconstruction Tree: rebuilds Kruskal's merges as a binary tree whose internal nodes carry edge weights.
// Use when: "reachable using only edges of weight <= w", or "minimise the largest edge on a path u..v".
// Handles: any graph, multi-edges, self loops, disconnected input (one KRT root per component); NOT edge updates.
// Time: build O(m log m) | component(u, w) and max_edge O(log n) each
// Indexing: 1-based original vertices are the leaves; internal nodes are n+1 .. total-1, and node 0 is a sentinel
// Note: build sorts edges ascending, so max_edge gives the minimax path weight. Sort descending for the minimax min.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Kruskal_Reconstruction_Tree {
    int n, total, LOG;
    vector<int> dsu, lo, hi, leaf_order; // lo/hi index leaf_order, so a subtree is a contiguous vertex range
    vector<ll> val;                      // the edge weight that created this internal node; 0 at a leaf
    vector<vector<int>> children, up;
    vector<int> roots; // one per connected component of the input

    int find(int x) { return dsu[x] == x ? x : dsu[x] = find(dsu[x]); }

    // edges as {weight, u, v}; the graph may be disconnected
    Kruskal_Reconstruction_Tree(int n, vector<array<ll, 3>> edges) : n(n) {
        sort(edges.begin(), edges.end());
        total = 2 * n; // leaves 1..n, internal nodes from n+1, all under a capacity of 2n-1 plus the sentinel
        dsu.resize(total);
        iota(dsu.begin(), dsu.end(), 0);
        val.assign(total, 0);
        children.assign(total, {});
        int next_node = n;
        for (auto &e : edges) {
            int a = find((int)e[1]), b = find((int)e[2]);
            if (a == b) continue; // already joined by a lighter edge, so it creates no node
            int id = ++next_node;
            dsu[a] = dsu[b] = dsu[id] = id;
            val[id] = e[0];
            children[id] = {a, b};
        }
        total = next_node + 1;
        val.resize(total), children.resize(total), dsu.resize(total);
        val[0] = LLONG_MAX; // sentinel above every root, so lifting always terminates

        LOG = 1;
        while ((1 << LOG) <= total) LOG++;
        up.assign(LOG, vector<int>(total, 0));
        lo.assign(total, 0), hi.assign(total, 0);
        vector<char> is_child(total, 0);
        for (int x = 1; x < total; x++)
            for (int c : children[x]) is_child[c] = 1;
        for (int x = 1; x < total; x++)
            if (!is_child[x]) roots.push_back(x);

        for (int r : roots) { // iterative, since a chain of merges is as deep as the node count
            vector<pair<int, int>> stack_{{r, 0}};
            up[0][r] = 0;
            while (!stack_.empty()) {
                auto [u, state] = stack_.back();
                stack_.pop_back();
                if (state) {
                    hi[u] = (int)leaf_order.size();
                    continue;
                }
                lo[u] = (int)leaf_order.size();
                if (children[u].empty()) leaf_order.push_back(u);
                stack_.push_back({u, 1});
                for (int k = 1; k < LOG; k++) up[k][u] = up[k - 1][up[k - 1][u]];
                for (int c : children[u]) up[0][c] = u, stack_.push_back({c, 0});
            }
        }
    }

    // highest ancestor of u reachable without exceeding weight w; its leaves are exactly what u can reach
    int component(int u, ll w) {
        for (int k = LOG - 1; k >= 0; k--)
            if (up[k][u] && val[up[k][u]] <= w) u = up[k][u];
        return u;
    }

    int component_size(int u, ll w) { // how many original vertices u reaches using edges of weight <= w
        int x = component(u, w);
        return hi[x] - lo[x];
    }

    vector<int> vertices(int x) { // the original vertices in node x's subtree
        return vector<int>(leaf_order.begin() + lo[x], leaf_order.begin() + hi[x]);
    }

    bool connected(int u, int v) { return find(u) == find(v); }

    // the smallest w for which u and v are connected using edges of weight <= w; LLONG_MAX if never
    ll max_edge(int u, int v) {
        if (!connected(u, v)) return LLONG_MAX;
        for (int k = LOG - 1; k >= 0; k--)
            if (up[k][u] && !(lo[up[k][u]] <= lo[v] && hi[v] <= hi[up[k][u]])) u = up[k][u];
        return (lo[u] <= lo[v] && hi[v] <= hi[u]) ? val[u] : val[up[0][u]];
    }
};

// Standard problem: n vertices, m weighted edges, q queries "u w" - how many vertices are reachable
// from u using only edges of weight at most w
void solve() {
    int n, m, q;
    cin >> n >> m >> q;
    vector<array<ll, 3>> edges(m);
    for (auto &e : edges) cin >> e[1] >> e[2] >> e[0];
    Kruskal_Reconstruction_Tree krt(n, edges);
    while (q--) {
        int u;
        ll w;
        cin >> u >> w;
        cout << krt.component_size(u, w) << '\n';
    }
}
