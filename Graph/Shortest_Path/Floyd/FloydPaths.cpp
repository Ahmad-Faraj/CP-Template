#include "../../../core.h"
/*
 * Topic: Path Counting (Floyd-Warshall variant)
 * Description: Counts the number of paths between all pairs.
 * 
 * Important Facts:
 * - 0-based and 1-based indexing supported via template parameter.
 * - Time Complexity: O(V^3)
 * - Space Complexity: O(V^2)
 */
template <typename T = int, int Base = 0>
struct FloydPaths {
    int n;
    vector<vector<T>> adj;

    FloydPaths(int _n = 0) : n(_n), adj(n + Base, vector<T>(n + Base, 0)) {}

    void add_edge(int u, int v, T w = 1) { adj[u][v] += w; }

    void build() {
        for (int k = Base; k < n + Base; k++)
            for (int i = Base; i < n + Base; i++)
                for (int j = Base; j < n + Base; j++) adj[i][j] += adj[i][k] * adj[k][j];
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m;
    if (cin >> n >> m) {
        FloydPaths<long long, 1> fp(n);
        for (int i = 0; i < m; i++) {
            int u, v;
            long long w;
            cin >> u >> v >> w;
            fp.add_edge(u, v, w);
        }
        fp.build();
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                cout << fp.adj[i][j] << " ";
            }
            cout << "\n";
        }
    }
    return 0;
}
