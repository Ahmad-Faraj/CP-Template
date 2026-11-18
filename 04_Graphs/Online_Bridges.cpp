// Online Bridges: keeps the number of bridges while undirected edges are added one at a time.
// Use when: "after each new road, how many critical roads are there", incremental 2-edge-connectivity.
// Usage: OnlineBridges ob(n); ob.add_edge(u, v); ob.count_bridges(); ob.is_bridge(u, v);
// Handles: multi-edges (a parallel edge kills the bridge), self loops (ignored), disconnected graphs.
// Not: edge deletions (go offline with Dynamic_Connectivity.cpp).
// Time: O(n log n + m) total, amortized
// Indexing: 0-based nodes 0..n-1; for 1-based use OnlineBridges(n + 1)
// Note: is_bridge(u, v) assumes u-v is an edge already added (it is "different 2-edge-connected
// components"). find_2ecc / find_cc are recursive DSU finds (depth can reach n).

#include <bits/stdc++.h>
using namespace std;

struct OnlineBridges {
    int n;
    vector<int> par, dsu_2ecc, dsu_cc, dsu_cc_size;
    vector<int> last_visit;
    int lca_iter = 0;
    int bridges = 0;

    OnlineBridges(int _n) : n(_n) {
        par.assign(n, -1);
        dsu_2ecc.resize(n);
        dsu_cc.resize(n);
        dsu_cc_size.resize(n);
        last_visit.assign(n, 0);
        for (int i = 0; i < n; i++) {
            dsu_2ecc[i] = i;
            dsu_cc[i] = i;
            dsu_cc_size[i] = 1;
        }
    }

    int find_2ecc(int v) {
        if (v == -1) return -1;
        return dsu_2ecc[v] == v ? v : dsu_2ecc[v] = find_2ecc(dsu_2ecc[v]);
    }

    int find_cc(int v) {
        v = find_2ecc(v);
        return dsu_cc[v] == v ? v : dsu_cc[v] = find_cc(dsu_cc[v]);
    }

    void make_root(int v) {
        v = find_2ecc(v);
        int root = v, child = -1;
        int comp_size = dsu_cc_size[find_cc(v)]; // the size lives on the old root
        while (v != -1) {
            int p = find_2ecc(par[v]);
            par[v] = child;
            dsu_cc[v] = root;
            child = v;
            v = p;
        }
        dsu_cc_size[root] = comp_size;
    }

    void merge_path(int a, int b) {
        lca_iter++;
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
            bridges--;
        }
        for (int v : path_b) {
            dsu_2ecc[v] = lca;
            if (v == lca) break;
            bridges--;
        }
    }

    void add_edge(int u, int v) {
        u = find_2ecc(u);
        v = find_2ecc(v);
        if (u == v) return;

        int cu = find_cc(u), cv = find_cc(v);
        if (cu != cv) {
            bridges++;
            if (dsu_cc_size[cu] > dsu_cc_size[cv]) {
                swap(u, v);
                swap(cu, cv);
            }
            make_root(u);
            par[u] = v;
            dsu_cc[u] = v;
            dsu_cc_size[cv] += dsu_cc_size[u];
        } else {
            merge_path(u, v);
        }
    }

    int count_bridges() const { return bridges; }

    bool is_bridge(int u, int v) {
        u = find_2ecc(u);
        v = find_2ecc(v);
        return u != v;
    }
};
