#include "../../../core.h"
/*
 * Topic: Dijkstra's Algorithm
 * Description: Single-source shortest path for graphs with non-negative weights.
 * 
 * Important Facts:
 * - 1-based indexing for nodes by default.
 * - Time Complexity: O(E log V)
 * - Space Complexity: O(V + E)
 */
template <typename T = int>
struct Dijkstra {
    struct Edge {
        T v, w;

        // Time Complexity: O(1)
        // Space Complexity: O(1)
        // Initializes an edge with destination v and weight w.
        Edge(T _v = 0, T _w = 0) : v(_v), w(_w) {}

        // Time Complexity: O(1)
        // Space Complexity: O(1)
        // Compares edges by weight for the priority queue (min-heap).
        bool operator<(const Edge& e) const { return w > e.w; }
    };

    int n;
    const T INF = 2e18;
    vector<vector<Edge>> adj;

    // Time Complexity: O(N)
    // Space Complexity: O(N)
    // Initializes the Dijkstra state for N nodes.
    Dijkstra(int _n) : n(_n), adj(n + 1) {}

    // Time Complexity: O(1)
    // Space Complexity: O(1) per edge
    // Adds a directed or undirected edge with weight w between u and v.
    void add_edge(int u, int v, T w, bool undirected = true) {
        adj[u].push_back(Edge(v, w));
        if (undirected) adj[v].push_back(Edge(u, w));
    }

    // Time Complexity: O((N + M) * log(M))
    // Space Complexity: O(N + M)
    // Computes the shortest distance from src to all other nodes.
    vector<T> get_dist(int src) {
        vector<T> dist(n + 1, INF);
        dist[src] = 0;
        priority_queue<Edge> pq;
        pq.push(Edge(src, 0));
        while (!pq.empty()) {
            Edge top = pq.top();
            pq.pop();
            T u = top.v, cost = top.w;
            if (cost > dist[u]) continue;
            for (const auto& edge : adj[u]) {
                if (dist[edge.v] > dist[u] + edge.w) {
                    dist[edge.v] = dist[u] + edge.w;
                    pq.push(Edge(edge.v, dist[edge.v]));
                }
            }
        }
        return dist;
    }


    T min_cost(int src, int dest) {
        vector<T> dist(n + 1, INF);
        dist[src] = 0;
        priority_queue<Edge> pq;
        pq.push(Edge(src, 0));
        while (!pq.empty()) {
            Edge top = pq.top();
            pq.pop();
            T u = top.v, cost = top.w;
            if (cost > dist[u]) continue;
            for (const auto& edge : adj[u]) {
                if (dist[edge.v] > dist[u] + edge.w) {
                    dist[edge.v] = dist[u] + edge.w;
                    pq.push(Edge(edge.v, dist[edge.v]));
                }
            }
        }
        return dist[dest] == INF ? -1 : dist[dest];
    }

    vector<int> get_path(int src, int dest) {
        vector<T> dist(n + 1, INF);
        vector<int> parent(n + 1, -1);
        dist[src] = 0;
        priority_queue<Edge> pq;
        pq.push(Edge(src, 0));
        while (!pq.empty()) {
            Edge top = pq.top();
            pq.pop();
            T u = top.v, cost = top.w;
            if (cost > dist[u]) continue;
            for (const auto& edge : adj[u]) {
                if (dist[edge.v] > dist[u] + edge.w) {
                    dist[edge.v] = dist[u] + edge.w;
                    parent[edge.v] = u;
                    pq.push(Edge(edge.v, dist[edge.v]));
                }
            }
        }
        vector<int> path;
        for (int i = dest; i != -1; i = parent[i]) path.push_back(i);
        reverse(path.begin(), path.end());
        return path;
    }

};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m;
    if (cin >> n >> m) {
        Dijkstra<long long> dij(n);
        for (int i = 0; i < m; ++i) {
            int u, v;
            long long w;
            cin >> u >> v >> w;
            dij.add_edge(u, v, w, false);
        }
        vector<long long> dist = dij.get_dist(1);
        for (int i = 1; i <= n; ++i) {
            cout << dist[i] << (i == n ? "" : " ");
        }
        cout << "\n";
    }
    return 0;
}
