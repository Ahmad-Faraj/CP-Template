#include <bits/stdc++.h>
using namespace std;

#define int long long
/*
    [1] Definition:
    Euler Tour (Tree Flattening) converts a tree into a 1D array.
    Each node 'u' represents a contiguous subarray [st[u], en[u]].
    This allows updating and querying subtrees using Fenwick or Segment Trees.

    [2] Time & Space Complexity:
    - Build Time: O(N)
    - Space: O(N)
    - Subtree Query / Update: O(1) to get the range, then O(log N) for BIT

    [3] Important Notes:
    - Uses 1-based indexing.
    - st[u] is the entry time, en[u] is the exit time.

    [4] Solves the sum of a subarray with updates.
*/

const int N = 2e5 + 5;

vector<int> adj[N];
long long val[N];

int st[N], en[N], node_at[N];
int timer_dfs = 0;

void dfs_euler(int u, int p) {
    st[u] = ++timer_dfs;
    node_at[timer_dfs] = u;

    for (int v : adj[u]) {
        if (v == p) continue;
        dfs_euler(v, u);
    }

    en[u] = timer_dfs;
}

// Check if 'u' is an ancestor of 'v'
bool is_ancestor(int u, int v) {
    return st[u] <= st[v] && en[u] >= en[v];
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, q;
    cin >> n >> q;

    for (int i = 1; i <= n; i++) {
        cin >> val[i];
    }

    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    timer_dfs = 0;
    dfs_euler(1, 0);

    vector<int> a(n + 1);

    // You need to declare Fenwick tree.

    // Fenwick_Tree<int> F ( n, a ) ;
    for (int i = 1; i <= n; i++) {
        // F.assign(st[i], val[i]);
    }

    while (q--) {
        int type;
        cin >> type;

        if (type == 1) {
            int s;
            long long x;
            cin >> s >> x;

            // F.assign(st[s], x);

            val[s] = x;

        } else if (type == 2) {
            int s;
            cin >> s;
            // cout << F.query(st[s], en[s]) << "\n";
        }
    }

    return 0;
}