#include "../../../core.h"
/*
 * Topic: Online Bridges
 * Description: Dynamically tracks the number of bridges as edges are added.
 *              Uses a combination of DSUs for 2-ECCs and connected components.
 * 
 * Important Facts:
 * - 0-based indexing for nodes by default.
 * - Time Complexity: O(log V) amortized per edge addition.
 * - Space Complexity: O(V)
 */

struct OnlineBridges {
    int n;
    vector<int> par, dsu_2ecc, dsu_cc, dsu_cc_size;
    vector<int> last_visit;
    int lca_iter = 0;
    int bridges = 0;

    // Time Complexity: O(V)
    // Space Complexity: O(V)
    // Initializes structures for an online graph with up to _n vertices.
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

    // Time Complexity: O(alpha(V))
    // Space Complexity: O(1) auxiliary
    // Internal function to find the 2-edge-connected component root.
    int find_2ecc(int v) {
        if (v == -1) return -1;
        return dsu_2ecc[v] == v ? v : dsu_2ecc[v] = find_2ecc(dsu_2ecc[v]);
    }

    // Time Complexity: O(alpha(V))
    // Space Complexity: O(1) auxiliary
    // Internal function to find the connected component root.
    int find_cc(int v) {
        v = find_2ecc(v);
        return dsu_cc[v] == v ? v : dsu_cc[v] = find_cc(dsu_cc[v]);
    }

    // Time Complexity: O(path length)
    // Space Complexity: O(1) auxiliary
    // Reroots the component tree at vertex v.
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

    // Time Complexity: O(path length)
    // Space Complexity: O(path length)
    // Merges the path between a and b into a single 2-edge-connected component.
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

    // Time Complexity: O(log V) amortized
    // Space Complexity: O(path length) temporary
    // Adds an undirected edge between u and v and updates the number of bridges.
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

    // Time Complexity: O(1)
    // Space Complexity: O(1)
    // Returns the current total number of bridges in the graph.
    int count_bridges() const { return bridges; }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m;
    if (cin >> n >> m) {
        OnlineBridges ob(n + 1);
        for (int i = 0; i < m; i++) {
            int u, v;
            cin >> u >> v;
            ob.add_edge(u, v);
        }
        int q;
        if (cin >> q) {
            while (q--) {
                int u, v;
                cin >> u >> v;
                ob.add_edge(u, v);
                cout << ob.count_bridges() << "\n";
            }
        } else {
            cout << ob.count_bridges() << "\n";
        }
    }
    return 0;
}
