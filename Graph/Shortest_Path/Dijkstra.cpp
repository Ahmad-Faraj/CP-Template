// Dijkstra: shortest paths from one or several sources on a graph with non-negative weights.
// Use when: "cheapest route from a to b", "shortest time to reach every city", grid shortest path with costs.
// Handles: directed or undirected, multi-edges, self loops, several sources, path recovery. Not negative weights.
// Time: O((n + m) log n)
// Indexing: 1-based nodes; the constructor takes the node count, not the edge count
// Note: unreachable nodes come back as INF, and path() comes back empty. Every call recomputes from scratch.

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

    vector<T> shortest_path(const vector<int> &srcs) { // distance to every node from the nearest source
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

    T shortest_path(int src, int dest) { return shortest_path(src)[dest]; } // INF if dest is unreachable

    vector<int> path(int src, int dest) { // the nodes src..dest, or empty if unreachable
        shortest_path(src);
        if (dist[dest] == INF) return {};
        vector<int> p;
        for (int i = dest; i != -1; i = par[i]) p.push_back(i);
        reverse(p.begin(), p.end());
        return p;
    }
};

// Standard problem: CSES 1671 - shortest distance from node 1 to every node of a directed graph
void solve() {
    int n, m;
    cin >> n >> m;
    Dijkstra<ll> dij(n);
    for (int i = 0, u, v; i < m; i++) {
        ll w;
        cin >> u >> v >> w;
        dij.add_edge(u, v, w, false);
    }
    vector<ll> d = dij.shortest_path(1);
    for (int i = 1; i <= n; i++) cout << d[i] << " \n"[i == n];
}
