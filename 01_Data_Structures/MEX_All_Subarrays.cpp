// MEX All Subarrays: which values occur as the mex of some subarray, and the mex of all of them.
// Use when: "mex of the mexes of all subarrays" (CF 1436E), "can some subarray have mex k".
// Usage: vector<bool> f = subarray_mexes(a); int m = mex_of_subarray_mexes(a);
// Handles: any values (negatives and values >= n are ignored), n = 0.
// Time: O(n log n)
// Indexing: a is 0-based; f has size n + 2 and f[k] = 1 iff some non-empty subarray has mex k
// Note: mex here is the smallest missing NON-NEGATIVE integer. CF 1436E uses values 1..n and a
//       positive mex: subtract 1 from every value, then answer mex_of_subarray_mexes(a) + 1.

#include <bits/stdc++.h>
using namespace std;

vector<bool> subarray_mexes(vector<int> a) {
    int n = (int)a.size();
    const int inf = 1e9;
    vector<int> t(4 * (n + 1), 0);
    auto upd = [&](auto &self, int v, int b, int e, int i, int x) -> void {
        if (b == e) return void(t[v] = x);
        int mid = (b + e) / 2;
        if (i <= mid) self(self, 2 * v, b, mid, i, x);
        else self(self, 2 * v + 1, mid + 1, e, i, x);
        t[v] = min(t[2 * v], t[2 * v + 1]);
    };
    auto get_min = [&](auto &self, int v, int b, int e, int i, int j) -> int {
        if (b > j || e < i) return inf;
        if (b >= i && e <= j) return t[v];
        int mid = (b + e) / 2;
        return min(self(self, 2 * v, b, mid, i, j), self(self, 2 * v + 1, mid + 1, e, i, j));
    };
    auto get_mex = [&](auto &self, int v, int b, int e, int i) -> int {
        if (b == e) return b;
        int mid = (b + e) / 2;
        if (t[2 * v] >= i) return self(self, 2 * v + 1, mid + 1, e, i);
        return self(self, 2 * v, b, mid, i);
    };
    vector<bool> f(n + 2, false);
    for (int i = 1; i <= n; i++) {
        int x = a[i - 1];
        if (x < 0 || x > n) x = n;
        int r = min(i - 1, get_min(get_min, 1, 0, n, 0, x - 1));
        int l = get_min(get_min, 1, 0, n, 0, x) + 1;
        upd(upd, 1, 0, n, x, i);
        for (int j = r; j >= l;) {
            int m = get_mex(get_mex, 1, 0, n, j);
            int L = max(l, get_min(get_min, 1, 0, n, 0, m) + 1);
            f[m] = true;
            j = L - 1;
        }
        f[x == 0 ? 1 : 0] = true;
    }
    return f;
}

int mex_of_subarray_mexes(const vector<int> &a) {
    vector<bool> f = subarray_mexes(a);
    int m = 0;
    while (f[m]) m++;
    return m;
}
