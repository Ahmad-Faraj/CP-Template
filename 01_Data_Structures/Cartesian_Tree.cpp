// Cartesian Tree: the binary tree whose in-order walk is the array and which is a heap by value.
// Use when: RMQ as LCA (lca(i, j) = position of the max of a[i..j]), divide and conquer on the
//           max / min, "sum over subarrays of their max", subtree = range where a[i] is the max.
// Usage: Cartesian_Tree<int> ct(a); ct.root, ct.l[i], ct.r[i], ct.parent[i]; min-heap: ct(a, false)
// Handles: duplicates (the earlier equal element is the ancestor), n = 0.
// Time: build O(n)
// Indexing: a is 0-based; node i is a[i - 1], nodes 1..n; l, r, parent, root are 0 when absent
// Note: max-heap by default (parent >= child).

#include <bits/stdc++.h>
using namespace std;

template <typename T = int> struct Cartesian_Tree {
    int n, root = 0;
    vector<int> l, r, parent;

    Cartesian_Tree(const vector<T> &a, bool max_heap = true)
        : n((int)a.size()), l(n + 1, 0), r(n + 1, 0), parent(n + 1, 0) {
        auto beats = [&](int i, int j) {
            return max_heap ? a[j - 1] < a[i - 1] : a[i - 1] < a[j - 1];
        };
        vector<int> st(n + 1);
        int tot = 0;
        for (int i = 1; i <= n; i++) {
            int k = tot;
            while (k > 0 && beats(i, st[k - 1])) k--;
            if (k) r[st[k - 1]] = i;
            if (k < tot) l[i] = st[k];
            st[k++] = i;
            tot = k;
        }
        for (int i = 1; i <= n; i++) {
            if (l[i]) parent[l[i]] = i;
            if (r[i]) parent[r[i]] = i;
        }
        root = n ? st[0] : 0;
    }
};
