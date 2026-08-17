// Sparse Table: range query on a static array - no updates, ever.
// Use when: "min/max/gcd on [l, r]" with no updates and many queries; RMQ in O(1) after an O(n log n) build.
// Handles: idempotent merges in O(1) via query(); any associative merge in O(log n) via query_any(). Static only.
// Time: build O(n log n) | query O(1) | query_any O(log n) | memory O(n log n)
// Indexing: 0-based, ranges inclusive
// Note: change merge() and none together - none is the identity. query() is valid only for idempotent merges.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <typename T> struct SparseTable {
    int n, k;
    vector<vector<T>> table;

    T none = numeric_limits<T>::max();
    T merge(T a, T b) { return min(a, b); }

    SparseTable(const vector<T> &a) {
        n = (int)a.size();
        k = n ? 32 - __builtin_clz(n) : 1;
        table.assign(k, vector<T>(n));
        for (int i = 0; i < n; i++) table[0][i] = a[i];
        for (int j = 1; j < k; j++)
            for (int i = 0; i + (1 << j) <= n; i++)
                table[j][i] = merge(table[j - 1][i], table[j - 1][i + (1 << (j - 1))]);
    }

    T query(int l, int r) { // merge over a[l .. r], idempotent merges only
        int j = 31 - __builtin_clz(r - l + 1);
        return merge(table[j][l], table[j][r - (1 << j) + 1]);
    }

    T query_any(int l, int r) { // merge over a[l .. r] for any associative merge
        T ret = none;
        for (int j = k - 1; j >= 0; j--)
            if (l + (1 << j) - 1 <= r) ret = merge(ret, table[j][l]), l += 1 << j;
        return ret;
    }
};

// Standard problem: static range minimum queries (CSES 1647)
void solve() {
    int n, q;
    cin >> n >> q;
    vector<ll> a(n);
    for (auto &x : a) cin >> x;
    SparseTable<ll> st(a);
    while (q--) {
        int l, r;
        cin >> l >> r;
        cout << st.query(l - 1, r - 1) << '\n';
    }
}
