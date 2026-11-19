// Inversions: counts pairs (i, j) with i < j and a[i] > a[j].
// Use when: "number of inversions", "min adjacent swaps to sort", "count later smaller values".
// Usage: ll inv = count_inversions(a);
// Handles: duplicates (equal values are not inversions), negatives, any type with operator<.
// Time: O(n log n)
// Indexing: 0-based vector
// Note: the answer can reach n(n-1)/2, so it is long long.

#include <bits/stdc++.h>
#pragma push_macro("int") // pb_ds breaks under #define int long long
#undef int
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#pragma pop_macro("int")
using namespace std;
using ll = long long;

template <class T> ll count_inversions(const vector<T> &a) {
    __gnu_pbds::tree<pair<T, int>, __gnu_pbds::null_type, less<pair<T, int>>,
                     __gnu_pbds::rb_tree_tag, __gnu_pbds::tree_order_statistics_node_update> se;
    ll ans = 0;
    for (int i = (int)a.size() - 1; i >= 0; i--) {
        ans += se.order_of_key({a[i], -1});
        se.insert({a[i], i});
    }
    return ans;
}
