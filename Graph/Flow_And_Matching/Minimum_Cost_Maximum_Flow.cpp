// Minimum Cost Maximum Flow: pushes the largest possible flow, and among all such flows picks the cheapest.
// Use when: every unit shipped has a price - "cheapest way to move k units", assignment problems, min-cost matching.
// Handles: negative edge costs, directed or undirected edges, a flow cap, per-edge flow recovery. Not negative cycles.
// Time: O(E log V) per augmentation, so O(flow * E log V)
// Indexing: any node ids in 0..n-1 or 1..n; the constructor just needs an upper bound on the node count
// Note: costs may be negative but a negative COST CYCLE breaks the potentials. Pass a goal to stop at k units.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Minimum_Cost_Maximum_Flow {
    static constexpr ll INF = numeric_limits<ll>::max() / 4;

    struct Edge {
        int u, v, id;
        ll cap, cost;
    };

    int n, s = -1, t = -1;
    bool any_negative = false;
    ll total_flow = 0, total_cost = 0;
    vector<vector<int>> g;
    vector<Edge> e;
    vector<ll> dist, potential, flow_through;
    vector<int> par;

    Minimum_Cost_Maximum_Flow(int n) : n(n + 1), g(n + 1) {}

    void add_edge(int u, int v, ll cap, ll cost, int id = -1, bool bidirectional = false) {
        if (cost < 0) any_negative = true;
        g[u].push_back((int)e.size());
        e.push_back({u, v, id, cap, cost});
        g[v].push_back((int)e.size());
        e.push_back({v, u, -1, 0, -cost}); // the residual edge, paired so id ^ 1 finds it
        if (id >= 0 && id >= (int)flow_through.size()) flow_through.resize(id + 1, 0);
        if (bidirectional) add_edge(v, u, cap, cost, -1, false);
    }

    void init_potential() { // Bellman Ford once, so Dijkstra can run on non-negative reduced costs
        potential.assign(n, 0);
        if (!any_negative) return;
        dist.assign(n, INF);
        dist[s] = 0;
        for (int round = 0; round < n; round++) {
            bool relaxed = false;
            for (const Edge &ed : e)
                if (ed.cap > 0 && dist[ed.u] != INF && dist[ed.u] + ed.cost < dist[ed.v]) {
                    dist[ed.v] = dist[ed.u] + ed.cost;
                    relaxed = true;
                }
            if (!relaxed) break;
        }
        for (int i = 0; i < n; i++)
            if (dist[i] != INF) potential[i] = dist[i];
    }

    bool dijkstra() {
        par.assign(n, -1);
        dist.assign(n, INF);
        priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<>> q;
        dist[s] = 0;
        q.push({0, s});
        while (!q.empty()) {
            auto [d, u] = q.top();
            q.pop();
            if (d != dist[u]) continue;
            for (int id : g[u]) {
                const Edge &ed = e[id];
                if (ed.cap <= 0) continue;
                ll w = ed.cost + potential[u] - potential[ed.v];
                if (dist[u] + w < dist[ed.v]) {
                    dist[ed.v] = dist[u] + w;
                    par[ed.v] = id;
                    q.push({dist[ed.v], ed.v});
                }
            }
        }
        for (int i = 0; i < n; i++)
            if (dist[i] != INF) potential[i] += dist[i];
        return dist[t] != INF;
    }

    ll augment(ll limit) { // push as much as the found path allows, and charge its cost
        ll pushed = limit;
        for (int v = t; par[v] != -1; v = e[par[v]].u) pushed = min(pushed, e[par[v]].cap);
        for (int v = t; par[v] != -1; v = e[par[v]].u) {
            e[par[v]].cap -= pushed;
            e[par[v] ^ 1].cap += pushed;
            total_cost += pushed * e[par[v]].cost;
        }
        return pushed;
    }

    pair<ll, ll> run(int source, int sink, ll goal = INF) { // {flow, cost}
        s = source, t = sink;
        total_flow = total_cost = 0;
        init_potential();
        while (total_flow < goal && dijkstra()) {
            ll pushed = augment(goal - total_flow);
            if (pushed <= 0) break;
            total_flow += pushed;
        }
        fill(flow_through.begin(), flow_through.end(), 0);
        for (size_t i = 0; i < e.size(); i += 2)
            if (e[i].id >= 0) flow_through[e[i].id] = e[i ^ 1].cap; // the reverse capacity is the flow pushed
        return {total_flow, total_cost};
    }

    ll flow_on(int id) { return id < (int)flow_through.size() ? flow_through[id] : 0; }
};

// Standard problem: cheapest way to send k units from node 1 to node n, or -1 when k units cannot fit
void solve() {
    int n, m;
    ll k;
    cin >> n >> m >> k;
    Minimum_Cost_Maximum_Flow f(n + 1);
    for (int i = 0, u, v; i < m; i++) {
        ll cap, cost;
        cin >> u >> v >> cap >> cost;
        f.add_edge(u, v, cap, cost, i);
    }
    auto [flow, cost] = f.run(1, n, k);
    cout << (flow < k ? -1 : cost) << '\n';
}
