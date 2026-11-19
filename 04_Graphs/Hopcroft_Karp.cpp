// Hopcroft-Karp: maximum matching in a bipartite graph.
// Use when: "assign workers to jobs", "max pairs", Konig cover, and Kuhn's O(V * E) is too slow.
// Usage: HopcroftKarp M(n, m); M.add_edge(u, v); int k = M.maximum_matching(); M.pairs();
// Handles: unequal sides, isolated nodes, multi-edges, n or m = 0. Re-callable after more edges.
// Time: O(E * sqrt(V))
// Indexing: 1-based both sides: left 1..n, right 1..m. Inside, right v is stored as v + n.
// Note: l[u] = v + n (0 = free), r[v + n] = u. dfs is recursive (depth = augmenting path length).

#include <bits/stdc++.h>
using namespace std;

struct HopcroftKarp {
    static const int inf = 1e9;
    int n;
    vector<int> l, r, d;
    vector<vector<int>> g;

    HopcroftKarp(int _n, int _m)
        : n(_n), l(_n + _m + 1), r(_n + _m + 1), d(_n + _m + 1), g(_n + 1) {}

    void add_edge(int u, int v) { g[u].push_back(v + n); } // left u, right v

    bool bfs() {
        queue<int> q;
        for (int u = 1; u <= n; u++) {
            if (!l[u]) d[u] = 0, q.push(u);
            else d[u] = inf;
        }
        d[0] = inf;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : g[u])
                if (d[r[v]] == inf) {
                    d[r[v]] = d[u] + 1;
                    q.push(r[v]);
                }
        }
        return d[0] != inf;
    }
    bool dfs(int u) {
        if (!u) return true;
        for (int v : g[u])
            if (d[r[v]] == d[u] + 1 && dfs(r[v])) {
                l[u] = v;
                r[v] = u;
                return true;
            }
        d[u] = inf;
        return false;
    }
    int maximum_matching() {
        fill(l.begin(), l.end(), 0);
        fill(r.begin(), r.end(), 0);
        int ans = 0;
        while (bfs())
            for (int u = 1; u <= n; u++)
                if (!l[u] && dfs(u)) ans++;
        return ans;
    }
    vector<pair<int, int>> pairs() { // matched (left, right) pairs, right in 1..m
        vector<pair<int, int>> out;
        for (int u = 1; u <= n; u++)
            if (l[u]) out.push_back({u, l[u] - n});
        return out;
    }
};
