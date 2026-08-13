#include "../../../core.h"
/*
 * Topic: Bellman-Ford
 * Description: Single-source shortest path for graphs with negative weights. Detects negative cycles.
 * 
 * Important Facts:
 * - 1-based indexing for nodes by default.
 * - Time Complexity: O(V * E)
 * - Space Complexity: O(V + E)
 */
template <typename T = int>
struct BellmanFord {
    struct Edge {
        T u, v, w;

        Edge(T _u, T _v, T _w) : u(_u), v(_v), w(_w) {}
    };

    bool was_processed = false;
    T n, m;
    const T INF = 2e18;
    vector<T> dist, par;
    vector<Edge> edges;

    // Time Complexity: O(N)
    // Space Complexity: O(N)
    BellmanFord(T _n) : n(_n), m(0), dist(n + 1, INF), par(n + 1, -1) {}

    // Time Complexity: O(1)
    // Space Complexity: O(1) per edge
    void add_edge(T u, T v, T w) {
        edges.push_back({u, v, w});
        m++;
    }

    // Time Complexity: O(N * M) worst case, O(M) best case
    // Space Complexity: O(N)
    vector<T> shortest_path(T source) {
        dist.assign(n + 1, INF);
        dist[source] = 0;
        was_processed = true;
        for (T i = 0; i < n - 1; ++i) {
            bool updated = false;
            for (const auto& e : edges) {
                if (dist[e.u] != INF && dist[e.u] + e.w < dist[e.v]) {
                    dist[e.v] = dist[e.u] + e.w;
                    par[e.v] = e.u;
                    updated = true;
                }
            }
            if (!updated) break;
        }
        return dist;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m;
    if (cin >> n >> m) {
        BellmanFord<long long> bf(n);
        for (int i = 0; i < m; ++i) {
            int u, v;
            long long w;
            cin >> u >> v >> w;
            bf.add_edge(u, v, w);
        }
        vector<long long> dist = bf.shortest_path(1);
        for (int i = 1; i <= n; ++i) {
            cout << dist[i] << (i == n ? "" : " ");
        }
        cout << "\n";
    }
    return 0;
}
