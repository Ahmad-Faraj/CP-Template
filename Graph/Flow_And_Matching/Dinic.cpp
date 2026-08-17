// Dinic: maximum flow from source to sink, and the minimum cut that equals it.
// Use when: "maximum throughput", "fewest edges to cut", disjoint paths, bipartite matching modelled as flow.
// Handles: directed or undirected edges, multi-edges, disconnected graphs, and recovering the flow on each edge.
// Time: O(V^2 * E), and O(E * sqrt(E)) on unit capacities
// Indexing: any node ids in 0..n-1 or 1..n; the constructor just needs an upper bound on the node count
// Note: call max_flow once per source/sink pair. min_cut_edges and flow_on need the flow already computed.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Dinic {
    static constexpr ll INF = numeric_limits<ll>::max() / 4;

    struct Edge {
        int to, rev, id;
        ll flow, cap;
    };

    int n, s = -1, t = -1;
    vector<vector<Edge>> g;
    vector<int> level, iter;
    vector<ll> flow_through; // indexed by the id given to add_edge

    Dinic(int n) : n(n + 1), g(n + 1), level(n + 1), iter(n + 1) {}

    // id lets you ask for this edge's flow later; bidirectional gives the reverse the same capacity
    void add_edge(int u, int v, ll cap, int id = -1, bool bidirectional = false) {
        g[u].push_back({v, (int)g[v].size(), id, 0, cap});
        g[v].push_back({u, (int)g[u].size() - 1, -1, 0, bidirectional ? cap : 0});
        if (id >= 0 && id >= (int)flow_through.size()) flow_through.resize(id + 1, 0);
    }

    bool bfs() {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        level[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (const Edge &e : g[u])
                if (level[e.to] == -1 && e.flow < e.cap) {
                    level[e.to] = level[u] + 1;
                    q.push(e.to);
                }
        }
        return level[t] != -1;
    }

    ll dfs(int u, ll limit) {
        if (u == t) return limit;
        for (int &i = iter[u]; i < (int)g[u].size(); i++) {
            Edge &e = g[u][i];
            if (e.flow >= e.cap || level[e.to] != level[u] + 1) continue;
            ll pushed = dfs(e.to, min(limit, e.cap - e.flow));
            if (pushed > 0) {
                e.flow += pushed;
                g[e.to][e.rev].flow -= pushed;
                return pushed;
            }
        }
        return 0;
    }

    ll max_flow(int source, int sink) {
        s = source, t = sink;
        ll total = 0;
        while (bfs()) {
            fill(iter.begin(), iter.end(), 0);
            while (ll pushed = dfs(s, INF)) total += pushed;
        }
        fill(flow_through.begin(), flow_through.end(), 0);
        for (int u = 0; u < n; u++)
            for (const Edge &e : g[u])
                if (e.id >= 0) flow_through[e.id] = e.flow;
        return total;
    }

    ll flow_on(int id) { return id < (int)flow_through.size() ? flow_through[id] : 0; }

    vector<char> min_cut_side() { // 1 for the nodes still reachable from s in the residual graph
        vector<char> side(n, 0);
        if (s == -1) return side;
        vector<int> stack_{s};
        side[s] = 1;
        while (!stack_.empty()) { // iterative, so a long residual path cannot blow the stack
            int u = stack_.back();
            stack_.pop_back();
            for (const Edge &e : g[u])
                if (!side[e.to] && e.flow < e.cap) side[e.to] = 1, stack_.push_back(e.to);
        }
        return side;
    }

    vector<pair<int, int>> min_cut_edges() { // the saturated edges crossing the cut
        vector<char> side = min_cut_side();
        vector<pair<int, int>> cut;
        for (int u = 0; u < n; u++) {
            if (!side[u]) continue;
            for (const Edge &e : g[u])
                if (!side[e.to] && e.cap > 0) cut.push_back({u, e.to});
        }
        return cut;
    }

    // one path per unit of flow, for unit-capacity graphs; works on a copy so the flow survives
    vector<vector<int>> decompose_paths() {
        vector<vector<Edge>> copy = g;
        vector<vector<int>> paths;
        while (true) {
            vector<int> path{s};
            int u = s;
            bool advanced = true;
            while (u != t && advanced) {
                advanced = false;
                for (Edge &e : copy[u])
                    if (e.flow > 0) {
                        e.flow--;
                        u = e.to;
                        path.push_back(u);
                        advanced = true;
                        break;
                    }
            }
            if (u != t) break;
            paths.push_back(path);
        }
        return paths;
    }
};

// Standard problem: CSES Download Speed - the maximum flow from node 1 to node n
void solve() {
    int n, m;
    cin >> n >> m;
    Dinic f(n + 1);
    for (int i = 0, u, v; i < m; i++) {
        ll c;
        cin >> u >> v >> c;
        f.add_edge(u, v, c, i);
    }
    cout << f.max_flow(1, n) << '\n';
}
