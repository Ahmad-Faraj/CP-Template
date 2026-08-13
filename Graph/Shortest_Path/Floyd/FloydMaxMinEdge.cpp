#include "../../../core.h"
/*
 * Topic: Maximin Path (Floyd-Warshall)
 * Description: Finds a path between all pairs maximizing the minimum edge weight.
 * 
 * Important Facts:
 * - 0-based and 1-based indexing supported via template parameter.
 * - Time Complexity: O(V^3)
 * - Space Complexity: O(V^2)
 */
template <typename T = int, int Base = 0>
struct FloydMaxMinEdge {
    int n;
    const T NINF = -(2e18);
    vector<vector<T>> dist;

    FloydMaxMinEdge(int _n = 0) : n(_n), dist(n + Base, vector<T>(n + Base, NINF)) {
        for (int i = Base; i < n + Base; i++) dist[i][i] = 0;
    }

    void add_edge(int u, int v, T w) { dist[u][v] = max(dist[u][v], w); }

    void build() {
        for (int k = Base; k < n + Base; k++)
            for (int i = Base; i < n + Base; i++)
                for (int j = Base; j < n + Base; j++) dist[i][j] = max(dist[i][j], min(dist[i][k], dist[k][j]));
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m;
    if (cin >> n >> m) {
        FloydMaxMinEdge<long long, 1> fw(n);
        for (int i = 0; i < m; i++) {
            int u, v;
            long long w;
            cin >> u >> v >> w;
            fw.add_edge(u, v, w);
        }
        fw.build();
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                if (fw.dist[i][j] == fw.NINF) cout << "NINF ";
                else cout << fw.dist[i][j] << " ";
            }
            cout << "\n";
        }
    }
    return 0;
}
