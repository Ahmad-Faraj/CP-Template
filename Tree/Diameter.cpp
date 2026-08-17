#include <bits/stdc++.h>
using namespace std;

#define int long long

/*
    [1] Definition:
    - Tree Diameter: Maximum distance between any two nodes. Solved by 2 DFSs.
    - Tree Distances I: Max distance from each node to any other node.
      The farthest node from any node 'u' is ALWAYS one of the two diameter endpoints (A or B).
    - Tree Distances II: Sum of distances from each node to all others.
      Solved using Rerooting DP (In-Out DP).

    [2] Time & Space Complexity:
    - Time: O(N) for all variations (just a few DFS traversals).
    - Space: O(N)

    [3] Important Notes:
    - Uses 1-based indexing.
    - 'distA' stores distances from diameter endpoint A.
    - 'distB' stores distances from diameter endpoint B.
    - 'sum_dist' MUST be long long as sums can exceed 2^31-1.
*/

const int N = 2e5 + 5;
vector<int> adj[N];

int distA[N], distB[N];
int endA, endB, diameter_len;

void dfs_dist(int u, int p, int d, int *current_dist) {
    current_dist[u] = d;
    for (int v : adj[u]) {
        if (v == p) continue;
        dfs_dist(v, u, d + 1, current_dist);
    }
}

void find_diameter_endpoints() {
    dfs_dist(1, 0, 0, distA);
    endA = 1;
    for (int i = 1; i <= N - 5; i++) {
        if (distA[i] > distA[endA]) endA = i;
    }

    dfs_dist(endA, 0, 0, distA);
    endB = 1;
    for (int i = 1; i <= N - 5; i++) {
        if (distA[i] > distA[endB]) endB = i;
    }

    diameter_len = distA[endB];

    dfs_dist(endB, 0, 0, distB);
}

// For Tree Distances II
int sz[N];
long long sum_dist[N];
int total_nodes;

// Precalculate subtree sizes and the answer for the root (node 1)
void dfs_sz(int u, int p, int d) {
    sz[u] = 1;
    sum_dist[1] += d;

    for (int v : adj[u]) {
        if (v == p) continue;
        dfs_sz(v, u, d + 1);
        sz[u] += sz[v];
    }
}

// Rerooting technique: calculate answer for children using parent's answer
void dfs_reroot(int u, int p) {
    for (int v : adj[u]) {
        if (v == p) continue;
        sum_dist[v] = sum_dist[u] + total_nodes - 2LL * sz[v];

        dfs_reroot(v, u);
    }
}
signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    total_nodes = n;

    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // -- [PROBLEM 1] Tree Diameter
    // find_diameter_endpoints();
    // cout << diameter_len << "\n";

    // -- [PROBLEM 2] Tree Distances I
    // find_diameter_endpoints();
    // for (int i = 1; i <= n; i++) {
    //       // The farthest node is always one of the diameter's endpoints (A or B)
    //       cout << max(distA[i], distB[i]) << (i == n ? "" : " ");
    // }
    // cout << "\n";

    // [-- PROBLEM 3] Tree Distances II
    // dfs_sz(1, 0, 0);
    // dfs_reroot(1, 0);

    // for (int i = 1; i <= n; i++) {
    //       cout << sum_dist[i] << (i == n ? "" : " ");
    // }
    // cout << "\n";

    return 0;
}