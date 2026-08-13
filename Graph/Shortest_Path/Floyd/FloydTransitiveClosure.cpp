#include "../../../core.h"
/*
 * Topic: Transitive Closure (Floyd-Warshall)
 * Description: Computes reachability between all pairs of nodes in a directed graph.
 * 
 * Important Facts:
 * - 0-based and 1-based indexing supported via template parameter.
 * - Time Complexity: O(V^3)
 * - Space Complexity: O(V^2)
 */
template <int Base = 0>
struct FloydTransitiveClosure {
    int n;
    vector<vector<bool>> adj;

    FloydTransitiveClosure(int _n = 0) : n(_n), adj(n + Base, vector<bool>(n + Base, false)) {}

    void add_edge(int u, int v) { adj[u][v] = true; }

    void build() {
        for (int k = Base; k < n + Base; k++)
            for (int i = Base; i < n + Base; i++)
                for (int j = Base; j < n + Base; j++) adj[i][j] = adj[i][j] | (adj[i][k] & adj[k][j]);
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m;
    if (cin >> n >> m) {
        FloydTransitiveClosure<1> ftc(n);
        for (int i = 0; i < m; i++) {
            int u, v;
            cin >> u >> v;
            ftc.add_edge(u, v);
        }
        ftc.build();
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                cout << (ftc.adj[i][j] ? 1 : 0) << " ";
            }
            cout << "\n";
        }
    }
    return 0;
}
