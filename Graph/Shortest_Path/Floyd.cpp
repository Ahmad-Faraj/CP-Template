// Floyd Warshall: shortest paths between every pair of nodes, in one O(n^3) pass over the distance matrix.
// Use when: n is small (a few hundred) and you need all pairs, "distance between every two cities", transitive closure.
// Handles: negative weights, negative-cycle detection, unreachable pairs, multi-edges, paths, later edge additions.
// Time: build O(n^3) | query O(1) | add_edge_after_build O(n^2)
// Indexing: 1-based nodes; the constructor takes the node count
// Note: call build() once after all add_edge calls. Unreachable pairs stay INF; use is_inf rather than comparing to it.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <typename T = ll> struct Floyd {
    static constexpr T INF = numeric_limits<T>::max() / 4;

    int n;
    vector<vector<T>> dist;
    vector<vector<int>> nxt; // nxt[u][v] = the next node after u on a shortest u..v path

    Floyd(int n) : n(n), dist(n + 1, vector<T>(n + 1, INF)), nxt(n + 1, vector<int>(n + 1, -1)) {
        for (int i = 1; i <= n; i++) dist[i][i] = 0, nxt[i][i] = i;
    }

    static bool is_inf(T d) { return d >= INF; }

    void add_edge(int u, int v, T w, bool bidirectional = false) { // before build()
        if (w < dist[u][v]) dist[u][v] = w, nxt[u][v] = v;
        if (bidirectional && w < dist[v][u]) dist[v][u] = w, nxt[v][u] = u;
    }

    void build() {
        for (int k = 1; k <= n; k++)
            for (int u = 1; u <= n; u++) {
                if (is_inf(dist[u][k])) continue; // skip, or INF + INF silently overflows
                for (int v = 1; v <= n; v++) {
                    if (is_inf(dist[k][v])) continue;
                    if (dist[u][k] + dist[k][v] < dist[u][v]) {
                        dist[u][v] = dist[u][k] + dist[k][v];
                        nxt[u][v] = nxt[u][k];
                    }
                }
            }
    }

    T get(int u, int v) { return dist[u][v]; }

    bool has_negative_cycle() { // a node reachable back to itself at negative cost
        for (int i = 1; i <= n; i++)
            if (dist[i][i] < 0) return true;
        return false;
    }

    // true when the u..v distance is meaningless because a negative cycle sits on some u..v path
    bool affected_by_negative_cycle(int u, int v) {
        for (int k = 1; k <= n; k++)
            if (dist[k][k] < 0 && !is_inf(dist[u][k]) && !is_inf(dist[k][v])) return true;
        return false;
    }

    vector<int> path(int u, int v) { // the nodes u..v, or empty if unreachable
        if (nxt[u][v] == -1) return {};
        vector<int> p{u};
        while (u != v) {
            u = nxt[u][v];
            if (u == -1) return {};
            p.push_back(u);
            if ((int)p.size() > n + 1) return {}; // a negative cycle makes the walk unbounded
        }
        return p;
    }

    void add_edge_after_build(int u, int v, T w, bool bidirectional = false) { // repairs all pairs in O(n^2)
        auto relax = [&](int a, int b, T weight) {
            if (weight >= dist[a][b]) return;
            dist[a][b] = weight, nxt[a][b] = b;
            for (int i = 1; i <= n; i++) {
                if (is_inf(dist[i][a])) continue;
                for (int j = 1; j <= n; j++) {
                    if (is_inf(dist[b][j])) continue;
                    if (dist[i][a] + weight + dist[b][j] < dist[i][j]) {
                        dist[i][j] = dist[i][a] + weight + dist[b][j];
                        nxt[i][j] = (i == a ? b : nxt[i][a]);
                    }
                }
            }
        };
        relax(u, v, w);
        if (bidirectional) relax(v, u, w);
    }
};

// Standard problem: CSES Shortest Routes II - q queries for the distance between two cities, -1 when unreachable
void solve() {
    int n, m, q;
    cin >> n >> m >> q;
    Floyd<ll> f(n);
    for (int i = 0, u, v; i < m; i++) {
        ll w;
        cin >> u >> v >> w;
        f.add_edge(u, v, w, true);
    }
    f.build();
    while (q--) {
        int u, v;
        cin >> u >> v;
        ll d = f.get(u, v);
        cout << (Floyd<ll>::is_inf(d) ? -1 : d) << '\n';
    }
}
