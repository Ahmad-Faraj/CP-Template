// DSU Rollback: union-find that can undo its most recent unions, newest first.
// Use when: offline dynamic connectivity, divide and conquer over time, "persist / rollback".
// Usage: DSU dsu(n); dsu.join(u, v); int cp = dsu.stk.size(); dsu.rollback(dsu.stk.size() - cp);
// Handles: union, same-set test, component count, component size, joins of already-connected nodes.
// Time: find / join / same / get_size O(log n) | rollback O(k)
// Indexing: 1-based nodes 1..n
// Note: every join() pushes one entry (even a no-op), so rollback(k) undoes exactly k joins.
// Note: find() must never path-compress - that is what keeps rollback possible.

#include <bits/stdc++.h>
using namespace std;

struct DSU {
    vector<int> parent, gsize;
    stack<pair<int, int>> stk; // one entry per join, {-1, -1} when it was a no-op
    int sets;

    DSU(int n) : sets(n) {
        parent = gsize = vector<int>(n + 1, 1);
        for (int i = 1; i <= n; i++) parent[i] = i;
    }

    int find(int u) { // no path compression, on purpose
        while (u != parent[u]) u = parent[u];
        return u;
    }

    void join(int u, int v) {
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

    int get_sets() { return sets; }                        // number of components
    int get_size(int u) { return gsize[find(u)]; }         // size of u's component
    bool same(int u, int v) { return find(u) == find(v); } // are u and v connected
};
