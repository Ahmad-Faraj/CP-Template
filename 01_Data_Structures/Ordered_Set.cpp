// Ordered Set: a std::set / std::multiset that also answers "k-th smallest" and "how many < x".
// Use when: rank of a value, k-th element of a changing set, count smaller elements so far.
// Usage: ordered_set<int> s; s.insert(x); *s.find_by_order(k); s.order_of_key(x);
//        ordered_multiset<int> m; m.insert(x); multiset_erase(m, x);
// Handles: order_of_key(x) = number of elements < x (both); find_by_order(k) = k-th smallest
//          (0-based), end() if k >= size. ordered_set also has find / erase / lower_bound.
// Time: O(log n) per operation
// Indexing: find_by_order is 0-based
// Note: ordered_multiset uses less_equal, so find / erase(value) silently fail and
//       lower_bound / upper_bound are swapped. Erase one copy with multiset_erase(m, x) only.

#include <bits/stdc++.h>
#pragma push_macro("int") // pb_ds breaks under #define int long long
#undef int
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#pragma pop_macro("int")
using namespace std;

template <class T>
using ordered_set = __gnu_pbds::tree<T, __gnu_pbds::null_type, less<T>, __gnu_pbds::rb_tree_tag,
                                     __gnu_pbds::tree_order_statistics_node_update>;
template <class T>
using ordered_multiset = __gnu_pbds::tree<T, __gnu_pbds::null_type, less_equal<T>,
                                          __gnu_pbds::rb_tree_tag,
                                          __gnu_pbds::tree_order_statistics_node_update>;

// removes one copy of x if present; returns whether something was erased
template <class S, class T> bool multiset_erase(S &s, const T &x) {
    auto it = s.find_by_order(s.order_of_key(x));
    if (it == s.end() || *it != x) return false;
    s.erase(it);
    return true;
}
