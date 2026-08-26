#include "../../../core.h"

/*
 * Topic: Shortest Path Faster Algorithm (SPFA)
 * Description: Queue-optimized Bellman-Ford. Fast on average but O(V*E) worst-case.
 *
 * Important Facts:
 * - 1-based indexing for nodes by default.
 * - Time Complexity: O(E) avg, O(V * E) worst
 * - Space Complexity: O(V + E)
 */
template <typename T = int> struct SPFA {
    struct Edge {
        int v;
        T w;

        // Time Complexity: O(1)
        // Space Complexity: O(1)
        // Initializes an edge with destination _v and weight _w.
        Edge(int _v, T _w) : v(_v), w(_w) {}

        // Time Complexity: O(1)
        // Space Complexity: O(1)
        // Compares edges by weight.
        bool operator<(const Edge &e) const { return w < e.w; }
    };

    int n;
    bool neg_cycle = false;
    const T INF = 2e18;
    vector<vector<Edge>> adj;
    vector<T> dist;

    // Time Complexity: O(N)
    // Space Complexity: O(N)
    // Initializes the SPFA state for N nodes.
    SPFA(int _n) : n(_n), adj(n + 1) {}

    // Time Complexity: O(1)
    // Space Complexity: O(1) per edge
    // Adds a directed or undirected edge with weight w between u and v.
    void add_edge(int u, int v, T w, bool directed = true) {
        adj[u].push_back(Edge(v, w));
        if (!directed) adj[v].push_back(Edge(u, w));
    }

    // Time Complexity: O(N * M)
    // Space Complexity: O(N)
    // Computes shortest distances from src to all nodes and detects negative cycles.
    void calc(int src) {
        dist.assign(n + 1, INF);
        queue<int> q;
        vector<int> cnt(n + 1, 0), in_queue(n + 1, 0);
        dist[src] = 0;
        q.push(src);
        in_queue[src] = 1;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            in_queue[u] = 0;
            for (const auto &e : adj[u]) {
                if (dist[u] + e.w < dist[e.v]) {
                    dist[e.v] = dist[u] + e.w;
                    if (!in_queue[e.v]) {
                        q.push(e.v);
                        in_queue[e.v] = 1;
                        if (++cnt[e.v] >= n) {
                            neg_cycle = true;
                            return;
                        }
                    }
                }
            }
        }
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m;
    if (cin >> n >> m) {
        SPFA<long long> spfa(n);
        for (int i = 0; i < m; ++i) {
            int u, v;
            long long w;
            cin >> u >> v >> w;
            spfa.add_edge(u, v, w, true);
        }
        spfa.calc(1);
        for (int i = 1; i <= n; ++i) {
            cout << spfa.dist[i] << (i == n ? "" : " ");
        }
        cout << "\n";
    }
    return 0;
}
