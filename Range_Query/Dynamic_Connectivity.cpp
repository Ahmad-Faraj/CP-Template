// Offline Dynamic Connectivity: component count over time while edges are added and removed.
// Use when: edges appear and disappear and you must answer connectivity at given moments, all queries known up front.
// Handles: edge add, edge remove, component count at any time step. Offline only. Duplicate edges are not tracked.
// Time: O(m log m * alpha) over m operations
// Indexing: 1-based nodes, 1-based time steps
// Note: rollback DSU, so root() must never path-compress. Call run() once - it answers every query in one sweep.

#include <bits/stdc++.h>
using namespace std;

struct RollbackDSU {
    struct state {
        int u, v, rnku, rnkv;
        state() : u(-1), v(-1), rnku(-1), rnkv(-1) {}
        state(int _u, int _rnku, int _v, int _rnkv) : u(_u), v(_v), rnku(_rnku), rnkv(_rnkv) {}
    };

    stack<state> st;
    vector<int> par, depth;
    int comp = 0;

    void init(int n) {
        comp = n;
        par.resize(n + 1), depth.resize(n + 1);
        for (int i = 0; i <= n; i++) par[i] = i, depth[i] = 1;
    }

    int root(int x) { // no path compression - rollback depends on the parent array staying as written
        while (x != par[x]) x = par[x];
        return x;
    }

    bool connected(int x, int y) { return root(x) == root(y); }

    void unite(int x, int y) {
        int rx = root(x), ry = root(y);
        if (rx == ry) {
            st.push(state());
            return;
        }
        if (depth[rx] < depth[ry])
            par[rx] = ry;
        else if (depth[ry] < depth[rx])
            par[ry] = rx;
        else
            par[rx] = ry, depth[rx]++;
        comp--;
        st.push(state(rx, depth[rx], ry, depth[ry]));
    }

    void backtrack(int c) { // undo the last c unions
        while (!st.empty() && c) {
            if (st.top().u == -1) {
                st.pop();
                c--;
                continue;
            }
            par[st.top().u] = st.top().u;
            par[st.top().v] = st.top().v;
            depth[st.top().u] = st.top().rnku;
            depth[st.top().v] = st.top().rnkv;
            st.pop();
            c--;
            comp++;
        }
    }
};

struct DynamicConnectivity {
    int n, m;
    RollbackDSU d;
    vector<vector<pair<int, int>>> alive; // edges alive over each time segment
    vector<int> ans;
    vector<bool> isquery;
    map<pair<int, int>, int> since; // edge -> the time it was added
    set<pair<int, int>> open;

    DynamicConnectivity(int nodes, int steps)
        : n(nodes), m(steps), alive(4 * steps + 4), ans(steps + 1), isquery(steps + 1) {
        d.init(n);
    }

    void seg_add(int node, int b, int e, int i, int j, pair<int, int> &p) {
        if (b > j || e < i) return;
        if (b >= i && e <= j) {
            alive[node].push_back(p);
            return;
        }
        int mid = (b + e) >> 1;
        seg_add(2 * node, b, mid, i, j, p);
        seg_add(2 * node + 1, mid + 1, e, i, j, p);
    }

    pair<int, int> norm(int u, int v) { return u < v ? make_pair(u, v) : make_pair(v, u); }

    void add_edge(int t, int u, int v) { since[norm(u, v)] = t, open.insert(norm(u, v)); } // edge appears at time t

    void remove_edge(int t, int u, int v) { // edge disappears at time t
        auto p = norm(u, v);
        open.erase(p);
        seg_add(1, 1, m, since[p], t - 1, p);
    }

    void add_query(int t) { isquery[t] = true; } // ask the component count at time t

    void sweep(int node, int b, int e) {
        if (b > e) return;
        int prev = (int)d.st.size();
        for (auto p : alive[node]) d.unite(p.first, p.second);
        if (b == e) {
            ans[b] = d.comp;
            d.backtrack((int)d.st.size() - prev);
            return;
        }
        int mid = (b + e) >> 1;
        sweep(2 * node, b, mid);
        sweep(2 * node + 1, mid + 1, e);
        d.backtrack((int)d.st.size() - prev);
    }

    vector<int> run() { // ans[t] = component count at time t, for every t marked by add_query
        for (auto p : open) seg_add(1, 1, m, since[p], m, p);
        open.clear();
        sweep(1, 1, m);
        return ans;
    }
};

// Standard problem: "+ u v" adds an edge, "- u v" removes one, "?" asks the component count (CF Gym 100551A)
void solve() {
    int n, m;
    cin >> n >> m;
    DynamicConnectivity dc(n, m);
    for (int i = 1; i <= m; i++) {
        string ty;
        cin >> ty;
        if (ty == "?") {
            dc.add_query(i);
        } else {
            int u, v;
            cin >> u >> v;
            if (ty == "+")
                dc.add_edge(i, u, v);
            else
                dc.remove_edge(i, u, v);
        }
    }
    auto ans = dc.run();
    for (int i = 1; i <= m; i++)
        if (dc.isquery[i]) cout << ans[i] << '\n';
}
