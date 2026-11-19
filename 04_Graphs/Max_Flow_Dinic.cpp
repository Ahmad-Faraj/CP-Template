// Max Flow (Dinic): maximum s-t flow, plus the min cut and the flow decomposed into s-t paths.
// Use when: "max throughput", "min edges/capacity to cut s from t", disjoint paths, matching as
//           flow.
// Usage: Dinic F(n); F.add_edge(u, v, cap, id); ll f = F.max_flow(s, t); F.get_min_cut();
// Handles: multi-edges, self loops, 0 capacities, ids for reading flow per edge (flow_through[id]).
// Time: O(V^2 * E); O(E * sqrt(V)) on unit bipartite matching graphs
// Indexing: any; nodes 0..n+9 are valid (the constructor pads by 10). Edge ids are >= 0.
// Note: s != t. Undirected edge u-v: add_edge(u, v, c) and add_edge(v, u, c). A second max_flow
// call only adds the extra flow. get_paths() eats the flow (call it last); its paths are walks
// that may repeat a vertex. dfs and mark_reachable recurse up to V deep.

#include <bits/stdc++.h>
using namespace std;

struct Dinic {
    static constexpr long long inf = 1LL << 61;
    struct edge {
        int to, rev;
        long long flow, w;
        int id;
    };
    int n, s, t, mxid;
    vector<int> d, done;
    vector<long long> flow_through; // flow_through[id] = flow on the edge added with that id
    vector<vector<edge>> g;
    Dinic() {}
    Dinic(int _n) {
        n = _n + 10;
        mxid = 0;
        g.resize(n);
    }

    void add_edge(int u, int v, long long w, int id = -1) {
        edge a = {v, (int)g[v].size(), 0, w, id};
        edge b = {u, (int)g[u].size(), 0, 0, -2};
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
        for (auto &e : g[u])
            if (!vis[e.to] && e.flow < e.w) mark_reachable(e.to);
    }

    // after max_flow: the saturated edges (u, v) from the s side to the t side; their caps sum to
    // the max flow. vis[u] = true marks the s side.
    vector<pair<int, int>> get_min_cut() {
        vis.assign(n, false);
        mark_reachable(s);
        vector<pair<int, int>> cut;
        for (int u = 0; u < n; ++u) {
            if (!vis[u]) continue;
            for (auto &e : g[u])
                if (!vis[e.to] && e.w > 0) cut.emplace_back(u, e.to);
        }
        return cut;
    }

    // after max_flow: splits the flow into unit s-t walks (a cap-c edge can appear in c of them),
    // so with unit capacities these are the max set of edge-disjoint paths. Destroys the flow.
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
