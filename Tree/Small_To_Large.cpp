// Small to Large: merges each child's container into the parent, always into the bigger one, over the whole tree.
// Use when: "for every subtree, the number of distinct values" and you want the shortest code that works.
// Handles: any mergeable container - set, map, multiset. Simpler than DSU on Tree, one log factor slower.
// Time: O(n log^2 n)
// Indexing: 1-based nodes
// Note: the swap before merging is the whole trick - never copy the larger container into the smaller one.

#include <bits/stdc++.h>
using namespace std;

const int MAX_N = 2e5;

vector<int> adj[MAX_N + 1];
set<int> colors[MAX_N + 1];
int distinct_num[MAX_N + 1];

void small_to_large(int node, int par) {
    for (int child : adj[node]) {
        if (child == par) continue;
        small_to_large(child, node);
        if (colors[node].size() < colors[child].size()) swap(colors[node], colors[child]);
        for (int item : colors[child]) colors[node].insert(item);
        colors[child].clear();
    }
    distinct_num[node] = (int)colors[node].size();
}

// Standard problem: for every node, how many distinct colours appear in its subtree
void solve() {
    int n;
    cin >> n;
    for (int i = 1; i <= n; i++) {
        int a;
        cin >> a;
        colors[i].insert(a);
    }
    for (int i = 1; i < n; i++) {
        int a, b;
        cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }
    small_to_large(1, 0);
    for (int i = 1; i <= n; i++) cout << distinct_num[i] << " \n"[i == n];
}
