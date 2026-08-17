#include <bits/stdc++.h>
using namespace std;

/*
    [1] Definition:
    Lowest Common Ancestor (LCA) using Binary Lifting.

    [2] Time & Space Complexity:
    - Build Time (DFS): O(N log N)
    - Query Time (LCA / K-th Ancestor): O(log N)
    - Space: O(N log N) for the 'anc' table.

    [3] Important Notes:
    - Uses 1-based indexing.
    - dep[u] stores the depth of node u (root is depth 1).
    - anc[u][i] stores the (2^i)-th ancestor of node u.
    - LOG = 20 is sufficient for N up to 10^6.
*/

const int N = 2e5 + 5;
const int LOG = 20;

vector<int> adj[N];
int anc[N][LOG];
int dep[N];

void dfs_lca(int u, int p) {
    dep[u] = dep[p] + 1;
    anc[u][0] = p;

    for (int bit = 1; bit < LOG; bit++) {
        anc[u][bit] = anc[anc[u][bit - 1]][bit - 1];
    }

    for (int v : adj[u]) {
        if (v == p) continue;
        dfs_lca(v, u);
    }
}

int kth_ancestor(int u, int k) {
    if (dep[u] <= k) return -1;
    
    for (int bit = LOG - 1; bit >= 0; bit--) {
        if (k & (1 << bit)) {
            u = anc[u][bit];
        }
    }
    return u;
}

int get_lca(int u, int v) {
    if (dep[u] < dep[v])
        swap(u, v);

    u = kth_ancestor(u, dep[u] - dep[v]);

    if (u == v) return u; 

    for (int bit = LOG - 1; bit >= 0; bit--) {
        if (anc[u][bit] != anc[v][bit]) {
            u = anc[u][bit];
            v = anc[v][bit];
        }
    }
    return anc[u][0];
}

int get_dist(int u, int v) {
    int lca = get_lca(u, v);
    return dep[u] + dep[v] - 2 * dep[lca];
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, q;
    cin >> n >> q ;

    for (int i = 2; i <= n; i++) {
        int par;
        cin >> par;
        adj[par].push_back(i);
        adj[i].push_back(par);
    }

    dfs_lca(1, 0);

    while (q--) {
        int a, b;
        cin >> a >> b;
        
        cout << get_lca(a, b) << "\n";
    }

    return 0;
}