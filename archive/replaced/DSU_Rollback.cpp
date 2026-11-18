#include <bits/stdc++.h>
using namespace std;

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
    int get_sets() { return sets; }
    int get_size(int u) { return gsize[find(u)]; }
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
            dsu.join(u, v);
            cout << dsu.get_sets() << '\n';
        } else if (cmd == "persist") {
            checkpoints.push_back(dsu.stk.size());
        } else if (cmd == "rollback") {
            int last = checkpoints.back();
            checkpoints.pop_back();
            dsu.rollback(dsu.stk.size() - last);
            cout << dsu.get_sets() << '\n';
        }
    }
}