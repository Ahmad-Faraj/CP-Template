#include "../../core.h"

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
    
    // Set to true for: (slope dec + query min) or (slope inc + query max)
    // Set to false for: (slope dec + query max) or (slope inc + query min)
    bool is_min_query = true;

    bool bad(int l1, int l2, int l3) {
        if (is_min_query)
            return (__int128_t)(b[l3] - b[l1]) * (m[l1] - m[l2]) <=
                   (__int128_t)(b[l2] - b[l1]) * (m[l1] - m[l3]);
        else
            return (__int128_t)(b[l3] - b[l1]) * (m[l1] - m[l2]) >
                   (__int128_t)(b[l2] - b[l1]) * (m[l1] - m[l3]);
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

    // Use when x values being queried are monotonic (increasing for min, decreasing for max)
    ll query(ll x) {
        if (ptr >= m.size()) ptr = m.size() - 1;
        while (ptr < m.size() - 1 && f(ptr + 1, x) < f(ptr, x)) ptr++;
        return f(ptr, x);
    }

    // Use when x values being queried are NOT monotonic
    ll bs(int l, int r, ll x) {
        int mid = (l + r) / 2;
        if (mid + 1 < m.size() && f(mid + 1, x) < f(mid, x)) return bs(mid + 1, r, x); // Change < to > for max
        if (mid - 1 >= 0 && f(mid - 1, x) < f(mid, x)) return bs(l, mid - 1, x);       // Change < to > for max
        return f(mid, x);
    }
};