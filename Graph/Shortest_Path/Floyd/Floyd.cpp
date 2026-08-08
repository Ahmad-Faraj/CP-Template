#include "../../../core.h"
/*
 * Algorithm: Floyd-Warshall (All-Pairs Shortest Path)
 * Purpose: Finds shortest paths between all pairs. Best for small dense graphs (V <= 400).
 * Complexity: Time O(V^3) | Space O(V^2)
 * Verified: [CSES 1672 - Shortest Routes II](https://cses.fi/problemset/task/1672)
 */
template <typename T = int, int Base = 0>
struct Floyd {
    int n;
    const T INF = 2e18;
    vector<vector<T>> dist;
    vector<vector<int>> par;

    // Time Complexity: O(N^2)
    // Space Complexity: O(N^2)
    Floyd(int _n = 0) : n(_n) {
        dist.assign(n + Base, vector<T>(n + Base, INF));
        par.assign(n + Base, vector<int>(n + Base, -1));
        for (int i = Base; i < n + Base; i++) dist[i][i] = 0;
    }

    // Time Complexity: O(N^3)
    // Space Complexity: O(N^2)
    Floyd(int _n, const vector<vector<T>>& D) : n(_n), dist(D) {
        par.assign(n + Base, vector<int>(n + Base, -1));
        build();
    }

    // Time Complexity: O(1)
    // Space Complexity: O(1) per edge
    void add_edge(int u, int v, T w) {
        if (w < dist[u][v]) {
            dist[u][v] = w;
            par[u][v] = u;
        }
    }

    // Time Complexity: O(N^3)
    // Space Complexity: O(1)
    void build() {
        for (int i = Base; i < n + Base; i++)
            for (int u = Base; u < n + Base; u++)
                for (int v = Base; v < n + Base; v++)
                    if (dist[u][i] != INF && dist[i][v] != INF) {
                        if (dist[u][i] + dist[i][v] < dist[u][v]) {
                            dist[u][v] = dist[u][i] + dist[i][v];
                            par[u][v] = par[i][v];
                        }
                    }
    }

};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m, q;
    if (cin >> n >> m >> q) {
        Floyd<long long, 1> floyd(n);
        for (int i = 0; i < m; ++i) {
            int u, v;
            long long w;
            cin >> u >> v >> w;
            floyd.add_edge(u, v, w);
            floyd.add_edge(v, u, w);
        }
        floyd.build();
        for (int i = 0; i < q; ++i) {
            int u, v;
            cin >> u >> v;
            if (floyd.dist[u][v] >= floyd.INF / 2) {
                cout << -1 << "\n";
            } else {
                cout << floyd.dist[u][v] << "\n";
            }
        }
    }
    return 0;
}
