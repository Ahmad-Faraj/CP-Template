// Bellman Ford: shortest paths from one source with negative edges; finds negative cycles.
// Use when: weights can be negative, "is there an ever-cheaper cycle", longest path via negation.
// Usage: Bellman_Ford<ll> g(n); g.add_edge(u, v, w); auto d = g.shortest_path(src); g.path(s, t);
// Handles: negative weights, negative cycles (every affected node), unreachable nodes, multi-edges.
// Time: O(n * m)
// Indexing: 1-based nodes; the constructor takes the node count
// Note: nodes pulled down forever by a negative cycle get NEG, unreachable ones INF:
//       test with is_neg / is_inf, never compare to them directly.

#include <bits/stdc++.h>
using namespace std;

using ll = long long;

template <typename T = ll> struct Bellman_Ford {
    struct Edge {
        int u, v;
        T w;
    };

    static constexpr T INF = numeric_limits<T>::max() / 4;
    static constexpr T NEG = -INF;

    int n;
    vector<Edge> edges;
    vector<T> dist;
    vector<int> par;

    Bellman_Ford(int n) : n(n), dist(n + 1, INF), par(n + 1, -1) {}

    void add_edge(int u, int v, T w, bool bidirectional = false) {
        edges.push_back({u, v, w});
        if (bidirectional) edges.push_back({v, u, w});
    }

    static bool is_inf(T d) { return d >= INF; }
    static bool is_neg(T d) { return d <= NEG; }

    // INF if unreachable, NEG if a negative cycle drives it down forever
    vector<T> shortest_path(int src) {
        dist.assign(n + 1, INF);
        par.assign(n + 1, -1);
        dist[src] = 0;
        for (int i = 0; i < n - 1; i++) {
            bool updated = false; // reset each round, so the early exit actually fires
            for (const Edge &e : edges)
                if (dist[e.u] != INF && dist[e.u] + e.w < dist[e.v]) {
                    dist[e.v] = dist[e.u] + e.w;
                    par[e.v] = e.u;
                    updated = true;
                }
            if (!updated) break;
        }

        // one more round: anything that still improves sits on or below a negative cycle
        vector<int> queue_;
        for (const Edge &e : edges)
            if (dist[e.u] != INF && dist[e.u] + e.w < dist[e.v]) queue_.push_back(e.v);

        if (!queue_.empty()) { // spread NEG to everything reachable from those nodes
            vector<vector<int>> adj(n + 1);
            for (const Edge &e : edges) adj[e.u].push_back(e.v);
            vector<char> seen(n + 1, 0);
            for (int s : queue_) seen[s] = 1;
            for (int i = 0; i < (int)queue_.size(); i++) {
                int u = queue_[i];
                dist[u] = NEG;
                for (int v : adj[u])
                    if (!seen[v]) seen[v] = 1, queue_.push_back(v);
            }
        }
        return dist;
    }

    T shortest_path(int src, int dest) { return shortest_path(src)[dest]; }

    bool has_negative_cycle(int src) { // one reachable from src
        shortest_path(src);
        for (int v = 1; v <= n; v++)
            if (is_neg(dist[v])) return true;
        return false;
    }

    // empty if unreachable or if the distance is unbounded below
    vector<int> path(int src, int dest) {
        shortest_path(src);
        if (is_inf(dist[dest]) || is_neg(dist[dest])) return {};
        vector<int> p;
        for (int i = dest; i != -1; i = par[i]) p.push_back(i);
        reverse(p.begin(), p.end());
        return p;
    }
};
