// Cartesian Tree: the tree whose in-order traversal is the array and which is a heap by value, built in O(n).
// Use when: range minimum as an LCA, counting subarrays where a[i] is the max, divide and conquer over minima.
// Handles: duplicates - ties break by index, so the tree is unique. Max-heap by default.
// Time: build O(n)
// Indexing: 1-based nodes; l[i], r[i] and parent[i] are 0 when absent
// Note: for a min-heap, flip the comparison marked in build() and use +i instead of -i in the key.

#include <bits/stdc++.h>
using namespace std;
#define sz(x) (int)(x).size()

struct CartesianTree {
    int n = 0, root = 0;
    vector<int> l, r, parent;
    vector<pair<int, int>> a;

    int build(const vector<int> &v) { // returns the root
        n = sz(v);
        l.assign(n + 2, 0), r.assign(n + 2, 0), parent.assign(n + 2, 0);
        a.assign(n + 2, {});
        for (int i = 1; i <= n; i++) a[i] = {v[i - 1], -i}; // -i for a max-heap, +i for a min-heap

        vector<int> st(n + 2, 0);
        int tot = 0;
        for (int i = 1; i <= n; i++) {
            int k = tot;
            while (k > 0 && a[st[k - 1]] < a[i]) k--; // use > for a min-heap
            if (k) r[st[k - 1]] = i;
            if (k < tot) l[i] = st[k];
            st[k++] = i;
            tot = k;
        }
        for (int i = 1; i <= n; i++) {
            if (l[i]) parent[l[i]] = i;
            if (r[i]) parent[r[i]] = i;
        }
        root = 0;
        for (int i = 1; i <= n; i++)
            if (!parent[i]) root = i;
        return root;
    }
};

// Standard problem: print the parent of every node in the Cartesian tree, 0 meaning the root
void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (auto &x : a) cin >> x;
    CartesianTree ct;
    ct.build(a);
    for (int i = 1; i <= n; i++) cout << ct.parent[i] << " \n"[i == n];
}
