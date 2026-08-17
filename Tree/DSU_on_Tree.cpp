// DSU on Tree (Sack): answers a question about every subtree in O(n log n), reusing one global counter.
// Use when: "for each subtree, how many distinct colours / the most frequent value / the count of colour c".
// Handles: any statistic you can add and remove one node at a time. Subtrees only - not paths.
// Time: O(n log n) calls to add / remove
// Indexing: 1-based nodes
// Note: the problem-specific parts are add()'s body and the line recording ans[u]. Everything else is fixed.

#include <bits/stdc++.h>
using namespace std;

const int N = 1e5 + 9;

vector<int> g[N];
int ans[N], col[N], sz[N], cnt[N];
bool big[N];
int cur = 0; // the statistic over the nodes currently counted; here, distinct colours

void dfs(int u, int p) {
    sz[u] = 1;
    for (auto v : g[u]) {
        if (v == p) continue;
        dfs(v, u);
        sz[u] += sz[v];
    }
}

void add(int u, int p, int x) { // fold u's subtree in (x = 1) or back out (x = -1), skipping the kept big child
    if (x == 1) {
        if (cnt[col[u]]++ == 0) cur++;
    } else {
        if (--cnt[col[u]] == 0) cur--;
    }
    for (auto v : g[u]) {
        if (v == p || big[v]) continue;
        add(v, u, x);
    }
}

void dsu(int u, int p, bool keep) {
    int bigchild = -1, mx = -1;
    for (auto v : g[u]) {
        if (v == p) continue;
        if (sz[v] > mx) mx = sz[v], bigchild = v;
    }
    for (auto v : g[u]) {
        if (v == p || v == bigchild) continue;
        dsu(v, u, 0);
    }
    if (bigchild != -1) dsu(bigchild, u, 1), big[bigchild] = 1;
    add(u, p, 1);
    ans[u] = cur; // <- record the answer for u's subtree here
    if (bigchild != -1) big[bigchild] = 0;
    if (keep == 0) add(u, p, -1);
}

// Standard problem: for every node, how many distinct colours appear in its subtree
void solve() {
    int n;
    cin >> n;
    for (int i = 1; i <= n; i++) cin >> col[i];
    for (int i = 1, u, v; i < n; i++) {
        cin >> u >> v;
        g[u].push_back(v), g[v].push_back(u);
    }
    dfs(1, 0);
    dsu(1, 0, 1);
    for (int i = 1; i <= n; i++) cout << ans[i] << " \n"[i == n];
}
