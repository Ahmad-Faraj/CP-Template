// DSU: union-find by size with path compression that also lists the members of every set.
// Use when: "merge groups, are u and v together", component sizes, number of components,
//           "print every group" after unions.
// Usage: DSU<> d(n); d.union_sets(u, v); d.is_same_sets(u, v); d.get_size(u);
//        d.get_components_number(); d.get_components();
// Time: union / find almost O(1) amortised | get_components O(n)
// Indexing: Base = 1 (default) means nodes 1..n; DSU<int, 0> for nodes 0..n-1
// Note: find_leader is recursive, but path compression keeps it shallow in practice.

#include <bits/stdc++.h>
using namespace std;

template <typename T = int, int Base = 1> struct DSU {

    vector<T> parent, Gsize, nxt, tail, pos, roots;

    DSU(int MaxNodes) {
        parent = Gsize = roots = tail = pos = nxt = vector<T>(MaxNodes + Base);
        for (int i = Base; i < MaxNodes + Base; i++) {
            parent[i] = roots[i] = pos[i] = tail[i] = i;
            nxt[i] = -1, Gsize[i] = 1;
        }
    }

    T find_leader(int node) {
        return parent[node] = (parent[node] == node ? node : find_leader(parent[node]));
    }

    bool is_same_sets(int u, int v) { return find_leader(u) == find_leader(v); }

    void union_sets(int u, int v) {
        int leader_u = find_leader(u), leader_v = find_leader(v);
        if (leader_u == leader_v) return;
        // make leader_u is the leader with the larger component
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

    void print() {
        for (int root = Base; root < (int)roots.size(); root++) {
            for (int u = roots[root]; ~u; u = nxt[u]) cout << u << " \n"[!~nxt[u]];
        }
    }

    vector<vector<int>> get_components() {
        vector<vector<int>> components;
        for (int root = Base; root < (int)roots.size(); root++) {
            vector<int> component;
            for (int u = roots[root]; ~u; u = nxt[u]) component.push_back(u);
            components.push_back(component);
        }
        return components;
    }

    int get_size(int u) { return Gsize[find_leader(u)]; }

    int get_components_number() { return (int)roots.size() - Base; }
};