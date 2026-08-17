// Bridges Online: keeps the bridge count correct while edges are added one at a time, with no rebuild.
// Use when: edges arrive as queries asking "how many bridges now" or "are u and v still two-edge-connected".
// Handles: added edges only - never deletions. Multi-edges, self loops, and a graph that starts disconnected.
// Time: O(n + m log n) overall, amortized nearly constant per edge
// Indexing: 1-based nodes; the constructor takes the node count
// Note: additions only. If edges are also removed, go offline with Range_Query/Dynamic_Connectivity.cpp instead.

#include <bits/stdc++.h>
using namespace std;

struct Bridges_Online {
    int n, bridges = 0, lca_iter = 0;
    vector<int> par, dsu_2ecc, dsu_cc, cc_size, last_visit;

    Bridges_Online(int n)
        : n(n), par(n + 1, -1), dsu_2ecc(n + 1), dsu_cc(n + 1), cc_size(n + 1, 1), last_visit(n + 1, 0) {
        for (int i = 0; i <= n; i++) dsu_2ecc[i] = dsu_cc[i] = i;
    }

    int find_2ecc(int v) { // iterative, so a long initial chain cannot blow the stack
        if (v == -1) return -1;
        int root = v;
        while (dsu_2ecc[root] != root) root = dsu_2ecc[root];
        while (dsu_2ecc[v] != root) {
            int nxt = dsu_2ecc[v];
            dsu_2ecc[v] = root;
            v = nxt;
        }
        return root;
    }

    int find_cc(int v) {
        v = find_2ecc(v);
        int root = v;
        while (dsu_cc[root] != root) root = dsu_cc[root];
        while (dsu_cc[v] != root) {
            int nxt = dsu_cc[v];
            dsu_cc[v] = root;
            v = nxt;
        }
        return root;
    }

    void make_root(int v) { // reverse the parent chain so v becomes its component's root
        int root = find_2ecc(v);
        int cur = root, child = -1, old_root = root;
        while (cur != -1) {
            int p = find_2ecc(par[cur]);
            par[cur] = child;
            dsu_cc[cur] = root;
            child = cur;
            old_root = cur;
            cur = p;
        }
        cc_size[root] = cc_size[old_root]; // the size lived on the OLD root, not on v
    }

    void merge_path(int a, int b) { // everything on the a..b tree path becomes one 2ecc
        ++lca_iter;
        vector<int> path_a, path_b;
        int lca = -1;
        while (lca == -1) {
            if (a != -1) {
                a = find_2ecc(a);
                path_a.push_back(a);
                if (last_visit[a] == lca_iter) {
                    lca = a;
                    break;
                }
                last_visit[a] = lca_iter;
                a = par[a];
            }
            if (b != -1) {
                b = find_2ecc(b);
                path_b.push_back(b);
                if (last_visit[b] == lca_iter) {
                    lca = b;
                    break;
                }
                last_visit[b] = lca_iter;
                b = par[b];
            }
        }
        for (int v : path_a) {
            dsu_2ecc[v] = lca;
            if (v == lca) break;
            --bridges;
        }
        for (int v : path_b) {
            dsu_2ecc[v] = lca;
            if (v == lca) break;
            --bridges;
        }
    }

    void add_edge(int u, int v) {
        u = find_2ecc(u), v = find_2ecc(v);
        if (u == v) return; // already two-edge-connected, or a self loop
        int cu = find_cc(u), cv = find_cc(v);
        if (cu != cv) { // joins two components, so the new edge is itself a bridge
            ++bridges;
            if (cc_size[cu] > cc_size[cv]) swap(u, v), swap(cu, cv);
            make_root(u);
            par[u] = v;
            dsu_cc[u] = cv;
            cc_size[cv] += cc_size[cu];
        } else
            merge_path(u, v);
    }

    int bridge_count() const { return bridges; }
    bool two_edge_connected(int u, int v) { return find_2ecc(u) == find_2ecc(v); }
    bool connected(int u, int v) { return find_cc(u) == find_cc(v); }
    bool is_bridge(int u, int v) { return find_2ecc(u) != find_2ecc(v); } // for an edge u-v already added
};

// Standard problem: after each added edge, report how many bridges the graph has
void solve() {
    int n, m;
    cin >> n >> m;
    Bridges_Online ob(n);
    for (int i = 0, u, v; i < m; i++) {
        cin >> u >> v;
        ob.add_edge(u, v);
        cout << ob.bridge_count() << '\n';
    }
}
