// DSU On Tree (Sack): a statistic for every subtree, reusing one counter (heavy child is kept).
// Use when: "for each subtree: how many distinct colours / most frequent value / count of c".
// Usage: DSU_On_Tree D(n); D.add_edge(u, v); D.col[u] = c; vector<int> ans = D.run(root);
// Handles: any statistic you can add / remove one node at a time; n = 1. Subtrees only.
// Not: small-to-large set merging (that is DSU_On_Tree_2.cpp, O(n log^2 n)).
// Time: O(n log n) calls to add
// Indexing: 1-based nodes; colours >= 0, cnt is sized max colour + 1 (compress big colours)
// Note: the problem-specific parts are add()'s counter update and the line recording ans[u].
// Note: recursive (depth = tree height), so a 2e5 path needs a big stack.

#include <bits/stdc++.h>
using namespace std;

struct DSU_On_Tree {
    int n, cur = 0; // cur = the statistic over the nodes counted now; here, distinct colours
    vector<vector<int>> g;
    vector<int> col, sz, cnt, ans;
    vector<char> big;

    DSU_On_Tree(int n) : n(n), g(n + 1), col(n + 1), sz(n + 1), ans(n + 1), big(n + 1) {}

    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void dfs(int u, int p) {
        sz[u] = 1;
        for (auto v : g[u]) {
            if (v == p) continue;
            dfs(v, u);
            sz[u] += sz[v];
        }
    }

    void add(int u, int p, int x) { // fold u's subtree in (x=1) / out (x=-1), skip the kept child
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

    vector<int> run(int root = 1) { // ans[u] for every u in root's tree
        cnt.assign(*max_element(col.begin(), col.end()) + 1, 0);
        cur = 0;
        dfs(root, 0);
        dsu(root, 0, 1);
        return ans;
    }
};
