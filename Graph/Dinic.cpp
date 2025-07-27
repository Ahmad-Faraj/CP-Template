#include <bits/stdc++.h>
using namespace std;
const long long inf = 1LL << 61;

struct Dinic {
    struct edge {
        int to, rev;
        long long flow, w;
        int id;
    };
    int n, s, t, mxid;
    vector<int> d, flow_through;
    vector<int> done;
    vector<vector<edge>> g;
    Dinic() {}
    Dinic(int _n) {
        n = _n + 10;
        mxid = 0;
        g.resize(n);
    }

    void add_edge(int u, int v, long long w, int id = -1) {
        edge a = {v, (int)g[v].size(), 0, w, id};
        edge b = {u, (int)g[u].size(), 0, 0, -2}; // for bidirectional edges cap(b) = w
        g[u].emplace_back(a);
        g[v].emplace_back(b);
        mxid = max(mxid, id);
    }

    bool bfs() {
        d.assign(n, -1);
        d[s] = 0;
        queue<int> q;
        q.push(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (auto &e : g[u]) {
                int v = e.to;
                if (d[v] == -1 && e.flow < e.w) d[v] = d[u] + 1, q.push(v);
            }
        }
        return d[t] != -1;
    }

    long long dfs(int u, long long flow) {
        if (u == t) return flow;
        for (int &i = done[u]; i < (int)g[u].size(); i++) {
            edge &e = g[u][i];
            if (e.w <= e.flow) continue;
            int v = e.to;
            if (d[v] == d[u] + 1) {
                long long nw = dfs(v, min(flow, e.w - e.flow));
                if (nw > 0) {
                    e.flow += nw;
                    g[v][e.rev].flow -= nw;
                    return nw;
                }
            }
        }
        return 0;
    }

    long long max_flow(int _s, int _t) {
        s = _s;
        t = _t;
        long long flow = 0;
        while (bfs()) {
            done.assign(n, 0);
            while (long long nw = dfs(s, inf)) flow += nw;
        }
        flow_through.assign(mxid + 10, 0);
        for (int i = 0; i < n; i++)
            for (auto e : g[i])
                if (e.id >= 0) flow_through[e.id] = e.flow;
        return flow;
    }

    vector<bool> vis;
    void mark_reachable(int u) {
        vis[u] = true;
        for (auto &e : g[u]) {
            if (!vis[e.to] && e.flow < e.w) // residual capacity exists
                mark_reachable(e.to);
        }
    }

    // min-cut == max-flow
    vector<pair<int, int>> get_min_cut() {
        vis.assign(n, false);
        mark_reachable(s);
        vector<pair<int, int>> cut;
        for (int u = 0; u < n; ++u) {
            if (!vis[u]) continue;
            for (auto &e : g[u]) {
                if (!vis[e.to] && e.w > 0) {
                    cut.emplace_back(u, e.to);
                }
            }
        }
        return cut;
    }

    // Extract edge-disjoint paths
    vector<vector<int>> get_paths() {
        vector<vector<int>> res;
        while (1) {
            vector<int> path;
            int u = s;
            path.push_back(u);
            while (u != t) {
                bool found = false;
                for (auto &e : g[u]) {
                    if (e.flow > 0) {
                        e.flow--;
                        u = e.to;
                        path.push_back(u);
                        found = true;
                        break;
                    }
                }
                if (!found) break;
            }
            if (path.back() != t) break;
            res.push_back(path);
        }
        return res;
    }
};

void solve_bipartite() {
    int n, m, p;
    cin >> n >> m >> p;
    int st = 0, en = n + m + 1;
    Dinic F(n + m + 2);
    vector<pair<int, int>> edges(p + 1);

    // source -> u
    for (int i = 1; i <= n; i++) F.add_edge(st, i, 1);
    // v -> sink
    for (int i = 1; i <= m; i++) F.add_edge(i + n, en, 1);

    for (int i = 1; i <= p; i++) {
        int u, v;
        cin >> u >> v;
        edges[i] = {u, v};
        F.add_edge(u, v + n, 1, i);
    }

    long long flow = F.max_flow(st, en);
    cout << flow << endl;

    // all matched edges
    for (int i = 1; i <= p; i++) {
        if (F.flow_through[i] == 1) {
            cout << edges[i].first << " " << edges[i].second << endl;
        }
    }
}

void solve_general() {
    int n, m;
    cin >> n >> m;
    int st = 1, en = n;
    Dinic F(n + 1);
    vector<pair<int, int>> edges(m + 1);
    for (int i = 1; i <= m; i++) {
        int u, v;
        cin >> u >> v;
        edges[i] = {u, v};
        F.add_edge(u, v, 1, i);
    }

    long long flow = F.max_flow(st, en);
    cout << flow << endl;

    auto min_cut = F.get_min_cut();
    for (auto [u, v] : min_cut) {
        cout << u << " " << v << endl;
    }

    auto paths = F.get_paths();
    for (auto &p : paths) {
        cout << p.size() << endl;
        for (int x : p) cout << x << " ";
        cout << endl;
    }
}