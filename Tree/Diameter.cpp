#include <bits/stdc++.h>
using namespace std;

/*
    [1] Definition:
    - Tree Diameter: Maximum distance between any two nodes. Solved by 2 DFSs.
      Includes endpoints, length, and the exact path between endpoints.
    - Tree Distances I: Max distance from each node to any other node.
      The farthest node from any 'u' is ALWAYS one of the diameter endpoints (A or B).
    - Tree Distances II: Sum of distances from each node to all others.
      Solved using Rerooting DP (In-Out DP).

    [2] Time & Space Complexity:
    - Time: O(N) for all operations.
    - Space: O(N)

    [3] Important Notes:
    - Uses 1-based indexing.
    - 'distA' stores distances from diameter endpoint A.
    - 'distB' stores distances from diameter endpoint B.
    - 'parent' array is used to reconstruct the diameter path.
*/

const int N = 2e5 + 5;
vector<int> adj[N];
int n;

int distA[N], distB[N], parent[N];
int endA, endB, diameter_len;

void dfs_dist(int u, int p, int d, int *current_dist, bool record_parent = false) {
    current_dist[u] = d;
    if (record_parent) parent[u] = p; // Record parent to reconstruct the path
    
    for (int v : adj[u]) {
        if (v == p) continue;
        dfs_dist(v, u, d + 1, current_dist, record_parent);
    }
}

void find_diameter_endpoints() {
    dfs_dist(1, 0, 0, distA, false);
    endA = 1;
    for (int i = 1; i <= n; i++) {
        if (distA[i] > distA[endA]) endA = i;
    }

    dfs_dist(endA, 0, 0, distA, true);
    endB = 1;
    for (int i = 1; i <= n; i++) {
        if (distA[i] > distA[endB]) endB = i;
    }
    
    diameter_len = distA[endB];

    dfs_dist(endB, 0, 0, distB, false);
}

vector<int> get_diameter_path() {
    vector<int> path;
    for (int u = endB; u != 0; u = parent[u]) {
        path.push_back(u);
    }
    return path;
}


// [2] Tree Distances II (Rerooting DP)
int sz[N];
long long sum_dist[N];

void dfs_sz(int u, int p, int d) {
    sz[u] = 1;
    sum_dist[1] += d;

    for (int v : adj[u]) {
        if (v == p) continue;
        dfs_sz(v, u, d + 1);
        sz[u] += sz[v];
    }
}

void dfs_reroot(int u, int p) {
    for (int v : adj[u]) {
        if (v == p) continue;
        
        // Rerooting logic: moving from u to v
        sum_dist[v] = sum_dist[u] + n - 2LL * sz[v];
        
        dfs_reroot(v, u);
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n ;

    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // [PROBLEM 1]: Tree Diameter
    /*
    find_diameter_endpoints();
    cout << diameter_len ;
    */

    // [PROBLEM 2]: Tree Distances I (Max distance from each node)
    /*
    find_diameter_endpoints();
    for (int i = 1; i <= n; i++) {
        // The farthest node is always one of the diameter's endpoints (A or B)
        cout << max(distA[i], distB[i]) << (i == n ? "" : " ");
    }
    cout << "\n";
    */

    // [PROBLEM 3]: Tree Distances II (Sum of distances from each node)
    /*
    dfs_sz(1, 0, 0);
    dfs_reroot(1, 0);

    for (int i = 1; i <= n; i++) {
        cout << sum_dist[i] << (i == n ? "" : " ");
    }
    cout << "\n";
    */

    return 0;
}