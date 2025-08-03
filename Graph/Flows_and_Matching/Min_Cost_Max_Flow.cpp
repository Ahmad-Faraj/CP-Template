#include <bits/stdc++.h>
using namespace std;

// Complexity: O(min(E^2 *V log V, E logV * flow))
// Works for both directed, undirected and with negative cost too
// For finding maximum cost maximum matching
// add -cost and return -matching()
// fails for negative cycles
// for undirected edges just make the directed flag false

using T = long long;
const T inf = 1LL << 61;
struct MCMF {
    struct edge {
        int u, v;
        T cap, cost;
        int id;
        edge(int _u, int _v, T _cap, T _cost, int _id) {
            u = _u;
            v = _v;
            cap = _cap;
            cost = _cost;
            id = _id;
        }
    };
    int n, s, t, mxid;
    T flow, cost;
    vector<vector<int>> g;
    vector<edge> e;
    vector<T> d, potential, flow_through;
    vector<int> par;
    bool neg;
    MCMF() {}
    MCMF(int _n) { // 0-based indexing
        n = _n + 10;
        g.assign(n, vector<int>());
        neg = false;
        mxid = 0;
    }
    void add_edge(int u, int v, T cap, T cost, int id = -1, bool directed = true) {
        if (cost < 0) neg = true;
        g[u].push_back(e.size());
        e.push_back(edge(u, v, cap, cost, id));
        g[v].push_back(e.size());
        e.push_back(edge(v, u, 0, -cost, -1));
        mxid = max(mxid, id);
        if (!directed) add_edge(v, u, cap, cost, -1, true);
    }
    bool dijkstra() {
        par.assign(n, -1);
        d.assign(n, inf);
        priority_queue<pair<T, T>, vector<pair<T, T>>, greater<pair<T, T>>> q;
        d[s] = 0;
        q.push(pair<T, T>(0, s));
        while (!q.empty()) {
            int u = q.top().second;
            T nw = q.top().first;
            q.pop();
            if (nw != d[u]) continue;
            for (int i = 0; i < (int)g[u].size(); i++) {
                int id = g[u][i];
                int v = e[id].v;
                T cap = e[id].cap;
                T w = e[id].cost + potential[u] - potential[v];
                if (d[u] + w < d[v] && cap > 0) {
                    d[v] = d[u] + w;
                    par[v] = id;
                    q.push(pair<T, T>(d[v], v));
                }
            }
        }
        for (int i = 0; i < n; i++) {
            if (d[i] < inf) d[i] += (potential[i] - potential[s]);
        }
        for (int i = 0; i < n; i++) {
            if (d[i] < inf) potential[i] = d[i];
        }
        return d[t] != inf;
    }

    T send_flow(int v, T cur) {
        if (par[v] == -1) return cur;
        int id = par[v];
        int u = e[id].u;
        T w = e[id].cost;
        T f = send_flow(u, min(cur, e[id].cap));
        cost += f * w;
        e[id].cap -= f;
        e[id ^ 1].cap += f;
        return f;
    }

    // returns {maxflow, mincost}
    pair<T, T> solve(int _s, int _t, T goal = inf) {
        s = _s;
        t = _t;
        flow = 0, cost = 0;
        potential.assign(n, 0);
        if (neg) {
            d.assign(n, inf);
            d[s] = 0;
            bool relax = true;
            for (int i = 0; i < n && relax; i++) {
                relax = false;
                for (int u = 0; u < n; u++) {
                    for (int k = 0; k < (int)g[u].size(); k++) {
                        int id = g[u][k];
                        int v = e[id].v;
                        T cap = e[id].cap, w = e[id].cost;
                        if (d[v] > d[u] + w && cap > 0) {
                            d[v] = d[u] + w;
                            relax = true;
                        }
                    }
                }
            }
            for (int i = 0; i < n; i++)
                if (d[i] < inf) potential[i] = d[i];
        }
        while (flow < goal && dijkstra()) flow += send_flow(t, goal - flow);
        flow_through.assign(mxid + 10, 0);
        for (int u = 0; u < n; u++) {
            for (auto v : g[u]) {
                if (e[v].id >= 0) flow_through[e[v].id] = e[v ^ 1].cap;
            }
        }
        return make_pair(flow, cost);
    }
};

void solve_bipartite() {
    int n;
    cin >> n;
    MCMF F(2 * n + 3);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int k;
            cin >> k;
            F.add_edge(i, j + n, 1, k, i * n + j);
        }
    }
    int s = 2 * n + 1, t = s + 1;
    // link source to u // link v to sink
    for (int i = 0; i < n; i++) {
        F.add_edge(s, i, 1, 0);
        F.add_edge(i + n, t, 1, 0);
    }
    auto cost = F.solve(s, t).second;
    cout << cost << endl;
    long long number_of_matched = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int p = i * n + j;
            if (F.flow_through[p] > 0) {
                cout << i + 1 << " " << j + 1 << endl;
                number_of_matched += F.flow_through[p];
            }
        }
    }
    cout << number_of_matched << endl;
}

void solve_general() {
    int n, m, k;
    cin >> n >> m >> k;
    MCMF F(n + 3);
    int st = 1, en = n + 1;
    for (int i = 1; i <= m; i++) {
        int u, v, r, c;
        cin >> u >> v >> r >> c;
        F.add_edge(u, v, r, c, i);
    }
    F.add_edge(n, en, k, 0, m + 1);
    auto ans = F.solve(st, en);
    if (ans.first < k) {
        cout << -1 << endl;
        return;
    }
    cout << ans.second << '\n';
}