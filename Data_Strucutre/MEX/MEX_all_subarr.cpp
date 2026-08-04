#include "../../core.h"

/*
 * Topic: Data Structures - MEX of All Subarrays
 * Description: Finds the MEX for all subarrays. Uses a Segment Tree.
 */

const int N = 1e5 + 9, inf = 1e9;

struct ST {
    int t[4 * N];
    ST() {}
    void build(int n, int b, int e) {
        t[n] = 0;
        if (b == e) {
            return;
        }
        int mid = (b + e) >> 1, l = n << 1, r = l | 1;
        build(l, b, mid);
        build(r, mid + 1, e);
        t[n] = min(t[l], t[r]);
    }
    void update(int n, int b, int e, int i, int x) {
        if (b > i || e < i) return;
        if (b == e && b == i) {
            t[n] = x;
            return;
        }
        int mid = (b + e) >> 1, l = n << 1, r = l | 1;
        update(l, b, mid, i, x);
        update(r, mid + 1, e, i, x);
        t[n] = min(t[l], t[r]);
    }
    int query_min(int n, int b, int e, int i, int j) {
        if (b > j || e < i) return inf;
        if (b >= i && e <= j) return t[n];
        int mid = (b + e) >> 1, l = n << 1, r = l | 1;
        int L = query_min(l, b, mid, i, j);
        int R = query_min(r, mid + 1, e, i, j);
        return min(L, R);
    }
    int query_mex(int n, int b, int e, int i) { // mex of [i... cur_id]
        if (t[n] >= i) return inf;
        if (b == e) return b;
        int mid = (b + e) >> 1, l = n << 1, r = l | 1;
        if (t[l] < i) return query_mex(l, b, mid, i);
        return query_mex(r, mid + 1, e, i);
    }
} t;

int a[N], f[N];
int32_t main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n;
    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
        --a[i];
    }
    t.build(1, 0, n);
    set<array<int, 3>> seg; // for cur_id = i, [x[0]...i], [x[0] + 1...i], ... [x[1]...i] has mex x[2]
    for (int i = 1; i <= n; i++) {
        int x = a[i];
        int r = min(i - 1, t.query_min(1, 0, n, 0, x - 1));
        int l = t.query_min(1, 0, n, 0, x) + 1;
        if (l <= r) {
            auto it = seg.lower_bound({l, -1, -1});
            while (it != seg.end() && (*it)[1] <= r) {
                auto x = *it;
                it = seg.erase(it);
            }
        }
        t.update(1, 0, n, x, i);
        for (int j = r; j >= l;) {
            int m = t.query_mex(1, 0, n, j);
            int L = max(l, t.query_min(1, 0, n, 0, m) + 1);
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
    // MEX of All Subarray MEXes
    cout << ans + 1 << '\n';
    return 0;
}