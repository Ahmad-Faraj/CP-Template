// Segment Tree Merging: one sparse segment tree over values per node, merged upward so each holds its subtree.
// Use when: "for every vertex, something about the multiset of values in its subtree" - most frequent, k-th, counts.
// Handles: counts per value, k-th smallest, range counts, most frequent with ties; merging is DESTRUCTIVE.
// Time: O((n + q) log V) overall - each merge step destroys a node, so the total work is linear in nodes
// Indexing: values are 1..V; roots are per-vertex and start at 0, which is the shared empty tree
// Note: about 20*n nodes at 24 bytes each, so n = 2e5 over V = 2e5 needs roughly 100 MB. Reserve accordingly.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Segment_Tree_Merging {
    struct Node {
        int left = 0, right = 0;
        int count = 0, best = 0; // count in this range; best = the largest count at a single value
        ll best_sum = 0;         // sum of the values achieving that largest count
    };
    int V;
    vector<Node> t;

    Segment_Tree_Merging(int V, int reserve_nodes = 0) : V(V) {
        t.reserve(reserve_nodes + 1);
        t.push_back(Node()); // node 0 is the empty tree and must stay empty
    }

    int new_node() {
        t.push_back(Node());
        return (int)t.size() - 1;
    }

    void pull(int x) {
        int l = t[x].left, r = t[x].right;
        t[x].count = t[l].count + t[r].count;
        if (t[l].best >= t[r].best) {
            t[x].best = t[l].best;
            t[x].best_sum = t[l].best_sum + (t[l].best == t[r].best ? t[r].best_sum : 0);
        } else {
            t[x].best = t[r].best;
            t[x].best_sum = t[r].best_sum;
        }
    }

    // add delta to the count of `value`, returning the root - which may have just been created
    int update(int x, int value, int delta, int b = 1, int e = -1) {
        if (e == -1) e = V;
        if (!x) x = new_node();
        if (b == e) {
            t[x].count += delta;
            t[x].best = t[x].count;
            t[x].best_sum = t[x].count ? b : 0;
            return x;
        }
        int mid = b + (e - b) / 2;
        // the recursive call can reallocate the pool, so it must finish before t[x] is addressed
        if (value <= mid)
            t[x].left = update(t[x].left, value, delta, b, mid);
        else
            t[x].right = update(t[x].right, value, delta, mid + 1, e);
        pull(x);
        return x;
    }

    // fold b into a and return the new root; b must not be used afterwards
    int merge(int a, int b, int lo = 1, int hi = -1) {
        if (hi == -1) hi = V;
        if (!a || !b) return a | b;
        if (lo == hi) {
            t[a].count += t[b].count;
            t[a].best = t[a].count;
            t[a].best_sum = t[a].count ? lo : 0;
            return a;
        }
        int mid = lo + (hi - lo) / 2;
        t[a].left = merge(t[a].left, t[b].left, lo, mid);
        t[a].right = merge(t[a].right, t[b].right, mid + 1, hi);
        pull(a);
        return a;
    }

    int count(int x, int l, int r, int b = 1, int e = -1) { // how many entries have a value in [l, r]
        if (e == -1) e = V;
        if (!x || r < b || e < l) return 0;
        if (l <= b && e <= r) return t[x].count;
        int mid = b + (e - b) / 2;
        return count(t[x].left, l, r, b, mid) + count(t[x].right, l, r, mid + 1, e);
    }

    int kth(int x, int k, int b = 1, int e = -1) { // k-th smallest value, 1-based; -1 when k is too large
        if (e == -1) e = V;
        if (!x || k < 1 || k > t[x].count) return -1;
        if (b == e) return b;
        int mid = b + (e - b) / 2, left_count = t[t[x].left].count;
        return k <= left_count ? kth(t[x].left, k, b, mid) : kth(t[x].right, k - left_count, mid + 1, e);
    }

    int size(int x) { return t[x].count; }                       // how many entries in total
    int max_frequency(int x) { return t[x].best; }               // the largest count at any single value
    ll frequent_sum(int x) { return t[x].best_sum; }             // sum of the values attaining it
};

// Standard problem: CF 600E - a tree whose vertices carry colours; for every vertex report the sum of
// the colours that occur the maximum number of times in its subtree
void solve() {
    int n;
    cin >> n;
    vector<int> colour(n + 1);
    for (int i = 1; i <= n; i++) cin >> colour[i];
    vector<vector<int>> adj(n + 1);
    for (int i = 1, u, v; i < n; i++) {
        cin >> u >> v;
        adj[u].push_back(v), adj[v].push_back(u);
    }
    Segment_Tree_Merging st(n, 20 * n);
    vector<int> root(n + 1, 0), parent(n + 1, 0), order;
    vector<char> seen(n + 1, 0);
    vector<int> stack_{1};
    seen[1] = 1;
    while (!stack_.empty()) { // iterative, so a path-shaped tree is safe
        int u = stack_.back();
        stack_.pop_back();
        order.push_back(u);
        for (int v : adj[u])
            if (!seen[v]) seen[v] = 1, parent[v] = u, stack_.push_back(v);
    }
    vector<ll> answer(n + 1);
    for (int i = (int)order.size() - 1; i >= 0; i--) { // children before parents
        int u = order[i];
        root[u] = st.update(root[u], colour[u], 1);
        answer[u] = st.frequent_sum(root[u]);
        if (parent[u]) root[parent[u]] = st.merge(root[parent[u]], root[u]);
    }
    for (int v = 1; v <= n; v++) cout << answer[v] << " \n"[v == n];
}
