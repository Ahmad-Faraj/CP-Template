#include "../../core.h"

/*
 * Topic: DP - Convex Hull Trick (CHT)
 * Description: Convex Hull Trick reduces the time complexity of DP transitions of
 *   the form dp[i] = min/max(m[j] * x[i] + b[j]) from O(N^2) to O(N) or O(N log N).
 * Condition (Deque CHT): Use when BOTH the slopes of the added lines m[j] are monotonic 
 *                        (e.g., sorted) AND the query coordinates x[i] are monotonic. O(N) amortized.
 * Condition (Binary Search CHT): Use when slopes m[j] are monotonic, but queries x[i] are random. O(N log N).
 * Note: If NEITHER slopes nor queries are monotonic, use Li-Chao Tree instead.
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
/* 
 * ---------------------------------------------------------
 * ALTERNATIVE: O(N) Deque CHT
 * Use this strictly when BOTH slopes and queries are monotonic.
 * ---------------------------------------------------------
 * struct Line { long long m, c; };
 * deque<Line> dq;
 * auto intersect = [](Line a, Line b) { return 1.0 * (b.c - a.c) / (a.m - b.m); };
 * 
 * // Add line (assuming slopes are monotonic)
 * while (dq.size() >= 2 && intersect(dq.back(), line) <= intersect(dq[dq.size()-2], dq.back()))
 *     dq.pop_back();
 * dq.push_back(line);
 * 
 * // Query x (assuming queries are monotonic)
 * while (dq.size() >= 2 && intersect(dq[0], dq[1]) <= x)
 *     dq.pop_front();
 * long long ans = dq.front().m * x + dq.front().c;
 */
