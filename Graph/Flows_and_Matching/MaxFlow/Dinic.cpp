#include "../../../core.h"
/*
 * Algorithm: Dinic's Algorithm (Max Flow)
 * Purpose: Finds max flow using layered graphs. Extremely fast in practice.
 * Complexity: Time O(V^2 * E) | Space O(V + E)
 * Verified: [CSES 1694 - Download Speed](https://cses.fi/problemset/task/1694)
 */
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
    // Time Complexity: O(1)
    // Space Complexity: O(1)
    // Default constructor.
    Dinic() {}
    // Time Complexity: O(V)
    // Space Complexity: O(V)
    // Initializes the Dinic structure with _n nodes.
    Dinic(int _n) {
        n = _n + 10;
        mxid = 0;
        g.resize(n);
    }

    // Time Complexity: O(1)
    // Space Complexity: O(1)
    // Adds a directed edge with capacity w to the network.
    void add_edge(int u, int v, long long w, int id = -1) {
        edge a = {v, (int)g[v].size(), 0, w, id};
        edge b = {u, (int)g[u].size(), 0, 0, -2}; // for bidirectional edges cap(b) = w
        g[u].emplace_back(a);
        g[v].emplace_back(b);
        mxid = max(mxid, id);
    }

    // Time Complexity: O(V + E)
    // Space Complexity: O(V)
    // Builds a level graph for the residual network using BFS.
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

    // Time Complexity: O(V) per augmenting path
    // Space Complexity: O(V) for recursion stack
    // Finds blocking flows along the layered network using DFS.
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

    // Time Complexity: O(V^2 E) generally, O(E * sqrt(V)) for bipartite graphs
    // Space Complexity: O(V)
    // Computes the maximum flow from source _s to sink _t.
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
                if (e.id >= 0) flow_through[e.id] = (int)e.flow;
        return flow;
    }



};

/*
 * Takes N (number of nodes) and M (number of edges).
 * Followed by M lines of directed edges: u v w (from u to v with capacity w).
 * Gives the maximum flow from node 1 to node N.
 */
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m;
    if (!(cin >> n >> m)) return 0;
    Dinic dinic(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v;
        long long c;
        cin >> u >> v >> c;
        dinic.add_edge(u, v, c);
    }
    cout << dinic.max_flow(1, n) << "\n";
    return 0;
}