// Bellman Ford SPFA: the queue-based Bellman Ford - only re-examines a node when its distance actually improved.
// Use when: you need negative weights but O(n*m) is too slow in practice; typical graphs run far below the bound.
// Handles: negative weights, negative-cycle detection, unreachable nodes, directed or undirected, path recovery.
// Time: O(n * m) worst case, close to O(n + m) on typical graphs
// Indexing: 1-based nodes; the constructor takes the node count
// Note: the worst case is real and constructible, so prefer Bellman_Ford.cpp on adversarial input.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <typename T = ll> struct SPFA {
    struct Edge {
        int v;
        T w;
    };

    static constexpr T INF = numeric_limits<T>::max() / 4;

    int n;
    vector<vector<Edge>> adj;
    vector<T> dist;
    vector<int> par;
    bool neg_cycle = false; // set by the last shortest_path call

    SPFA(int n) : n(n), adj(n + 1), dist(n + 1, INF), par(n + 1, -1) {}

    void add_edge(int u, int v, T w, bool bidirectional = false) {
        adj[u].push_back({v, w});
        if (bidirectional) adj[v].push_back({u, w});
    }

    static bool is_inf(T d) { return d >= INF; }

    // distances from src; sets neg_cycle and returns early if one is reachable
    vector<T> shortest_path(int src) {
        dist.assign(n + 1, INF);
        par.assign(n + 1, -1);
        neg_cycle = false; // reset, or a clean second run inherits the first verdict
        vector<int> relaxed(n + 1, 0), in_queue(n + 1, 0);
        queue<int> q;
        dist[src] = 0;
        q.push(src);
        in_queue[src] = 1;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            in_queue[u] = 0;
            for (const Edge &e : adj[u]) {
                if (dist[u] + e.w < dist[e.v]) {
                    dist[e.v] = dist[u] + e.w;
                    par[e.v] = u;
                    if (!in_queue[e.v]) {
                        if (++relaxed[e.v] >= n) { // enqueued n times means a negative cycle
                            neg_cycle = true;
                            return dist;
                        }
                        q.push(e.v);
                        in_queue[e.v] = 1;
                    }
                }
            }
        }
        return dist;
    }

    T shortest_path(int src, int dest) { return shortest_path(src)[dest]; }

    bool has_negative_cycle(int src) { // one reachable from src
        shortest_path(src);
        return neg_cycle;
    }

    vector<int> path(int src, int dest) { // empty if unreachable, or if a negative cycle was hit
        shortest_path(src);
        if (neg_cycle || is_inf(dist[dest])) return {};
        vector<int> p;
        for (int i = dest; i != -1; i = par[i]) p.push_back(i);
        reverse(p.begin(), p.end());
        return p;
    }
};

// Standard problem: shortest distance from node 1 to every node of a directed graph that may have negative edges
void solve() {
    int n, m;
    cin >> n >> m;
    SPFA<ll> sp(n);
    for (int i = 0, u, v; i < m; i++) {
        ll w;
        cin >> u >> v >> w;
        sp.add_edge(u, v, w);
    }
    vector<ll> d = sp.shortest_path(1);
    if (sp.neg_cycle) {
        cout << "NEGATIVE CYCLE\n";
        return;
    }
    for (int i = 1; i <= n; i++) {
        if (SPFA<ll>::is_inf(d[i]))
            cout << "INF" << " \n"[i == n];
        else
            cout << d[i] << " \n"[i == n];
    }
}
