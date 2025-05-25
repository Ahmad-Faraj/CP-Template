#include <bits/stdc++.h>
using namespace std;

// 0-based indexing if you want 1-based, use OnlineBridges(n + 1)
// Remember the Component graph that we have built in tarjan.
// each node is a 2ECC (2-edge-connected component / strongly connected component)
// and each edge is a bridge.
// Imagine the graph as a forest (a collection of trees), where:
// - Each tree is a connected component.
// - Each node in a tree is a 2ECC.
// - Edges between 2ECCs within a tree are bridges.
// What Happens When We Add an Edge (a,b)?
// There are three possibilities:
//  = a and b are already in the same 2ECC: 
//  - The edge adds a redundant path (part of a cycle), so it’s not a bridge. The number of bridges stays the same.
//  = a and b are in different trees (disconnected components): 
//  - The new edge connects them, becoming a bridge. The number of bridges increases by 1.
//  = a and b are in the same tree but different 2ECCs: 
//  - The edge forms a cycle, merging 2ECCs along that cycle. Bridges on that cycle are no longer bridges, reducing the count (usually by 2 or more).
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
        int comp_size = dsu_cc_size[v];
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
        // compress both paths into LCA, decrement bridges for each old bridge removed
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
        if (u == v) return;              // same 2ECC → nothing changes

        int cu = find_cc(u), cv = find_cc(v);
        if (cu != cv) {
            // new bridge
            bridges++;
            // attach smaller CC under larger
            if (dsu_cc_size[cu] > dsu_cc_size[cv]) {
                swap(u, v);
                swap(cu, cv);
            }
            make_root(u);
            par[u] = v;
            dsu_cc[u] = v;
            dsu_cc_size[cv] += dsu_cc_size[u];
        } else {
            // same CC but different 2ECC → cycle formed
            merge_path(u, v);
        }
    }

    int count_bridges() const {
        return bridges;
    }

    bool is_bridge(int u, int v) {
        u = find_2ecc(u);
        v = find_2ecc(v);
        return u != v;
    }
};

void solve(){
    // Example usage of OnlineBridges
    int n, q;
    cin >> n >> q;
    OnlineBridges ob(n + 1);
    while (q--) {
        int t, u, v;
        cin >> t >> u >> v;
        // u--; v--;
        if (t == 1) {
            ob.add_edge(u, v);
        } else {
            cout << (!ob.is_bridge(u, v) ? "YES" : "NO") << '\n';
        }
    }
}
int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int t = 1;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}