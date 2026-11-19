// Dynamic Connectivity: offline component count / "u, v connected?" while edges come and go.
// Use when: every add / remove / query is known up front ("+ u v", "- u v", "?").
// Usage: Dynamic_Connectivity D(n, T); D.add_edge(t, u, v); D.add_query(t, u, v); D.run();
// Handles: remove_edge(t, u, v), multi-edges (a remove drops one copy), self loops.
// Time: O(T log T log n)
// Indexing: 1-based nodes and times 1..T; time t sees the edges after every op at time <= t
// Note: remove only present edges, issue ops in nondecreasing t.

#include <bits/stdc++.h>
using namespace std;

struct Dynamic_Connectivity {
    struct persistent_dsu {
        struct state {
            int u, v, rnku, rnkv;
        };
        vector<state> st;
        vector<int> par, depth;
        int comp;
        void init(int n) {
            comp = n;
            par.resize(n + 1), depth.assign(n + 1, 1);
            iota(par.begin(), par.end(), 0);
        }
        int root(int x) { return x == par[x] ? x : root(par[x]); }
        bool connected(int x, int y) { return root(x) == root(y); }
        void unite(int x, int y) {
            int rx = root(x), ry = root(y);
            if (rx == ry) return st.push_back({-1, -1, -1, -1});
            st.push_back({rx, ry, depth[rx], depth[ry]});
            if (depth[rx] < depth[ry]) par[rx] = ry;
            else if (depth[ry] < depth[rx]) par[ry] = rx;
            else par[rx] = ry, depth[ry]++;
            comp--;
        }
        void backtrack(int c) {
            while (!st.empty() && c--) {
                state s = st.back();
                st.pop_back();
                if (s.u == -1) continue;
                par[s.u] = s.u, par[s.v] = s.v;
                depth[s.u] = s.rnku, depth[s.v] = s.rnkv;
                comp++;
            }
        }
    } d;

    int T;
    vector<vector<pair<int, int>>> alive;
    map<pair<int, int>, vector<int>> since;
    vector<pair<int, int>> ask;
    vector<int> comps, same; // comps[t] for every t; same[t] = u, v connected, at query times

    Dynamic_Connectivity(int n, int T)
        : T(T), alive(4 * T + 4), ask(T + 1, {0, 0}), comps(T + 1), same(T + 1) {
        d.init(n);
    }

    void upd(int x, int b, int e, int i, int j, const pair<int, int> &p) {
        if (b > j || e < i) return;
        if (b >= i && e <= j) return alive[x].push_back(p);
        int mid = (b + e) >> 1;
        upd(2 * x, b, mid, i, j, p);
        upd(2 * x + 1, mid + 1, e, i, j, p);
    }
    void add_edge(int t, int u, int v) {
        if (u > v) swap(u, v);
        since[{u, v}].push_back(t);
    }
    void remove_edge(int t, int u, int v) {
        if (u > v) swap(u, v);
        auto &s = since[{u, v}];
        upd(1, 1, T, s.back(), t - 1, {u, v});
        s.pop_back();
    }
    void add_query(int t, int u = 0, int v = 0) { ask[t] = {u, v}; } // u = 0: count only

    void query(int x, int b, int e) {
        int prevsz = (int)d.st.size();
        for (auto &p : alive[x]) d.unite(p.first, p.second);
        if (b == e) {
            comps[b] = d.comp;
            same[b] = ask[b].first && d.connected(ask[b].first, ask[b].second);
        } else {
            int mid = (b + e) >> 1;
            query(2 * x, b, mid);
            query(2 * x + 1, mid + 1, e);
        }
        d.backtrack((int)d.st.size() - prevsz);
    }
    void run() { // call once, after every operation
        for (auto &[p, ts] : since)
            for (int t : ts) upd(1, 1, T, t, T, p);
        since.clear();
        if (T > 0) query(1, 1, T);
    }
};
