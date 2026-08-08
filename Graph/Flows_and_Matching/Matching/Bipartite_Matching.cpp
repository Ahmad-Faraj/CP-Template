#include "../../../core.h"
/*
 * Algorithm: Bipartite Matching (Kuhn's Algorithm)
 * Purpose: Finds maximum bipartite matching using simple DFS.
 * Complexity: Time O(V * E) | Space O(V + E)
 * Verified: [CSES 1696 - School Dance](https://cses.fi/problemset/task/1696)
 */

struct BipartiteMatching {
    int n, m;
    vector<vector<int>> adj;
    vector<int> mt;
    vector<bool> used;

    BipartiteMatching(int _n, int _m) : n(_n), m(_m), adj(_n + 1), mt(_m + 1, -1) {}

    // Adds a directed edge from left node u to right node v
    void add_edge(int u, int v) {
        adj[u].push_back(v);
    }

    bool try_kuhn(int v) {
        if (used[v]) return false;
        used[v] = true;
        for (int to : adj[v]) {
            if (mt[to] == -1 || try_kuhn(mt[to])) {
                mt[to] = v;
                return true;
            }
        }
        return false;
    }

    int maximum_matching() {
        int ans = 0;
        for (int i = 1; i <= n; ++i) {
            used.assign(n + 1, false);
            if (try_kuhn(i)) ans++;
        }
        return ans;
    }


};

/*
 * Takes n (left size), m (right size), and k (edges). 
 * Gives maximum matching size and the matched pairs.
 */
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m, k;
    if (cin >> n >> m >> k) {
        BipartiteMatching bm(n, m);
        for (int i = 0; i < k; ++i) {
            int u, v;
            cin >> u >> v;
            bm.add_edge(u, v);
        }
        cout << bm.maximum_matching() << "\n";
        for (int i = 1; i <= m; ++i) {
            if (bm.mt[i] != -1) {
                cout << bm.mt[i] << " " << i << "\n";
            }
        }
    }
    return 0;
}