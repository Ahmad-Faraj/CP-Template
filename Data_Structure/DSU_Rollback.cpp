// DSU with Rollback: union-find that can undo its last k unions.
// Use when: offline dynamic connectivity, divide and conquer over time, "try this edge then take it back".
// Handles: union, connectivity test, component count, component size, rollback of any number of unions.
// Time: find O(log n) | join O(log n) | rollback O(k)
// Indexing: 1-based
// Note: find() must never path-compress - that is what makes rollback possible, and why it is O(log n).

#include <bits/stdc++.h>
using namespace std;

struct DSU {
    vector<int> parent, gsize;
    stack<pair<int, int>> stk; // one entry per join, {-1,-1} when the join was a no-op
    int sets;

    DSU(int n) : sets(n) {
        parent = gsize = vector<int>(n + 1, 1);
        for (int i = 1; i <= n; i++) parent[i] = i;
    }

    int find(int u) { // no path compression, on purpose
        while (u != parent[u]) u = parent[u];
        return u;
    }

    void join(int u, int v) { // merge the components of u and v
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

    void rollback(int k) { // undo the last k calls to join()
        while (k-- && !stk.empty()) {
            auto [u, v] = stk.top();
            stk.pop();
            if (u == -1) continue;
            parent[v] = v;
            gsize[u] -= gsize[v];
            sets++;
        }
    }

    int get_sets() { return sets; }                          // number of components
    int get_size(int u) { return gsize[find(u)]; }           // size of u's component
    bool same(int u, int v) { return find(u) == find(v); }   // are u and v connected
};

// Standard problem: "union u v" merges, "persist" marks a checkpoint, "rollback" returns to the last checkpoint
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
            dsu.join(u, v);
            cout << dsu.get_sets() << '\n';
        } else if (cmd == "persist") {
            checkpoints.push_back((int)dsu.stk.size());
        } else {
            int last = checkpoints.back();
            checkpoints.pop_back();
            dsu.rollback((int)dsu.stk.size() - last);
            cout << dsu.get_sets() << '\n';
        }
    }
}
