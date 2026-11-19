// MEX Range Query: the mex of a[l..r] for many queries on a static array, offline in one sweep.
// Use when: "mex of the subarray [l, r]" asked q times, all queries known up front, no updates.
// Usage: vector<int> ans = mex_queries(a, qs);  qs = {{l, r}, ...}
//        MexSegTree alone (online sweep): s(n + 1); for each r, if 0 <= a[r] < n
//        s.set(a[r], r + 1); then mex(a[l..r]) = s.get(l + 1).
// Handles: any values (negatives, values >= n), duplicates, repeated queries.
// Time: O((n + q) log n)
// Indexing: 0-based positions, queries [l, r] inclusive

#include <bits/stdc++.h>
using namespace std;

struct MexSegTree {
    int n;
    vector<int> tree;

    MexSegTree(int sz) {
        n = 1;
        while (n < sz) n <<= 1;
        tree.assign(2 * n, 0);
    }

    void set(int ind, int val) { // value ind was last seen at position val
        ind += n;
        tree[ind] = val;
        for (ind >>= 1; ind > 0; ind >>= 1) tree[ind] = min(tree[2 * ind], tree[2 * ind + 1]);
    }

    int get(int x) { // mex of 1-based positions [x .. last set]
        int node = 1;
        while (node < n) node = tree[2 * node] < x ? 2 * node : 2 * node + 1;
        return node - n;
    }
};

vector<int> mex_queries(const vector<int> &a, const vector<pair<int, int>> &qs) {
    int n = (int)a.size();
    vector<vector<pair<int, int>>> at(n);
    for (int i = 0; i < (int)qs.size(); i++) at[qs[i].second].push_back({qs[i].first, i});
    vector<int> res(qs.size());
    MexSegTree s(n + 1);
    for (int i = 0; i < n; i++) {
        if (0 <= a[i] && a[i] < n) s.set(a[i], i + 1);
        for (auto [l, id] : at[i]) res[id] = s.get(l + 1);
    }
    return res;
}
