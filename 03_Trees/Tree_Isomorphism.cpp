// Tree Isomorphism: a canonical bracket string per tree; isomorphic exactly when the strings match.
// Use when: "are these two trees the same shape", rooted or unrooted.
// Usage: canonical_rooted(adj, root) == canonical_rooted(adj2, root2); canonical_unrooted(adj, n).
// Handles: rooted (fixed roots) and unrooted (tries both centres, keeps the smaller string); n = 1.
// Time: O(n * height) time AND memory (the strings): random 2e5 tree 0.1s, but a 2e4 path 0.4s,
//   a 5e4 path 5.6s / ~2.5 GB, a 1e5 path runs out of memory. Deep trees need hashing instead.
// Indexing: 1-based nodes; adj sized n + 1 (index 0 unused)
// Note: different sizes give different strings, so no separate n1 == n2 check is needed.
// Note: canonical_rooted recurses (depth = tree height), so deep trees need a big stack.

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
    rep[r1].push_back(s2); // root at r1 with r2 hanging below, and vice versa
    rep[r2].push_back(s1);
    return min(node_rep(rep, r1), node_rep(rep, r2));
}
