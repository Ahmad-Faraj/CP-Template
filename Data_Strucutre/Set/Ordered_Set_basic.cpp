#include "../../core.h"
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;

/*
 * Topic: Data Structures - Ordered Set / Multiset (Policy-Based Data Structure)
 * Description: A GCC-specific balanced BST that supports finding the k-th smallest element
 *              and counting the number of elements strictly less than a value in O(log N).
 */

// 1. Ordered Set (Unique elements only)
template <typename T> using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

// 2. Ordered Multiset (Allows duplicate elements)
// WARNING: With less_equal, mst.erase(x) doesn't work correctly to remove a single instance.
// To erase a single instance of x safely, you must use:
// mst.erase(mst.find_by_order(mst.order_of_key(x)));
template <typename T>
using ordered_multiset = tree<T, null_type, less_equal<T>, rb_tree_tag, tree_order_statistics_node_update>;

int main() {
    // --- Ordered Set Example ---
    ordered_set<int> st;
    st.insert(2);
    st.insert(2);
    st.insert(1);

    cout << "Set size: " << st.size() << '\n'; // 2 (duplicates ignored)
    cout << *st.find_by_order(0) << '\n';      // 1 (0-th smallest)
    cout << st.order_of_key(2) << '\n';        // 1 (elements strictly less than 2)

    // --- Ordered Multiset Example ---
    ordered_multiset<int> mst;
    mst.insert(2);
    mst.insert(2);
    mst.insert(1);

    cout << "Multiset size: " << mst.size() << '\n'; // 3
    cout << *mst.find_by_order(1) << '\n';           // 2 (1-st smallest)
    cout << mst.order_of_key(2) << '\n';             // 1 (elements strictly less than 2)

    // Erasing a single instance of 2 safely
    mst.erase(mst.find_by_order(mst.order_of_key(2)));
    cout << "Multiset size after erase: " << mst.size() << '\n'; // 2

    return 0;
}