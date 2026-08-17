// MEX on a Range: the mex of a[l..r] for many queries, answered offline in one sweep.
// Use when: "mex of the subarray [l, r]" asked q times on a static array, with all queries known up front.
// Handles: any non-negative values, duplicates, overlapping queries. Offline only - no updates.
// Time: build O(V) | all queries O((n + q) log V), V = size passed to the segment tree
// Indexing: 1-based positions in the array; values must fit inside the segment tree's size
// Note: size the tree by max(n, max value) + 1, not by n - a value beyond it is a out-of-bounds read.

#include <bits/stdc++.h>
using namespace std;

struct MexSegTree { // min over last-occurrence positions, with descent to find the mex
    int n;
    vector<int> tree;

    MexSegTree(int sz) {
        n = 1;
        while (n < sz) n <<= 1;
        tree.assign(2 * n, 0);
    }

    void set(int ind, int val) { // last occurrence of value ind is position val
        ind += n;
        tree[ind] = val;
        for (ind >>= 1; ind > 0; ind >>= 1) tree[ind] = min(tree[2 * ind], tree[2 * ind + 1]);
    }

    int get(int x) { // smallest value whose last occurrence is before x - that is the mex of [x .. current]
        int node = 1;
        while (node < n) {
            int left = node << 1;
            node = (tree[left] < x) ? left : left | 1;
        }
        return node - n;
    }
};

// Standard problem: q offline queries, each asking the mex of a[l .. r]
void solve() {
    int n;
    cin >> n;
    vector<int> a(n + 1);
    int mx = 0;
    for (int i = 1; i <= n; ++i) cin >> a[i], mx = max(mx, a[i]);
    int q;
    cin >> q;
    vector<vector<pair<int, int>>> queries(n + 1); // grouped by right endpoint
    for (int i = 0; i < q; ++i) {
        int l, r;
        cin >> l >> r;
        queries[r].push_back({l, i});
    }
    vector<int> res(q);
    MexSegTree s(max(n, mx) + 2);
    for (int i = 1; i <= n; ++i) {
        s.set(a[i], i);
        for (auto [l, ind] : queries[i]) res[ind] = s.get(l);
    }
    for (int x : res) cout << x << '\n';
}
