// CHT (monotonic): min of lines y = m x + b added with non-increasing slopes, O(1) amortised.
// Use when: dp[i] = min over j < i of dp[j] + b[j] * a[i] with b[j] decreasing (CF 319C), and
// the query x only grows (query) or is arbitrary (query_bs).
// Usage: CHT c; c.add(m, b); c.query(x) /* x non-decreasing */; c.query_bs(x) /* any x */;
// Handles: equal slopes, duplicate lines, negative m/b/x, adds interleaved with queries.
// Max: add(-m, -b) with slopes non-DEcreasing, negate the answer.
// Not: slopes in any order (use Li_Chao_Tree).
// Time: add O(1) amortised | query O(1) amortised | query_bs O(log n)
// Indexing: 0-based lines in insertion order (only the hull survives)
// Note: query on an empty hull is undefined. |m*x + b| must fit in ll; the hull test is __int128.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct CHT {
    vector<ll> m, b;
    int ptr = 0;

    bool bad(int l1, int l2, int l3) { // l2 is never strictly below both l1 and l3
        return (__int128)(b[l3] - b[l1]) * (m[l1] - m[l2]) <=
               (__int128)(b[l2] - b[l1]) * (m[l1] - m[l3]);
    }

    void add(ll _m, ll _b) {
        if (!m.empty() && m.back() == _m) { // equal slopes: keep only the lower line
            if (b.back() <= _b) return;
            m.pop_back(), b.pop_back();
        }
        m.push_back(_m);
        b.push_back(_b);
        int s = m.size();
        while (s >= 3 && bad(s - 3, s - 2, s - 1)) {
            s--;
            m.erase(m.end() - 2);
            b.erase(b.end() - 2);
        }
        ptr = min(ptr, s - 1); // ptr's line may be gone; the new last line replaces it
    }

    ll f(int i, ll x) { return m[i] * x + b[i]; }

    ll query(ll x) { // x must be >= every earlier x passed to query()
        while (ptr < (int)m.size() - 1 && f(ptr + 1, x) < f(ptr, x)) ptr++;
        return f(ptr, x);
    }

    ll bs(int l, int r, ll x) {
        int mid = (l + r) / 2;
        if (mid + 1 < (int)m.size() && f(mid + 1, x) < f(mid, x)) return bs(mid + 1, r, x);
        if (mid - 1 >= 0 && f(mid - 1, x) < f(mid, x)) return bs(l, mid - 1, x);
        return f(mid, x);
    }

    ll query_bs(ll x) { return bs(0, m.size() - 1, x); } // any x, does not move ptr
};
