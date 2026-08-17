// DSU: union-find with component sizes, plus the actual member list of every component.
// Use when: connectivity, Kruskal, grouping equal elements, "how many components" or "how big is mine".
// Handles: union, connectivity test, component size, component count, and listing each component's members.
// Time: find O(alpha(n)) amortized | union O(alpha(n)) | get_components O(n)
// Indexing: 1-based by default; use DSU<int, 0> for 0-based
// Note: a linked list is kept per component, so get_components() costs O(n) rather than sorting.

#include <bits/stdc++.h>
using namespace std;
#define sz(x) (int)(x).size()

template <typename T = int, int Base = 1> struct DSU {
    vector<T> parent, Gsize, nxt, tail, pos, roots;

    DSU(int MaxNodes) {
        parent = Gsize = roots = tail = pos = nxt = vector<T>(MaxNodes + Base);
        for (int i = Base; i < MaxNodes + Base; i++) {
            parent[i] = roots[i] = pos[i] = tail[i] = i;
            nxt[i] = -1, Gsize[i] = 1;
        }
    }

    T find_leader(int node) { return parent[node] = (parent[node] == node ? node : find_leader(parent[node])); }

    bool is_same_sets(int u, int v) { return find_leader(u) == find_leader(v); }

    void union_sets(int u, int v) {
        int leader_u = find_leader(u), leader_v = find_leader(v);
        if (leader_u == leader_v) return;
        if (Gsize[leader_u] < Gsize[leader_v]) swap(leader_u, leader_v);
        int p = pos[leader_v];
        Gsize[leader_u] += Gsize[leader_v];
        parent[leader_v] = leader_u;
        roots[p] = roots.back();
        pos[roots[p]] = p;
        roots.pop_back();
        nxt[tail[leader_u]] = leader_v;
        tail[leader_u] = tail[leader_v];
    }

    int get_size(int u) { return Gsize[find_leader(u)]; } // size of u's component

    int get_components_number() { return sz(roots) - Base; } // number of components

    vector<vector<int>> get_components() { // every component, as a list of its members
        vector<vector<int>> components;
        for (int root = Base; root < sz(roots); root++) {
            vector<int> component;
            for (int u = roots[root]; ~u; u = nxt[u]) component.push_back(u);
            components.push_back(component);
        }
        return components;
    }
};

// Standard problem: n nodes and m edges; report the component count, then each component's members
void solve() {
    int n, m;
    cin >> n >> m;
    DSU<> dsu(n);
    while (m--) {
        int u, v;
        cin >> u >> v;
        dsu.union_sets(u, v);
    }
    cout << dsu.get_components_number() << '\n';
    for (auto &comp : dsu.get_components()) {
        for (int u : comp) cout << u << ' ';
        cout << '\n';
    }
}
