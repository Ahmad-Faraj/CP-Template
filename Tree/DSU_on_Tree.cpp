#include <bits/stdc++.h>
using namespace std;

/*
    [1] Definition:
    DSU on Tree (Sack) is a technique to answer offline subtree queries.

    [2] Time & Space Complexity:
    - Time: O(N log N) -> Each node is added/removed at most O(log N) times.
    - Space: O(N) memory for tree and frequency arrays.

    [3] Important Notes:
    - Uses 1-based indexing.
    - If node values/colors are large (e.g., up to 10^9), you MUST use
    Coordinate Compression before passing them to the struct.

    [4] Solves the number of distinct colors in a subtree.
*/

const int N = 2e5 + 5;

vector<int> adj[N];
int col[N], sz[N], ans[N];
bool big[N];

int cnt[N]; // Frequency array for colors
int current_distinct = 0;

void addNode(int u) {
    if (cnt[col[u]] == 0) current_distinct++;
    cnt[col[u]]++;
}

void removeNode(int u) {
    cnt[col[u]]--;
    if (cnt[col[u]] == 0) current_distinct--;
}

void dfs_sz(int u, int p) {
    sz[u] = 1;
    for (int v : adj[u]) {
        if (v == p) continue;
        dfs_sz(v, u);
        sz[u] += sz[v];
    }
}

void updateSubtree(int u, int p, int val) {
    if (val == 1) addNode(u);
    else removeNode(u);

    for (int v : adj[u]) {
        if (v == p || big[v]) continue;
        updateSubtree(v, u, val);
    }
}

void dfs_dsu(int u, int p, bool keep) {
    int mx = -1, bigChild = -1;
    
    for (int v : adj[u]) {
        if (v == p) continue;
        if (sz[v] > mx) {
            mx = sz[v];
            bigChild = v;
        }
    }

    for (int v : adj[u]) {
        if (v == p || v == bigChild) continue;
        dfs_dsu(v, u, 0);
    }

    if (bigChild != -1) {
        dfs_dsu(bigChild, u, 1);
        big[bigChild] = 1; 
    }

    updateSubtree(u, p, 1);
    
    // Answer the subtree of node 'u' here.
    ans[u] = current_distinct;

    if (bigChild != -1) big[bigChild] = 0;
    if (!keep) updateSubtree(u, p, -1);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    vector<int> original_colors(n);
    for (int i = 1; i <= n; i++) {
        cin >> col[i];
        original_colors[i - 1] = col[i];
    }

    // --- Coordinate Compression ---
    sort(original_colors.begin(), original_colors.end());
    original_colors.erase(unique(original_colors.begin(), original_colors.end()), original_colors.end());
    
    for (int i = 1; i <= n; i++) {
        col[i] = lower_bound(original_colors.begin(), original_colors.end(), col[i]) - original_colors.begin() + 1;
    }

    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    dfs_sz(1, 0);

    dfs_dsu(1, 0, 1);

    for (int i = 1; i <= n; i++) {
        cout << ans[i] << (i == n ? "" : " ");
    }
    cout << "\n";

    return 0;
}