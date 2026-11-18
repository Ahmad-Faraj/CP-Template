// Dijkstra: shortest paths from one or several sources with non-negative weights.
// Use when: "cheapest route from a to b", "shortest time to every city", weighted grid paths.
// Usage: Dijkstra<ll> g(n); g.add_edge(u, v, w); auto d = g.shortest_path(src); g.path(s, t);
// Handles: directed or undirected (add_edge is undirected by default), multi-edges, self loops,
//          several sources, path recovery. Not: negative weights.
// Time: O((n + m) log n)
// Indexing: 1-based nodes; the constructor takes the node count, not the edge count
// Note: unreachable nodes get INF and path() is empty. Every call recomputes from scratch.

#include <bits/stdc++.h>
using namespace std;

using ll = long long;

template <typename T = ll> struct Dijkstra {
    struct Edge {
        int v;
        T w;
        Edge(int V = 0, T W = 0) : v(V), w(W) {}
        bool operator<(const Edge &e) const { return w > e.w; }
    };

    static constexpr T INF = numeric_limits<T>::max() / 4;

    int n;
    vector<vector<Edge>> adj;
    vector<T> dist;
    vector<int> par;

    Dijkstra(int n) : n(n), adj(n + 1), dist(n + 1, INF), par(n + 1, -1) {}

    void add_edge(int u, int v, T w, bool bidirectional = true) {
        adj[u].push_back(Edge(v, w));
        if (bidirectional) adj[v].push_back(Edge(u, w));
    }

    // distance to every node from the nearest source
    vector<T> shortest_path(const vector<int> &srcs) {
        dist.assign(n + 1, INF);
        par.assign(n + 1, -1);
        priority_queue<Edge> pq;
        for (int s : srcs) {
            dist[s] = 0;
            pq.push(Edge(s, 0));
        }
        while (!pq.empty()) {
            Edge e = pq.top();
            pq.pop();
            if (e.w > dist[e.v]) continue;
            for (const Edge &ne : adj[e.v]) {
                if (dist[ne.v] > e.w + ne.w) {
                    dist[ne.v] = e.w + ne.w;
                    par[ne.v] = e.v;
                    pq.push(Edge(ne.v, dist[ne.v]));
                }
            }
        }
        return dist;
    }

    vector<T> shortest_path(int src) { return shortest_path(vector<int>{src}); }

    // INF if dest is unreachable
    T shortest_path(int src, int dest) { return shortest_path(src)[dest]; }

    vector<int> path(int src, int dest) { // the nodes src..dest, or empty if unreachable
        shortest_path(src);
        if (dist[dest] == INF) return {};
        vector<int> p;
        for (int i = dest; i != -1; i = par[i]) p.push_back(i);
        reverse(p.begin(), p.end());
        return p;
    }
};
