#include "../../../core.h"
/*
 * Topic: Minimum Delta Cycle
 * Description: Finds cycle weight differences using Bellman-Ford relaxations.
 * 
 * Important Facts:
 * - 0-based indexing for nodes by default.
 * - Time Complexity: O(V * E)
 * - Space Complexity: O(V + E)
 */
template <typename T = int>
struct MinDeltaCycle {
    struct Edge {
        int u, v;
        T w;
    };

    int n;
    vector<Edge> edges;
    const T INF = 2e18;

    MinDeltaCycle(int _n) : n(_n) {}

    void add_edge(int u, int v, T w) { edges.push_back({u, v, w}); }

    // Finds the minimum total-weight (most negative) cycle-delta in the graph.
    T min_delta_negative_cycle() {
        vector<vector<T>> dp(n + 1, vector<T>(n, INF));
        for (int v = 0; v < n; ++v) dp[0][v] = 0;
        for (int k = 1; k <= n; ++k) {
            for (const auto& e : edges) {
                if (dp[k - 1][e.u] != INF) { dp[k][e.v] = min(dp[k][e.v], dp[k - 1][e.u] + e.w); }
            }
        }
        T min_delta = INF;
        for (int v = 0; v < n; ++v) {
            if (dp[n][v] == INF) continue;
            T temp = -(2e18);
            for (int k = 0; k < n; ++k) {
                if (dp[k][v] == INF) continue;
                T delta = dp[n][v] - dp[k][v];
                temp = max(temp, delta);
            }
            min_delta = min(min_delta, temp);
        }
        return min_delta;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m;
    if (cin >> n >> m) {
        MinDeltaCycle<long long> mdc(n);
        for (int i = 0; i < m; i++) {
            int u, v;
            long long w;
            cin >> u >> v >> w;
            mdc.add_edge(u - 1, v - 1, w);
        }
        long long res = mdc.min_delta_negative_cycle();
        if (res >= 0) {
            cout << "No negative cycle\n";
        } else {
            cout << res << "\n";
        }
    }
    return 0;
}
