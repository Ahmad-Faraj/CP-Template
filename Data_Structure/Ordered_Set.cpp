// Ordered Set: a std::set that also answers "k-th smallest" and "how many elements are below x".
// Use when: rank queries alongside insert and erase - k-th element, counting inversions, "position of x".
// Handles: insert, erase, find_by_order, order_of_key. A multiset variant is included, with its caveat.
// Time: every operation O(log n)
// Indexing: find_by_order is 0-based
// Note: the multiset uses less_equal, which breaks erase(value) and find() - erase via find_by_order instead.

#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace std;
using namespace __gnu_pbds;
using ll = long long;

template <typename T>
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

// duplicates allowed; erase(value) and find(value) do NOT work on this one
template <typename T>
using ordered_multiset = tree<T, null_type, less_equal<T>, rb_tree_tag, tree_order_statistics_node_update>;

// Standard problem: count inversions - pairs i < j with a[i] > a[j]
void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (auto &x : a) cin >> x;
    ordered_multiset<int> st;
    ll inversions = 0;
    for (int i = n - 1; i >= 0; i--) {
        inversions += st.order_of_key(a[i]); // already-seen values strictly smaller than a[i]
        st.insert(a[i]);
    }
    cout << inversions << '\n';

    ordered_set<int> s;
    for (int x : a) s.insert(x);
    if (!s.empty()) {
        cout << *s.find_by_order(0) << '\n';        // smallest distinct value
        cout << s.order_of_key(a[0]) << '\n';       // how many distinct values are below a[0]
    }
}
