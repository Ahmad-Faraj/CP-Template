#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define sz(st) int(st.size())
#define all(st) st.begin(), st.end()

class DSU {
    vector<int> parent, gsize;
    stack<pair<int, int>> stk;
    int sets;
    DSU(int n) : sets(n) {
        parent = gsize = vector<int>(n + 1, 1);
        for (int i = 1; i <= n; i++) parent[i] = i;
    }
    int find(int u) { return parent[u] == u ? u : parent[u] = find(parent[u]); }
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