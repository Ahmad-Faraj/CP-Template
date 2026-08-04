#include "../../core.h"

/*
 * Topic: Data Structures - DSU with Rollback
 * Description: A Disjoint Set Union that supports rolling back the last union operations.
 *              Because of rollbacks, path compression cannot be used, so it relies 
 *              strictly on union by size/rank, making operations O(log N).
 */

// 1-based indexing / O(log n)
struct DSU {
    vector<int> parent, gsize;
    stack<pair<int, int>> stk;
    int sets;
    DSU(int n) : sets(n) {
        parent = gsize = vector<int>(n + 1, 1);
        for (int i = 1; i <= n; i++) parent[i] = i;
    }
    int find(int u) {
        while (u != parent[u]) u = parent[u];
        return u;
    }
    void unite(int u, int v) {
        u = find(u), v = find(v);
        if (u == v) {
            stk.push({-1, -1});
            return;
        }
        if (gsize[u] < gsize[v]) swap(u, v);
        parent[v] = u;
        gsize[u] += gsize[v];
        stk.push({u, v});
        sets--;
    }
    void rollback(int k) {

        while (k-- && !stk.empty()) {
            auto [u, v] = stk.top();
            stk.pop();
            if (u == -1) continue;
            parent[v] = v;
            gsize[u] -= gsize[v];
            sets++;
        }
    }
    int components() { return sets; }
    int size(int u) { return gsize[find(u)]; }
    bool same(int u, int v) { return find(u) == find(v); }
};

void solve() {
    int n, m;
    cin >> n >> m;

    DSU dsu(n);
    vector<int> checkpoints;

    while (m--) {
        string cmd;
        cin >> cmd;
        if (cmd == "union") {
            int u, v;
            cin >> u >> v;
            dsu.unite(u, v);
            cout << dsu.components() << '\n';
        } else if (cmd == "persist") {
            checkpoints.push_back(dsu.stk.size());
        } else if (cmd == "rollback") {
            int last = checkpoints.back();
            checkpoints.pop_back();
            dsu.rollback(dsu.stk.size() - last);
            cout << dsu.components() << '\n';
        }
    }
}