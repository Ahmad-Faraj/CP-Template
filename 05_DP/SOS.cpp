// SOS DP: in place, f[mask] becomes the sum of f over all submasks (or all supermasks) of mask.
// Use when: "for each x count the y with y & x == y" (x | y == x), pairs by AND/OR, subset sums.
// Usage: vector<ll> f(1 << B); f[a[i]]++; sum_over_subsets(f); // f[x] = #{i : a[i] submask of x}
// CSES 1654: x|y == x -> sub[x]; x&y == x -> sup[x]; x&y != 0 -> n - sub[~x & (2^B - 1)].
// Time: O(B * 2^B)
// Indexing: 0-based masks; f.size() must be a power of two, 2^B
// Note: B = 20 is 2^20 entries; with long long that is 8 MB per array.

#include <bits/stdc++.h>
using namespace std;

template <class T> void sum_over_subsets(vector<T> &f) {
    int n = f.size();
    for (int bit = 1; bit < n; bit <<= 1)
        for (int mask = 0; mask < n; mask++)
            if (mask & bit) f[mask] += f[mask ^ bit];
}

template <class T> void sum_over_supersets(vector<T> &f) {
    int n = f.size();
    for (int bit = 1; bit < n; bit <<= 1)
        for (int mask = n - 1; mask >= 0; mask--)
            if (!(mask & bit)) f[mask] += f[mask ^ bit];
}
