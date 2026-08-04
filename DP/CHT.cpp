#include "../core.h"

/*
 * Topic: DP - Convex Hull Trick (CHT)
 * Description: Optimizes DP transitions of the form dp[i] = min/max(m[j] * x[i] + b[j])
 * where slopes m[j] are monotonic. If x[i] are also monotonic, we can use a pointer
 * to answer queries in O(1) amortized. Otherwise, we can use binary search in O(log N).
 * Input: Lines (m, b) added sequentially, and queries x.
 * Output: The minimum/maximum evaluated value of m*x + b across all lines.
 */

struct CHT {
    vector<ll> m, b;
    int ptr = 0;

    bool bad(int l1, int l2, int l3) {
        return 1.0 * (b[l3] - b[l1]) * (m[l1] - m[l2]) <=
               1.0 * (b[l2] - b[l1]) * (m[l1] - m[l3]); //(slope dec+query min),(slope inc+query max)
        return 1.0 * (b[l3] - b[l1]) * (m[l1] - m[l2]) >
               1.0 * (b[l2] - b[l1]) * (m[l1] - m[l3]); //(slope dec+query max), (slope inc+query min)
    }

    void add(ll _m, ll _b) {
        m.push_back(_m);
        b.push_back(_b);
        int s = m.size();
        while (s >= 3 && bad(s - 3, s - 2, s - 1)) {
            s--;
            m.erase(m.end() - 2);
            b.erase(b.end() - 2);
        }
    }

    ll f(int i, ll x) { return m[i] * x + b[i]; }

    //(slope dec+query min), (slope inc+query max) -> x increasing
    //(slope dec+query max), (slope inc+query min) -> x decreasing
    ll query(ll x) {
        if (ptr >= m.size()) ptr = m.size() - 1;
        while (ptr < m.size() - 1 && f(ptr + 1, x) < f(ptr, x)) ptr++;
        return f(ptr, x);
    }

    ll bs(int l, int r, ll x) {
        int mid = (l + r) / 2;
        if (mid + 1 < m.size() && f(mid + 1, x) < f(mid, x)) return bs(mid + 1, r, x); // > for max
        if (mid - 1 >= 0 && f(mid - 1, x) < f(mid, x)) return bs(l, mid - 1, x);       // > for max
        return f(mid, x);
    }
};

ll a[N], b[N];
CHT cht;
int32_t main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int n;
    cin >> n;
    for (int i = 0; i < n; i++) cin >> a[i];
    for (int i = 0; i < n; i++) cin >> b[i];
    cht.add(b[0], 0);
    ll ans = 0;
    for (int i = 1; i < n; i++) {
        ans = cht.query(a[i]);
        cht.add(b[i], ans);
    }
    cout << ans << '\n';
    return 0;
}