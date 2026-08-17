// MEX of All Subarray MEXes: takes the mex of every subarray, then reports the mex of that whole collection.
// Use when: exactly this question - "which is the smallest positive integer that is no subarray's mex".
// Handles: values in [1, n]. Here mex means the smallest POSITIVE integer absent, matching the classic statement.
// Time: O(n log n)
// Indexing: 1-based array positions
// Note: for the mex of one given range use MEX_Range_Query.cpp - this file answers a different question.

#include <bits/stdc++.h>
using namespace std;

const int inf = 1e9;

struct ST { // min over last-occurrence positions
    int n;
    vector<int> t;

    void init(int sz) {
        n = sz;
        t.assign(4 * (n + 2), 0);
    }

    void upd(int node, int b, int e, int i, int x) { // last occurrence of value i is position x
        if (b > i || e < i) return;
        if (b == e) {
            t[node] = x;
            return;
        }
        int mid = (b + e) >> 1, l = node << 1, r = l | 1;
        upd(l, b, mid, i, x);
        upd(r, mid + 1, e, i, x);
        t[node] = min(t[l], t[r]);
    }

    int get_min(int node, int b, int e, int i, int j) { // min last-occurrence over values [i, j]
        if (b > j || e < i) return inf;
        if (b >= i && e <= j) return t[node];
        int mid = (b + e) >> 1, l = node << 1, r = l | 1;
        return min(get_min(l, b, mid, i, j), get_min(r, mid + 1, e, i, j));
    }

    int get_mex(int node, int b, int e, int i) { // mex of the window that starts at i and ends at the current index
        if (b == e) return b;
        int mid = (b + e) >> 1, l = node << 1, r = l | 1;
        if (t[l] >= i) return get_mex(r, mid + 1, e, i);
        return get_mex(l, b, mid, i);
    }
} t;

// Standard problem: the smallest positive integer that is not the mex of any subarray (CF 1436E)
void solve() {
    int n;
    cin >> n;
    vector<int> a(n + 1), f(n + 3, 0);
    for (int i = 1; i <= n; i++) cin >> a[i], --a[i];
    t.init(n);

    set<array<int, 3>> seg; // for the current right end i, [x[0]..i] .. [x[1]..i] all have mex x[2]
    for (int i = 1; i <= n; i++) {
        int x = a[i];
        int r = min(i - 1, t.get_min(1, 0, n, 0, x - 1)); // x == 0 gives an empty range, so this is i - 1
        int l = t.get_min(1, 0, n, 0, x) + 1;
        if (l <= r) {
            auto it = seg.lower_bound({l, -1, -1});
            while (it != seg.end() && (*it)[1] <= r) it = seg.erase(it);
        }
        t.upd(1, 0, n, x, i);
        for (int j = r; j >= l;) {
            int m = t.get_mex(1, 0, n, j);
            int L = max(l, t.get_min(1, 0, n, 0, m) + 1);
            f[m] = 1;
            seg.insert({L, j, m});
            j = L - 1;
        }
        int m = !a[i];
        seg.insert({i, i, m});
        f[m] = 1;
    }
    int ans = 0;
    while (f[ans]) ++ans;
    cout << ans + 1 << '\n';
}
