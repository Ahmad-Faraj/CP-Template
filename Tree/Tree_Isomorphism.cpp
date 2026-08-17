// Tree Isomorphism: a canonical string for a tree, so two trees are isomorphic exactly when the strings match.
// Use when: "are these two trees the same shape", counting distinct subtree shapes, hashing a tree's structure.
// Handles: rooted trees via canonical_rooted, unrooted via canonical_unrooted, which locates the centre itself.
// Time: O(n log n)
// Indexing: 1-based nodes; adj must be sized n + 1
// Note: an unrooted tree can have two centres - canonical_unrooted tries both and keeps the smaller string.

#include <bits/stdc++.h>
using namespace std;

string canonical_rooted(const vector<vector<int>> &adj, int node, int par = -1) {
    vector<string> children;
    for (int child : adj[node])
        if (child != par) children.push_back(canonical_rooted(adj, child, node));
    sort(children.begin(), children.end());
    string ret = "(";
    for (auto &c : children) ret += c;
    return ret + ")";
}

string node_rep(vector<vector<string>> &rep, int node) {
    sort(rep[node].begin(), rep[node].end());
    string ret = "(";
    for (auto &c : rep[node]) ret += c;
    return ret + ")";
}

string canonical_unrooted(const vector<vector<int>> &adj, int n) {
    if (n == 1) return "()";
    vector<int> degree(n + 1);
    for (int i = 1; i <= n; i++) degree[i] = (int)adj[i].size();

    queue<int> leafs;
    int remaining = n;
    for (int i = 1; i <= n; i++)
        if (degree[i] <= 1) leafs.push(i);

    vector<vector<string>> rep(n + 1);
    while (remaining > 2) { // peel one layer of leaves at a time until only the centre remains
        int layer = (int)leafs.size();
        while (layer--) {
            int leaf = leafs.front();
            leafs.pop();
            --remaining;
            string s = node_rep(rep, leaf);
            for (int p : adj[leaf]) {
                rep[p].push_back(s);
                if (--degree[p] == 1) leafs.push(p);
            }
        }
    }

    int r1 = leafs.front();
    leafs.pop();
    int r2 = leafs.empty() ? -1 : leafs.front();
    string s1 = node_rep(rep, r1);
    if (r2 == -1) return s1;
    string s2 = node_rep(rep, r2);
    rep[r1].push_back(s2);
    rep[r2].push_back(s1);
    return min(node_rep(rep, r1), node_rep(rep, r2));
}

// Standard problem: two unrooted trees; report whether they are isomorphic
void solve() {
    int n1, n2;
    cin >> n1 >> n2;
    vector<vector<int>> a(n1 + 1), b(n2 + 1);
    for (int i = 0, u, v; i < n1 - 1; i++) {
        cin >> u >> v;
        a[u].push_back(v), a[v].push_back(u);
    }
    for (int i = 0, u, v; i < n2 - 1; i++) {
        cin >> u >> v;
        b[u].push_back(v), b[v].push_back(u);
    }
    bool same = (n1 == n2) && canonical_unrooted(a, n1) == canonical_unrooted(b, n2);
    cout << (same ? "YES" : "NO") << '\n';
}
