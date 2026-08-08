#include "../../../core.h"
/*
 * Algorithm: Minimum Mean Cycle
 * Purpose: Finds a cycle in a directed graph with the minimum average edge weight.
 * Complexity: Time O(V * E) | Space O(V + E)
 * Verified: ..........
 */
template <typename T = int>
struct MinMeanCycle {
    struct Edge {
        int u, v;
        T w;
    };

    int n;
    vector<Edge> edges;
    const T INF = 2e18;

    MinMeanCycle(int _n) : n(_n) {}

    void add_edge(int u, int v, T w) { edges.push_back({u, v, w}); }

    // Returns minimum mean cycle or infinity if acyclic
    long double min_mean_cycle() {
        vector<vector<T>> dp(n + 1, vector<T>(n, INF));
        for (int v = 0; v < n; ++v) dp[0][v] = 0;
        for (int k = 1; k <= n; ++k) {
            for (const auto& e : edges) {
                if (dp[k - 1][e.u] != INF) { dp[k][e.v] = min(dp[k][e.v], dp[k - 1][e.u] + e.w); }
            }
        }
        long double mu = numeric_limits<long double>::infinity();
        for (int v = 0; v < n; ++v) {
            if (dp[n][v] == INF) continue;
            long double local_max = -numeric_limits<long double>::infinity();
            for (int k = 0; k < n; ++k) {
                if (dp[k][v] == INF) continue;
                long double mean = (long double)(dp[n][v] - dp[k][v]) / (n - k);
                local_max = max(local_max, mean);
            }
            mu = min(mu, local_max);
        }
        return mu;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m;
    if (cin >> n >> m) {
        MinMeanCycle<long long> mmc(n);
        for (int i = 0; i < m; i++) {
            int u, v;
            long long w;
            cin >> u >> v >> w;
            mmc.add_edge(u - 1, v - 1, w);
        }
        long double res = mmc.min_mean_cycle();
        if (res == numeric_limits<long double>::infinity()) {
            cout << "No cycle\n";
        } else {
            cout << fixed << setprecision(4) << res << "\n";
        }
    }
    return 0;
}
